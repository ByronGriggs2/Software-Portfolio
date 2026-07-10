#include "SummonAttack.h"
#include "Definitions.h"
#include "unrealHelpers.h"
//#include "Passive.h"
#include "ChasingSummon.h"
//#include "Active.h"
#include "helpers.h"
#include "MyPlayer.h"

void SummonFactory::setLevel(int newLevel) {
	if (getLevel() == newLevel)
		return;
	int max = getMaxLevel();
	if (max <= 0) {
		LOGERROR("SummonFactory::setLevel - invalid max level");
		return;
	}
	if (newLevel < 0 || newLevel > max) {
		LOGERROR("SummonFactory::setLevel - level out of bounds");
		return;
	}
	if (!IsValid(_template)){
		LOGERROR("SummonFactory::setLevel - invalid template");
		return;
	}
	for (auto& summon : _pastSummons) {
		if (summon.IsValid())
			summon->setLevel(newLevel);
	}
	_level = newLevel;
}



SummonFactory::SummonFactory(SummonFactory&& other) :
	BaseAttackFactory(std::move(other)),
	_template(other._template)
{
	other._template = nullptr;
}

// Creating a dynamic template is uneccessary, because ACombatant creates a dynamic template for this summon factory (which is a weapon) in GiveWeapon().
SummonFactory::SummonFactory(ACombatant* owner, const USummonTemplate* temp) :
	BaseAttackFactory(owner),
	_template(temp)
{}

void SummonFactory::tick(float delta) {
	if (!_owner.IsValid())
		return;
	BaseAttackFactory::tick(delta);
	_pastSummons.RemoveAll([](const TWeakObjectPtr<ASummon>& ptr)
		{
			return !(ptr.IsValid());
		});
	_owner->getAttributes()->getSummonPassives(_passiveCache);
	for (auto& summon : _pastSummons) {
		updateSummon(summon.Get());
	}
}

void SummonFactory::updateSummon(ASummon* summon) {
	if (!IsValid(summon))
		return;
	summon->replacePassiveList(_passiveCache);
}

void SummonFactory::launchAttack(const FVector& forward) {
	if (!_owner.IsValid())
		return;
	if (!IsValid(_template) || !IsValid(_template->_summonConfig)) {
		LOGERROR("SummonFactory::launchAttack - invalid template");
		return;
	}
	APawn* player = UGameplayStatics::GetPlayerPawn(_owner.Get(), 0);
	AMyPlayer* castedPlayer = Cast<AMyPlayer>(player);
	if (!IsValid(castedPlayer))
		return;

	ASummon* summon = nullptr;
	SummonInitStruct tempInit = getSummonInit();

	if (_template->_summonConfig->_type == _CHASING) {
		AChasingSummon* temp = nullptr;
		if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AChasingSummon>(_owner.Get(), temp)) {
			LOGERROR("SummonFactory::launchAttack - failed to spawn summon");
			return;
		}
		temp->initialise_ASummon(tempInit);
		if (!unrealHelpers::finishDeferredSpawn<AChasingSummon>(_owner.Get(), temp)) {
			LOGERROR("SummonFactory::launchAttack - failed to finish spawning summon");
			return;
		}
		temp->setLevel(tempInit._level);
		summon = temp;
	}
	else {
		LOGERROR("SummonFactory::launchAttack - summon type not implemented");
		return;
	}
	summon->SetActorLocation(unrealHelpers::getRandomLocationOnScreen(castedPlayer), false, nullptr, ETeleportType::TeleportPhysics);
	_pastSummons.Add(summon);
	updateSummon(summon);
}

SummonInitStruct SummonFactory::getSummonInit() const {
	SummonInitStruct init;
	init._template = _template;
	init._level = getLevel();
	return init;
}

int SummonFactory::getMaxLevel() const {
	if (!IsValid(_template)) {
		LOGERROR("SummonFactory::getMaxLevel - invalid template");
		return -1;
	}
	return _template->_combatantLevels.Num()-1;
}

void USummonConfig::replaceOverrides() {
	if (unrealHelpers::isInvalidData(_targeting))
		_targeting = static_cast<ESummonTargeting>(0);
	if (unrealHelpers::isInvalidData(_type))
		_type = static_cast<ESummonType>(0);
	if (helpers::isInvalidData(_duration))
		_duration = _defaults._duration;
}

SummonFactory::~SummonFactory() {
	for (auto& summon : _pastSummons) {
		if (summon.IsValid())
			summon->onKilled();
	}
}

#if WITH_EDITOR
void USummonTemplate::PostEditChangeProperty(FPropertyChangedEvent& event) {
	Super::PostEditChangeProperty(event);
	if (event.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USummonTemplate, _combatantLevels)
		&& event.ChangeType == EPropertyChangeType::ArrayAdd
		&& _combatantLevels.Num() >= 2) {
		UCombatantAttributeData* Source = _combatantLevels[_combatantLevels.Num() - 2];
		if (IsValid(Source)) {
			FName NewName = MakeUniqueObjectName(this, UCombatantAttributeData::StaticClass());
			UCombatantAttributeData* NewLevel = DuplicateObject<UCombatantAttributeData>(Source, this, NewName);
			_combatantLevels.Last() = NewLevel;
		}
	}
}
#endif