#include "MyPlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "AssetRefs.h"
#include "ExperienceShard.h"
#include "EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatantManager.h"
#include "MyGameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Definitions.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SpriteSorter.h"
#include "PaperFlipbookComponent.h"
#include "CombatGameModeBase.h"
#include <cmath>
#include "WeaponContainerUnreal.h"
#include "WeaponContainer.h"
#include "unrealHelpers.h"
#include "InputGrabbingUserWidget.h"
#include "WidgetSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Active.h"
#include "CustomChannels.h"
#include "TestingDefinitions.h"

//void AMyPlayer::PostInitializeComponents() {
//	Super::PostInitializeComponents();
//}
//
//bool AMyPlayer::assetRefInitialise() {
//	UAssetRefs* refs = nullptr;
//	if (!MyGameplayStatics::getAssetRefs(refs)) {
//		LOGERROR("AMyPlayer::assetRefInitialise - failed to get asset refs");
//		return false;
//	}
//	const UCombatantTemplate* playerTemplate = refs->getBasePlayerTemplate();
//	if (!IsValid(playerTemplate)) {
//		LOGERROR("AMyPlayer::assetRefInitialise - failed to get player template");
//		return false;
//	}
//	initialise_AMyPlayer(playerTemplate);
//	return true;
//}

UCameraComponent* AMyPlayer::setupCamera(UCameraComponent* camera) {
	camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	camera->OrthoWidth = ORTHO_WIDTH;
	//aspect ratio
	camera->bConstrainAspectRatio = true;
	camera->AspectRatio = 1920.0 / 1080.0;
	//remove filters that can't be removed in settings
	camera->PostProcessSettings.bOverride_AutoExposureMethod = true;
	camera->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
	camera->PostProcessSettings.bOverride_AutoExposureBias = true;
	camera->PostProcessSettings.AutoExposureBias = 0.0;
	camera->PostProcessSettings.bOverride_AutoExposureApplyPhysicalCameraExposure = true;
	camera->PostProcessSettings.AutoExposureApplyPhysicalCameraExposure = false;
	camera->PostProcessSettings.bOverride_AutoExposureLowPercent = true;
	camera->PostProcessSettings.AutoExposureLowPercent = 1.0;
	camera->PostProcessSettings.bOverride_AutoExposureHighPercent = true;
	camera->PostProcessSettings.AutoExposureHighPercent = 1.0;
	//orientation
	camera->SetWorldRotation(FRotator(0, -90, 0), false, nullptr, ETeleportType::TeleportPhysics);
	camera->SetRelativeLocation(FVector(0, 100, 0), false, nullptr, ETeleportType::TeleportPhysics);
	return camera;
}

// ACombatant() creates the flipbook as a hitbox and disables automatic overlap events. We do not change that here because we manually check for overlaps in AMyPlayer::Tick
AMyPlayer::AMyPlayer() : AFriendlyCombatant() {
	// Create camera
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (!IsValid(_camera)) {
		LOGERROR("AMyPlayer::AMyPlayer - _camera creation failed");
		return;
	}
	//basics
	_camera->SetupAttachment(RootComponent);
	setupCamera(_camera);

	// replaced with a manual call in Tick since this doesn't do repeated checks.
	//OnActorBeginOverlap.AddDynamic(this, &AMyPlayer::onOverlapBegin);
}

void AMyPlayer::myInitialise(UCombatantTemplate* in) {
	if (!IsValid(in)) {
		LOGERROR("AMyPlayer::myInitialise - invalid parameter");
		return;
	}
	UPlayerTemplate* casted = Cast<UPlayerTemplate>(in);
	if (!IsValid(casted)) {
		LOGERROR("AMyPlayer::myInitialise - not a UPlayerTemplate");
		return;
	}
	initialise_AMyPlayer(casted);
}

bool AMyPlayer::onOverlapBegin_Impl(AActor* me, AActor* other) {
	if (Super::onOverlapBegin_Impl(me, other))
		return true;
	
	AExperienceShard* shard = Cast<AExperienceShard>(other);
	if (IsValid(shard)) {
		handleExperienceShardCollision(shard);
		return true;
	}
	return false;
}

void AMyPlayer::handleExperienceShardCollision(AExperienceShard* other) {
	if (!IsValid(other)) {
		LOGERROR("AMyPlayer::handleExperienceShardCollision - other is invalid");
		return;
	}
	if (other->isMagnetised()) {
		addExperience(other->getExperienceValue());
		other->suicide();
	}
	else {
		other->beginMagnetism(this);
	}
}

void AMyPlayer::addExperience(float val) {
	if (DISABLE_EXPERIENCE)
		return;
	_experience += val;
}

void AMyPlayer::levelUp() {
	_level += 1;
	_experience = std::max<float>(0, _experience - _maxExperience);
	_maxExperience *= PHI;
	_onLevelUp.Broadcast();
}

bool AMyPlayer::setIMCEnabled(inputContext type, bool val) {
	// get enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInput = nullptr;
	if (!MyGameplayStatics::myGetEnhancedInputLocalPlayerSubsystem(this, enhancedInput)) {
		LOGERROR("AMyPlayer::setIMCEnabled - can't get enhanced input local player subsystem");
		return false;
	}
	UAssetRefs* assetRefs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(assetRefs)) {
		LOGERROR("AMyPlayer::setIMCEnabled - can't get asset refs");
		return false;
	}
	UInputMappingContext* context;
	int priority = -1;
	switch (type) {
	case inputContext::action:
		context = assetRefs->getActionContext();
		priority = 1;
		break;
	default:
		LOGERROR("AMyPlayer::setIMCEnabled - invalid input context type");
		return false;
	}
	if (!IsValid(context)) {
		LOGERROR("AMyPlayer::setIMCEnabled - invalid input mapping context");
		return false;
	}
	if (val) {
		enhancedInput->AddMappingContext(context, priority, FModifyContextOptions());
	}
	else
		enhancedInput->RemoveMappingContext(context);
	return true;
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

		UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!IsValid(enhancedInput)) {
		LOGERROR("AMyPlayer::SetupPlayerInputComponent - invalid enhancedInput");
		return;
	}

	UAssetRefs* assetRefs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(assetRefs)) {
		return;
	}

	const UInputAction* move = assetRefs->getMoveAction();
	enhancedInput->BindAction(move, ETriggerEvent::Triggered, this, &AMyPlayer::handleMovement);
	const UInputAction* cancel = assetRefs->getCancelAction();
	enhancedInput->BindAction(cancel, ETriggerEvent::Triggered, this, &AMyPlayer::handleCancel);
}

void AMyPlayer::handleCancel(const FInputActionValue& in) {
	ACombatGameModeBase* mode = nullptr;
	if (!MyGameplayStatics::getCombatGameMode(this, mode)) {
		LOGERROR("AMyPlayer::handleCancel - failed to get combat game mode");
		return;
	}
	mode->onPlayerCancel();
}

bool AMyPlayer::checkDataValid(const UPlayerTemplate* data) {
	if (!IsValid(data) || !IsValid(data->_config)) {
		LOGERROR("AMyPlayer::initialise_AMyPlayer - invalid parameters");
		return false;
	}
	int gunCount = 0;
	int swordCount = 0;
	for (const auto& weapon : data->_config->_startingWeapons) {
		if (weapon->_weaponType == _SWORD)
			swordCount++;
		else if (weapon->_weaponType == _GUN)
			gunCount++;
		else {
			LOGERROR("AMyPlayer::initialise_AMyPlayer - uninitialised weapon");
			return false;
		}
	}
	int gunContainerCount = 0;
	int swordContainerCount = 0;
	if (data->_container0 == _SWORD)
		swordContainerCount++;
	else if (data->_container0 == _GUN)
		gunContainerCount++;
	else {
		LOGERROR("AMyPlayer::initialise_AMyPlayer - uninitialised container");
		return false;
	}
	if (data->_container1 == _SWORD)
		swordContainerCount++;
	else if (data->_container1 == _GUN)
		gunContainerCount++;
	else {
		LOGERROR("AMyPlayer::initialise_AMyPlayer - uninitialised container");
		return false;
	}
	if (gunCount > 3 * gunContainerCount || swordCount > 3 * swordContainerCount) {
		LOGERROR("AMyPlayer::initialise_AMyPlayer - not eno");
		return false;
	}
	return true;
}

void AMyPlayer::initialise_AMyPlayer(UPlayerTemplate* diskTemplate) {
	if (!checkDataValid(diskTemplate))
		return;
	const UPlayerTemplate* runtime = unrealHelpers::getDynamicTemplate(this, diskTemplate);
	auto initSword = [this]() {
		USword* temp = NewObject<USword>(this);
		temp->initialise_USword();
		return temp;
		};
	auto initGun = [this]() {
		UGun* temp = NewObject<UGun>(this);
		temp->initialise_UGun();
		return temp;
		};
	if (runtime->_container0 == _SWORD)
		_weaponContainers[0] = initSword();
	else
		_weaponContainers[0] = initGun();
	if (runtime->_container1 == _SWORD)
		_weaponContainers[1] = initSword();
	else
		_weaponContainers[1] = initGun();
	initialise_ACombatant(diskTemplate);
}

void AMyPlayer::BeginPlay() {
	ACombatant::BeginPlay();
	if (!setIMCEnabled(inputContext::action, true)) {
		LOGERROR("AMyPlayer::BeginPlay - failed to add action context");
		return;
	}
	UCombatantManager* combatantManager = nullptr;
	if (!MyGameplayStatics::getCombatantManager(this, combatantManager)) {
		LOGERROR("AMyPlayer::BeginPlay - failed to get combatant manager");
		return;
	}
	combatantManager->setPlayerRef(this);
	USpriteSorter* sorter = nullptr;
	if (!MyGameplayStatics::getSpriteSorter(this, sorter)) {
		LOGERROR("AMyPlayer::BeginPlay - failed to get sprite sorter");
		return;
	}
	sorter->sortSprite<AMyPlayer>(TArray<UPaperFlipbookComponent*>({ _combatantFlipbook, _overlayFlipbook }));
}

void AMyPlayer::PossessedBy(AController* NewController) {
	APlayerController* con = Cast<APlayerController>(NewController);
	if (!IsValid(con)) {
		LOGERROR("AMyPlayer::PossessedBy - new controller is not a player controller");
		return;
	}
	UWidgetSubsystem* sub = nullptr;
	if (!MyGameplayStatics::getWidgetSubsystem(sub))
		return;
	sub->setUIInputEnabled(false);
	FInputModeGameAndUI mode;
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	con->SetInputMode(mode);
	con->SetShowMouseCursor(true);
}

//void AMyPlayer::initWeaponContainers() {
//	int swordIndex = 0;
//	int gunIndex = 0;
//	int firstSword = -1;
//	int firstGun = -1;
//	{
//		int containerIndex = 0;
//		for (const auto& container : _weaponContainers) {
//			if (firstSword == -1 && IsValid(Cast<USword>(container)))
//				firstSword = containerIndex;
//			else if (firstGun == -1 && IsValid(Cast<UGun>(container)))
//				firstGun = containerIndex;
//			containerIndex++;
//		}
//	}
//	for (const auto& active : getAllActives()) {
//		EWeaponType type = active->getWeaponType();
//		if (type == _SWORD) {
//			if (firstSword == -1) {
//				LOGERROR("AMyPlayer::BeginPlay - no sword container found for active");
//				continue;
//			}
//			int res = _weaponContainers[firstSword]->getContainer()->addWeapon(active, swordIndex);
//			if (res == -1)
//				LOGERROR("AMyPlayer::BeginPlay - failed to add sword active to container");
//			else if (res == 1)
//				LOGERROR("AMyPlayer::BeginPlay - failed to add sword active to container slot occupied");
//			swordIndex++;
//		}
//		else if (type == _GUN) {
//			if (firstGun == -1) {
//				LOGERROR("AMyPlayer::BeginPlay - no gun container found for active");
//				continue;
//			}
//			int res = _weaponContainers[firstGun]->getContainer()->addWeapon(active, gunIndex);
//			if (res == -1)
//				LOGERROR("AMyPlayer::BeginPlay - failed to add gun active to container");
//			else if (res == 1)
//				LOGERROR("AMyPlayer::BeginPlay - failed to add gun active to container, slot occupied");
//			gunIndex++;
//		}
//		else {
//			LOGERROR("AMyPlayer::BeginPlay - unrecognized weapon type");
//		}
//	}
//}

bool AMyPlayer::isOutOfDeadzone(float x, float z) const {
	return std::sqrt(std::pow(x, 2) + std::pow(z, 2)) > 0.3;
}

void AMyPlayer::handleMovement(const FInputActionValue& rawInput) {
	auto input = rawInput.Get<FVector2d>();
	if (!isOutOfDeadzone(input.X, input.Y))
		return;
	_queuedMovement = input;
}

void AMyPlayer::Tick(float delta) {
	Super::Tick(delta);
	if (!IsValid(this))
		return;
	const FVector difference = FVector(_queuedMovement.X, 0, _queuedMovement.Y);
	float magnitude = difference.Size();
	const FVector direction = difference / magnitude;
	moveTowardsDirection(direction, delta, magnitude);
	_queuedMovement = FVector2D(0, 0);

	APlayerController* controller = nullptr;
	if (!MyGameplayStatics::myGetPlayerController(this, controller)) {
		ACombatant::Tick(delta);
		return;
	}
	double X = 0;
	double Y = 0;
	if (!controller->GetMousePosition(X, Y)) {
		return;
	}
	int32 viewX = 0;
	int32 viewY = 0;
	controller->GetViewportSize(viewX, viewY);

	const float directionX = X / static_cast<double>(viewX) - 0.5;
	const float directionZ = (Y / static_cast<double>(viewY) - 0.5) * -1.0;
	lookAtDirection(directionX, directionZ);

	FVector currentPos = GetActorLocation();
	currentPos.X = std::round(currentPos.X);
	currentPos.Z = std::round(currentPos.Z);
	FHitResult* throwaway = nullptr;
	_camera->SetWorldLocation(currentPos, false, throwaway, ETeleportType::TeleportPhysics);

	if (_experience >= _maxExperience)
		levelUp();
}

AMyPlayer* AMyPlayer::spawnAMyActorDeferred(UObject* worldContext, const FTransform& trans, AActor* deferredOwner, APawn* deferredInstigator) {
	UWorld* world = worldContext->GetWorld();
	if (!IsValid(world)) {
		LOGERROR("AMyPlayer::spawnAMyActorDeferred - invalid world");
		return nullptr;
	}
	return world->SpawnActorDeferred<AMyPlayer>(
		AMyPlayer::StaticClass(),
		trans,
		deferredOwner,
		deferredInstigator,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		ESpawnActorScaleMethod::MultiplyWithRoot
	);
}

void AMyPlayer::finishAMyActorDeferredSpawn(AMyPlayer* spawnedActor, const FTransform& trans) {
	if (!IsValid(spawnedActor)) {
		LOGERROR("AMyPlayer::finishAMyActorDeferredSpawn - spawnedActor is not valid");
		return;
	}
	UGameplayStatics::FinishSpawningActor(spawnedActor, trans, ESpawnActorScaleMethod::MultiplyWithRoot);
}

void AMyPlayer::onKilled() {
			

	ACombatGameModeBase* gameMode;
	if (MyGameplayStatics::getCombatGameMode(this, gameMode)) {
		gameMode->onPlayerDeath();
	}
	Super::onKilled();
}

UWeaponContainer* AMyPlayer::getWeaponContainer(int index) const {
	if (index < 0 || index >= 2) {
		LOGERROR("AMyPlayer::getWeaponContainer - index out of bounds");
		return nullptr;
	}
	return _weaponContainers[index].Get();
}

bool AMyPlayer::containersValid() const {
	if (!IsValid(_weaponContainers[0]) || !IsValid(_weaponContainers[1]) || _weaponContainers[0]->getContainer() == nullptr || _weaponContainers[1]->getContainer() == nullptr) {
		LOGERROR("AMyPlayer::canGiveWeapon - weapon containers not valid");
		return false;
	}
	return true;
}

bool AMyPlayer::canGiveWeapon(const UWeaponTemplate* weapon) const {
	// Weapon can be nullptr for legacy. Also because enemies don't have containers.
	if (!IsValid(weapon))
		return false;
	if (!containersValid())
		return false;
	if (!Super::canGiveWeapon(weapon))
		return false;
	if (weapon->_weaponType == _SWORD)
		return canGiveSword();
	else if (weapon->_weaponType == _GUN)
		return canGiveGun();
	else {
		LOGERROR("AMyPlayer::canGiveWeapon - unrecognized weapon type");
		return false;
	}
}
bool AMyPlayer::canGiveSword() const {
	for (const auto& container : _weaponContainers) {
		if (container->getContainer()->getType() == EWeaponType::sword) {
			if (!container->isFull()) {
				return true;
			}
		}
	}
	return false;
}
bool AMyPlayer::canGiveGun() const {
	for (const auto& container : _weaponContainers) {
		if (container->getContainer()->getType() == EWeaponType::gun) {
			if (!container->isFull()) {
				return true;
			}
		}
	}
	return false;
}

// if socketIndex is the default (-1) put in first available slot. Otherwise, put in that literal slot (wrapping from one container to the other)
void AMyPlayer::giveWeapon_safe(const UWeaponTemplate* data, int socketIndex) {
	auto getActualSocket = [this, socketIndex, &data]() -> int {
		if (socketIndex != -1)
			return socketIndex;

		const auto con1 = _weaponContainers[0]->getContainer();
		if (data->_weaponType == con1->getType()) {
			int temp = con1->getFirstEmptySocket();
			if (temp != -1)
				return temp;
		}
		const auto con2 = _weaponContainers[1]->getContainer();
		if (data->_weaponType == con2->getType()) {
			int temp = con2->getFirstEmptySocket();
			if (temp != -1)
				return temp + con1->getSockets().size();
		}
		return -1;
	};

	int actualSocket = getActualSocket();
	if (actualSocket == -1) {
		LOGERROR("AMyPlayer::giveWeapon_safe - no available socket for weapon");
		return;
	}
	WeaponContainer* con;
	if (actualSocket < _weaponContainers[0]->getContainer()->size()) {
		con = _weaponContainers[0]->getContainer();
	}
	else {
		con = _weaponContainers[1]->getContainer();
		actualSocket -= _weaponContainers[0]->getContainer()->size();
	}
	Super::giveWeapon_safe(data, actualSocket);
	int res = con->addWeapon(getAllActives().back(), actualSocket);
	if (res == -1)
		LOGERROR("AMyPlayer::giveWeapon_safe - failed to add weapon to container");
	else if (res == 1)
		LOGERROR("AMyPlayer::giveWeapon_safe - failed to add weapon to container slot occupied");
}

bool AMyPlayer::evolveWeapon(UWeaponTemplate_Evolution* evolution) {
	std::shared_ptr<Active> oldActive = getActiveBaseFromEvolution(evolution);
	if (oldActive == nullptr) {
		LOGERROR("AMyPlayer::evolveWeapon - no active found for evolution");
		return false;
	}
	if (!containersValid())
		return false;
	int socketIndex = _weaponContainers[0]->getContainer()->getIndexOfActive(oldActive);
	auto con = _weaponContainers[0]->getContainer();
	if (socketIndex == -1) {
		socketIndex = _weaponContainers[1]->getContainer()->getIndexOfActive(oldActive);
		con = _weaponContainers[1]->getContainer();
	}
	if (socketIndex == -1) {
		LOGERROR("AMyPlayer::evolveWeapon - active to evolve not found in any container");
		return false;
	}
	Super::evolveWeapon(evolution);
	int success = con->replaceWeapon(getAllActives().back(), socketIndex);
	if (success == -1) {
		LOGERROR("AMyPlayer::evolveWeapon - invalid slot");
		return false;
	}
	return true;
};

void AMyPlayer::setupFlipbook() {
	CustomChannels::setupCollision<AMyPlayer>(_combatantFlipbook);
}

void AMyPlayer::getOverlappingActors(TSet<AActor*>& ret) {
	if (!unrealHelpers::getOverlappingActors<AMyPlayer>(this, _combatantFlipbook, ret)) {
		LOGERROR("AMyPlayer::getOverlappingActors - failed to get overlapping actors");
		return;
	}
}