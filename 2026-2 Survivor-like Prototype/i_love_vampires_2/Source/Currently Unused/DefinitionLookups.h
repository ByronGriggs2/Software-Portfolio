#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatantTemplate.h"
#include "CombatantReferences.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API UCombatantReferences : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void setTableReference(UDataTable* newTable) { 
		combatantTable = newTable;
		UE_LOG(LogTemp, Warning, TEXT("Setter called. Table = %s, this = %p"),
			newTable ? *newTable->GetName() : TEXT("nullptr"), this);
	}

	UFUNCTION(BlueprintCallable)
	FCombatantTemplate getTemplate(FName ID);

private:
	UPROPERTY(VisibleAnywhere)
	UDataTable* combatantTable;
};