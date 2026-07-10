#pragma once

#include "CoreMinimal.h"
#include "FriendlyCombatant.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ActiveEnum.h"
#include "InputEnum.h"
#include "InputActionValue.h"
#include "InputGrabbingUserWidget.h"
#include "MyPlayer.generated.h"
class AExperienceShard;
struct FInputActionValue;
class UWeaponContainer;
class UWeaponTemplate_Evolution;

UCLASS()
class I_LOVE_VAMPIRES_2_API AMyPlayer : public AFriendlyCombatant
{
	GENERATED_BODY()

	const static inline EWeaponType _SWORD = EWeaponType::sword;
	const static inline EWeaponType _GUN = EWeaponType::gun;
	enum inputContext { action };
	friend class UInputGrabbingUserWidget;

	float _experience = 0;
	float _maxExperience = 8;
	float _level = 1;
	FVector2d _queuedMovement = FVector2d::ZeroVector;
	UPROPERTY(VisibleAnywhere) UCameraComponent* _camera = nullptr;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UWeaponContainer> _weaponContainers[2] = { nullptr, nullptr };

	bool containersValid() const;

private:
	void handleMovement(const FInputActionValue&);
	void handleCancel(const FInputActionValue&);

	void addExperience(float);
	void levelUp();
	bool isOutOfDeadzone(float, float) const;
	static bool checkDataValid(const UPlayerTemplate*);
	virtual void onKilled() override;
	virtual void giveWeapon_safe(const UWeaponTemplate* data, int socket = -1) override;

protected:
	virtual bool onOverlapBegin_Impl(AActor* left, AActor* right) override;
	virtual void setupFlipbook() override;
	virtual void getOverlappingActors(TSet<AActor*>& ret) override;

public:
	AMyPlayer();
	virtual void myInitialise(UCombatantTemplate* temp);
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float delta) override;
	void handleExperienceShardCollision(AExperienceShard* other);
	bool setIMCEnabled(inputContext type, bool val);
	virtual bool canGiveWeapon(const UWeaponTemplate* weapon = nullptr) const override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual bool evolveWeapon(UWeaponTemplate_Evolution* evolution) override;

	UFUNCTION(BlueprintCallable) void initialise_AMyPlayer(UPlayerTemplate* data);
	UFUNCTION(BlueprintCallable) static AMyPlayer* spawnAMyActorDeferred(UObject* worldContext, const FTransform& trans, AActor* deferredOwner = nullptr, APawn* deferredInstigator = nullptr);
	UFUNCTION(BlueprintCallable) static void finishAMyActorDeferredSpawn(AMyPlayer* deferredObject, const FTransform& trans);
	UFUNCTION(BlueprintPure) float getExperience() const { return _experience; }
	UFUNCTION(BlueprintPure) float getMaxExperience() const { return _maxExperience; }
	UFUNCTION(BlueprintPure) UWeaponContainer* getWeaponContainer(int index) const;
	UFUNCTION(BlueprintPure) bool canGiveSword() const;
	UFUNCTION(BlueprintPure) bool canGiveGun() const;
	UFUNCTION(BlueprintPure) float getOrthoWidth() const { return _camera->OrthoWidth; }
	UFUNCTION(BlueprintCallable) static UCameraComponent* setupCamera(UCameraComponent* in);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLevelUp);
	UPROPERTY(BlueprintAssignable, Category = "Events") FOnPlayerLevelUp _onLevelUp;
};	

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UPlayerTemplate : public UCombatantTemplate {
	GENERATED_BODY()

	const static inline EWeaponType _SWORD = EWeaponType::sword;
	const static inline EWeaponType _GUN = EWeaponType::gun;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EWeaponType _container0 = _SWORD;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EWeaponType _container1 = _GUN;

	virtual void replaceOverrides() override { Super::replaceOverrides(); }
	virtual void dynamicDeepCopy(UObject* context) override { Super::dynamicDeepCopy(context); }
	UPlayerTemplate(const FObjectInitializer& init) : Super(init){
		if (!IsValid(_config)) {
			LOGERROR("UPlayerTemplate::UPlayerTemplate - invalid config");
			return;
		}
		_config->_combatantClass = UPlayerTemplate::StaticClass();
	}
};