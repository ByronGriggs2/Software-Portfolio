#include "CombatantReferences.h"
//#include "CombatantTemplate.h"

FCombatantTemplate UCombatantReferences::getTemplate(FName enemyID) {
	UE_LOG(LogTemp, Warning, TEXT("Getter called. This = %p"), this);
	if (combatantTable == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("CombatantTable reference is null in CombatantReferences"));
		return FCombatantTemplate(); // return default struct to avoid crashing
	}
	FCombatantTemplate* row = combatantTable->FindRow<FCombatantTemplate>(enemyID, "", false);
	if (row) {
		return *row;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No row with ID %s found in CombatantTable"), *enemyID.ToString());
		return FCombatantTemplate(); // return default struct to avoid crashing
	}
}