#include "Combatant.h"
#include "CombatantAttributeSet.h"
#include "CombatantTemplate.h"
#include "StatusAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DefinitionLookups.h"

bool ACombatant::getDefinitionLookupSubsystem(UDefinitionLookups*& subsystem) {
	subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDefinitionLookups>();
	if (!IsValid(subsystem)) {
		LOGERROR("ACombatant::initialiseFromTemplate - failed to get UDefinitionLookups subsystem");
		return false;
	}
	return true;
}

ACombatant::ACombatant() : _combatantAttributeRefs()
{
	abilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	if (IsValid(abilitySystemComponent)) {
		{
			UCombatantAttributeSet* defaultSet = CreateDefaultSubobject<UCombatantAttributeSet>(TEXT("DefaultCombatantSet"));
			abilitySystemComponent->AddAttributeSetSubobject(defaultSet);
			combatantAttributes = abilitySystemComponent->GetSet<UCombatantAttributeSet>();
			UCombatantAttributeSet::GetAttributesFromSetClass(UCombatantAttributeSet::StaticClass(), _combatantAttributeRefs);
		}
		{
			UStatusAttributeSet* defaultSet = CreateDefaultSubobject<UStatusAttributeSet>(TEXT("DefaultStatusSet"));
			abilitySystemComponent->AddAttributeSetSubobject(defaultSet);
			statusAttributes = abilitySystemComponent->GetSet<UStatusAttributeSet>();
			//UStatusAttributeSet::GetAttributesFromSetClass(UStatusAttributeSet::StaticClass(), _statusAttributeRefs);
		}
	}
	else {
		checkSlow(false)
	}
}
void ACombatant::initialiseFromTemplate(const FCombatantTemplate& myTemplate)
{
	name = myTemplate.name;
	ID = myTemplate.ID;
	UDefinitionLookups* subsystem;
	if (!getDefinitionLookupSubsystem(subsystem))
		return;
	bool allFloats = false;
	TArray<float> values = subsystem->getFloatPropertiesAsArray(myTemplate.attributes, allFloats);
	if (!allFloats) {
		LOGERROR("ACombatant::initialiseFromTemplate - not all properties of template attributes are floats");
		return;
	}
	TArray<FName> keys = subsystem->combatantAttributeKeys();
	if (keys.Num() != values.Num()) {
		LOGERROR("ACombatant::initialiseFromTemplate - number of attribute keys does not match number of attribute values");
	}
	for (auto i = 0; i < keys.Num();i++) {
		FName key = keys[i];
		float value = values[i];
		FGameplayAttribute attribute = subsystem->getCombatantAttributeByName(key);
		abilitySystemComponent->SetNumericAttributeBase(attribute, value);
	}
}
void ACombatant::onAttributeChangedReroute(const FOnAttributeChangeData& data)
{
	UDefinitionLookups* subsystem;
	if (!getDefinitionLookupSubsystem(subsystem))
		return;
	TArray<FGameplayAttribute> allAttributes = subsystem->combatantAttributeValues();
	int index = allAttributes.IndexOfByKey(data.Attribute);
	if (index == INDEX_NONE) {
		LOGERROR("ACombatant::onAttributeChangedReroute - attribute not found in allAttributes");
		return;
	}
	onAttributeChanged(index, data.NewValue, data.OldValue);
}
void ACombatant::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (const auto& attr : _combatantAttributeRefs) {
		abilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attr).RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}