#pragma once
#include "GameFramework/Actor.h"
#include "Definitions.h"
#include <memory>
#include <functional>
#include "AOE.h"
#include "Tickable.h"
#include "Math/Color.h"
#include "Trail.generated.h"

/////////////////////////////////////////////////////////////
class UPaperFlipbook;
struct TrailSegmentInitStruct {
	virtual ~TrailSegmentInitStruct() = default;
	FVector _location = FVector::ZeroVector;
	float _rotation = 0;
	bool _fadeOut = false;

	virtual float getTotalDuration() const = 0;
};
struct TrailSegmentCosmeticInitStruct : public TrailSegmentInitStruct {
	UPaperFlipbook* _sprite = nullptr;
	float _scale = 1;
	float _totalDuration = 1;
	float _duration = 1;

	virtual float getTotalDuration() const override { return _totalDuration; }
};
class ATrailSegmentFactory;
struct TrailSegmentAttackInitStruct : public TrailSegmentInitStruct {
	AOEInitStruct _AOEInit;
	ATrailSegmentFactory* _caller;

	TrailSegmentAttackInitStruct() = delete;
	TrailSegmentAttackInitStruct(const AOEInitStruct& init) : _AOEInit(init) {}

	virtual float getTotalDuration() const override { return _AOEInit._AOEAttributes._duration.getFinal(); }
};
/////////////////////////////////////////////////////////////

class UTrailSegmentWrapper;
UCLASS()
class I_LOVE_VAMPIRES_2_API ATrailSegmentFactory : public AActor {
	GENERATED_BODY()

	float _period = 0;
	TWeakObjectPtr<AActor> _trackedObject = nullptr;
	bool _isTracking = false;
	float _nextSpawn = 0;
	float _elapsed = 0;
	std::unique_ptr<TrailSegmentInitStruct> _template = nullptr;
	std::function<UTrailSegmentWrapper*(const TrailSegmentInitStruct*)> _constructSegmentFunction;
	bool _initialised = false;
	UPROPERTY()
	TArray<TObjectPtr<UTrailSegmentWrapper>> _refs;

	UTrailSegmentWrapper* createTrailSegment() const;
	template<typename T, typename U>
	std::function<UTrailSegmentWrapper*(const TrailSegmentInitStruct*)> getSegmentFunction();
	template<typename T, typename U>
	bool initialise_ATrailSegmentFactory_internal(float period, const U& temp, AActor* trackedObject = nullptr);
	
public:
	ATrailSegmentFactory() : Super() { 
		PrimaryActorTick.bCanEverTick = true;
		if (!RootComponent) { RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")); }
	}
	bool initialise_ATrailSegmentFactory(float period, const TrailSegmentCosmeticInitStruct& temp, AActor* trackedObject = nullptr);
	bool initialise_ATrailSegmentFactory(float period, const TrailSegmentAttackInitStruct& temp, AActor* trackedObject = nullptr);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void maintainRefs();
};

template<typename T, typename U>
std::function<UTrailSegmentWrapper*(const TrailSegmentInitStruct*)> ATrailSegmentFactory::getSegmentFunction() {
	static_assert(std::is_base_of<UTrailSegmentWrapper, T>::value, "T must be derived from TrailSegmentWrapper");
	static_assert(std::is_base_of<TrailSegmentInitStruct, U>::value, "U must be derived from TrailSegmentInitStruct");
	// This would be useful if unreal didn't suck ass
	//static_assert(std::is_constructible<T, ATrailSegmentFactory*, const U&>::value, "T must be constructible with parameters (ATrailSegmentFactory*, const U&)");
	
	return [this](const TrailSegmentInitStruct* init) -> UTrailSegmentWrapper* {
		auto casted = dynamic_cast<const U*>(init);
		if (casted == nullptr) {
			LOGERROR("ATrailSegmentFactory::getSegmentFunction - casted parameter is null");
			return nullptr;
		}
		auto ret = NewObject<T>(this);
		ret->myInit(this, casted);
		_refs.Add(ret);
		return ret;
		};
}

template<typename T, typename U>
bool ATrailSegmentFactory::initialise_ATrailSegmentFactory_internal(float period, const U& temp, AActor* trackedObject) {
	static_assert(std::is_base_of<UTrailSegmentWrapper, T>::value, "T must be derived from TrailSegmentWrapper");
	static_assert(std::is_base_of<TrailSegmentInitStruct, U>::value, "U must be derived from TrailSegmentInitStruct");
	// This would be useful if unreal didn't suck ass
	//static_assert(std::is_constructible<T, ATrailSegmentFactory*, const U&>::value, "T must be constructible with parameters (ATrailSegmentFactory*, const U&)");

	if (_initialised) {
		LOGERROR("ATrailSegmentFactory::initialise_TrailSegmentFactory_internal - already initialised");
		return false;
	}
	_period = period;
	_trackedObject = trackedObject;
	_isTracking = trackedObject != nullptr;
	_nextSpawn = 0;
	_elapsed = 0;
	_template = std::make_unique<U>(temp);
	_constructSegmentFunction = getSegmentFunction<T, U>();
	_initialised = true;
	return true;
}
/////////////////////////////////////////////////////////////

class UPaperFlipbookComponent;
UCLASS()
class I_LOVE_VAMPIRES_2_API ATrailSegmentCosmetic : public AActor
{
	GENERATED_BODY()

	friend class TrailSegmentCosmeticWrapper;

	float _duration = 0;
	float _elapsed = 0;

	UPROPERTY()
	TObjectPtr<UPaperFlipbookComponent> _flipbook = nullptr;

public:
	ATrailSegmentCosmetic(const FObjectInitializer& init);
	bool initialise_ATrailSegmentCosmetic(const TrailSegmentCosmeticInitStruct& init);
	virtual void Tick(float delta) override;
	virtual void BeginPlay() override;
	float getRemainingDuration() const { return _duration - _elapsed; }
	float getDuration() const { return _duration; }
	UPaperFlipbookComponent* getFlipbookRef() const { return _flipbook; }
};

/////////////////////////////////////////////////////////////
UCLASS()
class I_LOVE_VAMPIRES_2_API UTrailSegmentWrapper : public UObject, public FTickableGameObject {
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<ATrailSegmentFactory> _owner = nullptr;

	FLinearColor _originalColor = FLinearColor::White;
	bool _fadeOut = false;
	float _elapsed = 0;

protected:
	bool postSpawn(const TrailSegmentInitStruct* init);

public:
	bool initialise_UTrailSegmentWrapper(ATrailSegmentFactory* owner) { _owner = owner; return true; }
	virtual void myInit(ATrailSegmentFactory* owner, const TrailSegmentInitStruct* init) { initialise_UTrailSegmentWrapper(owner); }
	virtual void Tick(float DeltaTime) override;
	bool applyFade();
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UMyObject, STATGROUP_Tickables); }
	virtual bool IsTickable() const override { return !IsTemplate(); }

	virtual AActor* getActor() const
		PURE_VIRTUAL(UTrailSegmentWrapper::getActor,
			LOGERROR("getActor not implemented in this class"); return nullptr;);
	virtual UPaperFlipbookComponent* getFlipbookRef() const
		PURE_VIRTUAL(UTrailSegmentWrapper::getFlipbookRef,
			LOGERROR("getFlipbookRef not implemented in this class"); return nullptr;);
	virtual float getRemainingDuration() const
		PURE_VIRTUAL(UTrailSegmentWrapper::getRemainingDuration,
			LOGERROR("getRemainingDuration not implemented in this class"); return -1;);
	virtual float getTotalDuration() const
		PURE_VIRTUAL(UTrailSegmentWrapper::getTotalDuration,
			LOGERROR("getTotalDuration not implemented in this class"); return -1;);
	virtual bool iWantToDie() const
		PURE_VIRTUAL(UTrailSegmentWrapper::iWantToDie,
			LOGERROR("iWantToDie not implemented in this class"); return true;);
	virtual void onDeath() {};
};

UCLASS()
class I_LOVE_VAMPIRES_2_API UTrailSegmentCosmeticWrapper : public UTrailSegmentWrapper {
	GENERATED_BODY()

	TWeakObjectPtr<ATrailSegmentCosmetic> _segment = nullptr;

public:
	bool initialise_UTrailSegmentCosmeticWrapper(ATrailSegmentFactory* owner, const TrailSegmentCosmeticInitStruct& init);
	virtual void myInit(ATrailSegmentFactory* owner, const TrailSegmentInitStruct* init) override {
		const auto casted = dynamic_cast<const TrailSegmentCosmeticInitStruct*>(init);
		if (casted != nullptr)
			initialise_UTrailSegmentCosmeticWrapper(owner, *casted);
	}
	virtual AActor* getActor() const override { return _segment.IsValid() ? _segment.Get() : nullptr; }
	virtual UPaperFlipbookComponent* getFlipbookRef() const override;
	virtual float getRemainingDuration() const override { return _segment.IsValid() ? _segment->getRemainingDuration() : -1; }
	virtual float getTotalDuration() const override { return _segment.IsValid() ? _segment->getDuration() : -1; }
	virtual bool iWantToDie() const override { return _segment.IsValid(); }
};

class AAttackTrail;
UCLASS()
class I_LOVE_VAMPIRES_2_API UTrailSegmentAttackWrapper : public UTrailSegmentWrapper {
	GENERATED_BODY()

	TWeakObjectPtr<AAttackTrail> _segment = nullptr;

public:
	bool initialise_UTrailSegmentAttackWrapper(ATrailSegmentFactory* owner, const TrailSegmentAttackInitStruct& init);
	virtual void myInit(ATrailSegmentFactory* owner, const TrailSegmentInitStruct* init) override {
		const auto casted = dynamic_cast<const TrailSegmentAttackInitStruct*>(init);
		if (casted != nullptr)
			initialise_UTrailSegmentAttackWrapper(owner, *casted);
	}
	virtual AActor* getActor() const override;
	virtual UPaperFlipbookComponent* getFlipbookRef() const override;
	virtual float getRemainingDuration() const override;
	virtual float getTotalDuration() const override;
	virtual bool iWantToDie() const override;
};