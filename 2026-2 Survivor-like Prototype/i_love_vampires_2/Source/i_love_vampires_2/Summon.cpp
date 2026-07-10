#include "Summon.h"
#include "SummonAttack.h"
#include "SummonEnum.h"
#include "Definitions.h"
#include "SpriteSorter.h"
#include "DynamicAssetManager.h"
#include "MyGameplayStatics.h"
#include "helpers.h"
#include "Active.h"
#include "CustomChannels.h"
#include "unrealHelpers.h"

// the disk template is not dynamically copied. ACombatant creates a dynamic copy of UCombatantConfig. This class creates a dynamic dopy of each UCombatantAttributeData
bool ASummon::initialise_ASummon(const SummonInitStruct& init) {
	if (!IsValid(init._template) || !IsValid(init._template->_summonConfig)) {
		LOGERROR("ASummon::initialise_ASummon - invalid template");
		return false;
	}
	if (init._template->_combatantLevels.Num() == 0) {
		LOGERROR("ASummon::initialise_ASummon - template has no levels");
		return false;
	}
	USpriteSorter* sorter = nullptr;
	if (!MyGameplayStatics::getSpriteSorter(this, sorter)) {
		LOGERROR("ASummon::initialise_ASummon - failed to get sprite sorter");
		return false;
	}
	_duration = init._template->_summonConfig->_duration;
	// ACombatant stores ACombatant->Config, but things are a lot easier if I just duplicate the data (technically it's only duplicating the pointer but still)
	_template = init._template;
	if (!initialise_ACombatant_noDynamic(_template->_config, _template->_combatantLevels[0])) {
		LOGERROR("ASummon::initialise_ASummon - failed to initialize config");
		return false;
	}
	sorter->sortSprite<ASummon>(TArray<UPaperFlipbookComponent*>({ _combatantFlipbook, _overlayFlipbook }));
	return true;
}

bool ASummon::setLevel(int newLevel) {
	UDynamicAssetManager* manager = nullptr;
	if (!MyGameplayStatics::getDynamicAssetManager(this, manager)) {
		LOGERROR("ASummon::setLevel - failed to get dynamic asset manager");
		return false;
	}
	int max = getMaxLevel();
	if (max <= 0) {
		LOGERROR("ASummon::setLevel - invalid max level");
		return false;
	}
	if (newLevel < 0 || (newLevel >= max)) {
		LOGERROR("ASummon::setLevel - level out of bounds");
		return false;
	}
	replaceAttributeData(_template->_combatantLevels[newLevel]);
	if (!IsValid(_template) || !IsValid(_template->_config)) {
		LOGERROR("ASummon::setLevel - invalid template");
		return false;
	}

	bool success = true;
	for (const auto& weapon : _template->_config->_startingWeapons) {
		auto diskWeapon = manager->getKey<UWeaponTemplate>(weapon);
		if (!IsValid(diskWeapon)) {
			LOGERROR("ASummon::setLevel - invalid weapon in template");
			return false;
		}
		success = success && setWeaponLevel(diskWeapon, newLevel);
	}
	return success;
}

int ASummon::getMaxLevel() const {
	if (!IsValid(_template)) {
		LOGERROR("ASummon::getMaxLevel - invalid template");
		return -1;
	}
	return _template->_combatantLevels.Num();
}

void ASummon::myInitialise(UCombatantTemplate* temp) {
	LOGERROR("ASummon::myInitialise - summons should not be initialized with a combatant template");
}

void ASummon::Tick(float delta) {
	TRACE_CPUPROFILER_EVENT_SCOPE(ASummon_Tick);
	Super::Tick(delta);
	if (!IsValid(this))
		return;
	if (!helpers::nearEq(999, _duration)) {
		_duration -= delta;
		if (_duration <= 0) {
			onKilled();
			return;
		}
	}
	Tick_Impl(delta);
}

void ASummon::setupFlipbook() {
	CustomChannels::setupCollision<ASummon>(_combatantFlipbook);
}

void ASummon::getOverlappingActors(TSet<AActor*>& ret) {
	if (!unrealHelpers::getOverlappingActors<ASummon>(this, _combatantFlipbook, ret)) {
		LOGERROR("AMyPlayer::getOverlappingActors - failed to get overlapping actors");
		return;
	}
}