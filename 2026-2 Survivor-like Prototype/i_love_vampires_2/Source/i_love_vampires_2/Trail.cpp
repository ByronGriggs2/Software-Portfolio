#include "Trail.h"
#include "AOE.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "unrealHelpers.h"
#include "Definitions.h"
#include "AttackTrail.h"

void ATrailSegmentFactory::Tick(float delta) {
	maintainRefs();
	if (_template.get() == nullptr) {
		LOGERROR("TrailSegmentFactory::Tick - _template is not set");
		return;
	}
	const float totalDuration = _template->getTotalDuration();
	if (helpers::nearEq(totalDuration, -1)) {
		LOGERROR("TrailSegmentFactory::Tick - total duration is not valid");
		return;
	}
	if (_isTracking) {
		if (_trackedObject.IsValid()) {
			_template->_location = _trackedObject->GetActorLocation();
		}
		else {
			Destroy();
			return;
		}
	}
	_nextSpawn -= delta;
	if (_nextSpawn <= 0) {
		createTrailSegment();
		_nextSpawn = _period;
	}
	_elapsed += delta;
	if (totalDuration > 0 && _elapsed >= totalDuration) {
		Destroy();
	}
}
void ATrailSegmentFactory::maintainRefs() {
	auto lamb = [this](const TObjectPtr<UTrailSegmentWrapper>& element) {
		return !IsValid(element) || element->iWantToDie();
		};
	_refs.RemoveAll(lamb);
}
bool ATrailSegmentFactory::initialise_ATrailSegmentFactory(float period, const TrailSegmentCosmeticInitStruct& temp, AActor* trackedObject) {
	return initialise_ATrailSegmentFactory_internal<UTrailSegmentCosmeticWrapper, TrailSegmentCosmeticInitStruct>(period, temp, trackedObject);
}
bool ATrailSegmentFactory::initialise_ATrailSegmentFactory(float period, const TrailSegmentAttackInitStruct& temp, AActor* trackedObject) {
	return initialise_ATrailSegmentFactory_internal<UTrailSegmentAttackWrapper, TrailSegmentAttackInitStruct>(period, temp, trackedObject);
}
UTrailSegmentWrapper* ATrailSegmentFactory::createTrailSegment() const {
	if (!_constructSegmentFunction) {
		LOGERROR("TrailSegmentFactory::createTrailSegment - _constructSegmentFunction is not set");
		return nullptr;
	}
	return _constructSegmentFunction(_template.get());
}

void ATrailSegmentFactory::BeginPlay() {
	Super::BeginPlay();
	if (!_constructSegmentFunction) {
		LOGERROR("TrailSegmentFactory::BeginPlay - _constructSegmentFunction is not set");
		return;
	}
	if (_template.get() == nullptr) {
		LOGERROR("TrailSegmentFactory::BeginPlay - _template is not set");
		return;
	}
}

/////////////////////////////////////////////////////////////
// ATrailSegmentCosmetic
// Set up root component???
ATrailSegmentCosmetic::ATrailSegmentCosmetic(const FObjectInitializer& init) {
	PrimaryActorTick.bCanEverTick = true;
	_flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("flipbook"));
}

bool ATrailSegmentCosmetic::initialise_ATrailSegmentCosmetic(const TrailSegmentCosmeticInitStruct& init) {
	if (!unrealHelpers::initFlipbook(init._sprite, _flipbook)) {
		LOGERROR("ATrailSegmentCosmetic::initialise_ATrailSegmentCosmetic - failed to init flipbook");
		return false;
	}
	_duration = init._duration;
	SetActorScale3D(FVector(init._scale, init._scale, init._scale));
	return true;
}

void ATrailSegmentCosmetic::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(_flipbook)) {
		LOGERROR("ATrailSegmentCosmetic::BeginPlay - _flipbook is not valid");
		return;
	}
}

void ATrailSegmentCosmetic::Tick(float delta) {
	Super::Tick(delta);
	_elapsed += delta;
	if (_elapsed >= _duration) {
		Destroy();
	}
}

/////////////////////////////////////////////////////////////
// UTrailSegmentWrapper
void UTrailSegmentWrapper::Tick(float delta) {
	if (_fadeOut) {
		applyFade();
	}
}

bool UTrailSegmentWrapper::applyFade() {
	auto flipbook = getFlipbookRef();
	if (!IsValid(flipbook)) {
		LOGERROR("UTrailSegmentWrapper::Tick - flipbook is not valid");
		return false;
	}
	const float remaining = getRemainingDuration();
	if (helpers::nearEq(-1, remaining)) {
		LOGERROR("UTrailSegmentWrapper::Tick - remaining duration is not valid");
		return false;
	}
	const float total = getTotalDuration();
	if (helpers::nearEq(-1, total)) {
		LOGERROR("UTrailSegmentWrapper::Tick - total duration is not valid");
		return false;
	}
	const float proportion = remaining / total;
	if (proportion < 0 || proportion > 1) {
		LOGERROR("UTrailSegmentWrapper::Tick - proportion is out of bounds");
		return false;
	}
	float alpha = _originalColor.A * (1 - proportion);
	FLinearColor newColor = _originalColor;
	newColor.A = alpha;
	flipbook->SetSpriteColor(newColor);
	return true;
}

bool UTrailSegmentWrapper::postSpawn(const TrailSegmentInitStruct* init) {
	AActor* actor = getActor();
	if (!IsValid(actor) || init == nullptr) {
		LOGERROR("UTrailSegmentWrapper::postSpawn - parameter is not valid");
		return false;
	}
	UPaperFlipbookComponent* flipbook = getFlipbookRef();
	if (!IsValid(flipbook)) {
		LOGERROR("UTrailSegmentWrapper::postSpawn - flipbook is not valid");
		return false;
	}

	actor->SetActorLocation(init->_location);
	actor->SetActorRotation(FRotator(0, 0, init->_rotation));
	_originalColor = flipbook->GetSpriteColor();
	_fadeOut = init->_fadeOut;
	return true;
}

bool UTrailSegmentCosmeticWrapper::initialise_UTrailSegmentCosmeticWrapper(ATrailSegmentFactory* owner, const TrailSegmentCosmeticInitStruct& init) {
	if (!initialise_UTrailSegmentWrapper(owner))
		return false;
	{
		ATrailSegmentCosmetic* temp = nullptr;
		if (!unrealHelpers::spawnActorOnTopOfMeDeferred<ATrailSegmentCosmetic>(owner, temp)) {
			LOGERROR("UTrailSegmentCosmeticWrapper::UTrailSegmentCosmeticWrapper - failed to spawn ATrailSegmentCosmetic");
			return false;
		}
		_segment = temp;
	}
	_segment->initialise_ATrailSegmentCosmetic(init);
	if (!unrealHelpers::finishDeferredSpawn(owner, _segment.Get())) {
		LOGERROR("UTrailSegmentCosmeticWrapper::UTrailSegmentCosmeticWrapper - failed to finish spawning ATrailSegmentCosmetic");
		return false;
	}
	UTrailSegmentWrapper::postSpawn(&init);
	return true;
}
UPaperFlipbookComponent* UTrailSegmentCosmeticWrapper::getFlipbookRef() const {
	if (_segment.IsValid()) {
		return _segment->getFlipbookRef();
	}
	return nullptr;
}

bool UTrailSegmentAttackWrapper::initialise_UTrailSegmentAttackWrapper(ATrailSegmentFactory* owner, const TrailSegmentAttackInitStruct& init) {
	if (!initialise_UTrailSegmentWrapper(owner))
		return false;
	{
		AAttackTrail* temp = nullptr;
		if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AAttackTrail>(owner, temp)) {
			LOGERROR("UTrailSegmentAttackWrapper::UTrailSegmentAttackWrapper - failed to spawn ATrailSegmentAttack");
			return false;
		}
		_segment = temp;
	}
	_segment->initialise_AAttackTrail(init._caller, init._AOEInit);
	if (!unrealHelpers::finishDeferredSpawn<AAttackTrail>(owner, _segment.Get())) {
		LOGERROR("UTrailSegmentAttackWrapper::UTrailSegmentAttackWrapper - failed to finish spawning ATrailSegmentAttack");
		return false;
	}
	UTrailSegmentWrapper::postSpawn(&init);
	return true;
}
UPaperFlipbookComponent* UTrailSegmentAttackWrapper::getFlipbookRef() const {
	if (_segment.IsValid()) {
		return AAttackActorAttourney::getFlipbookRef(_segment.Get());
	}
	return nullptr;
}

AActor* UTrailSegmentAttackWrapper::getActor() const { return _segment.IsValid() ? _segment.Get() : nullptr; }
float UTrailSegmentAttackWrapper::getRemainingDuration() const { return _segment.IsValid() ? _segment->getRemainingDuration() : -1; }
float UTrailSegmentAttackWrapper::getTotalDuration() const { return _segment.IsValid() ? _segment->getTotalDuration() : -1; }
bool UTrailSegmentAttackWrapper::iWantToDie() const { return _segment.IsValid(); }