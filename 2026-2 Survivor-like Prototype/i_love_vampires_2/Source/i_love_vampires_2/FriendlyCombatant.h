#pragma once
#include "CoreMinimal.h"
#include "Combatant.h"
#include "Definitions.h"
#include "FriendlyCombatant.generated.h"

class AEnemyBase;
UCLASS()
class I_LOVE_VAMPIRES_2_API AFriendlyCombatant : public ACombatant {
	GENERATED_BODY()

	void onOverlapBegin(AActor* left, AActor* right);
	void handleEnemyCollision(AEnemyBase* other);

protected:
	// Returns true iff the overlap has been handled. It's expected that inheritors will call "if(!Super()): check". Can assume parameters are valid.
	virtual bool onOverlapBegin_Impl(AActor* left, AActor* right);
	// collision may be different for player vs summon, so this has to be implemented in the subclass
	virtual void getOverlappingActors(TSet<AActor*>& ret)
		PURE_VIRTUAL(AFriendlyCombatant::getOverlappingActors,
			LOGERROR("getOverlappingActors not implemented in this class"););

public:
	virtual void Tick(float delta) override;
};