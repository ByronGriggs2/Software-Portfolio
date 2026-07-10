#pragma once
#include <functional>
#include "EffectStruct.h"
#include <vector>
#include "CoreMinimal.h"
class Stat;
class CombatantAttributes;
class UBaseAttributeData;

#define BASEATTRIBUTES_OVERRIDE(member, myDefault) \
	if ((this->*member) == SENTINEL_FLOAT) \
		(this->*member) = myDefault;
#define BASEATTRIBUTES_DECLARE(name) \
	Stat name = SENTINEL_FLOAT;
#define BASEATTRIBUTES_APPLY(name) \
	func(name);

class BaseAttributes {
	std::vector<Stat*> _zeroedAttributes;
protected:
	//to be called by inherited constructors
	void baseInit() {}
	void baseInit(const UBaseAttributeData* data);
	void baseInit(const BaseAttributes& other);
	void baseInit(BaseAttributes&& other);

	void softReset();
	void tick_internal(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects);
	// For things like capping current HP to max
	virtual void postTick(float delta) {}

	// Guarantees that the stat is set to 0 at the end of calculations. Useful for instant death effects
	void zeroStat(Stat* stat) { _zeroedAttributes.push_back(stat); }

public:
	BaseAttributes() = default;
	virtual ~BaseAttributes() = default;
	BaseAttributes(const UBaseAttributeData* data) : BaseAttributes() {}
	BaseAttributes(const BaseAttributes& other) = default;
	BaseAttributes(BaseAttributes&& other) = default;
	BaseAttributes& operator=(const BaseAttributes& other) = delete;
	BaseAttributes& operator=(BaseAttributes&& other) = delete;
	virtual void discretizeFull() = 0;
	virtual void applyStatus(UObject* context, const FEffectStruct& status, float delta) = 0;
	
	virtual void tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects);
	virtual void applyToAllStats(const std::function<void(const Stat&)>& func) const = 0;
	virtual void applyToAllStats(const std::function<void(Stat&)>& func) = 0;
	virtual bool isCompatibleWith(const UBaseAttributeData* data) const = 0;
};

class StatAttourney;
class Stat {
	float _final;
	float _base;
	bool _softReset = false;
public:
	friend class StatAttourney;
	void discretize();
	float _prebonus;
	float _postbonus;
	float _multiplier;
	float _offset;
	float _modifier;
	Stat() = delete;
	Stat(const Stat& other) = default;
	Stat(Stat&& other) = default;
	Stat& operator=(const Stat& other) = default;
	Stat& operator=(Stat&& other) = default;
	Stat(float base) : _final(base), _base(base), _prebonus(0), _postbonus(0), _multiplier(0), _offset(0) {}
	static float calculateFinal(Stat& stat) {
		// stat._modifier is stat._base that has been passed through an arbitrary function depending on the attribute set and its modifier set.
		stat._final = (stat._modifier + stat._prebonus) * (1 + stat._multiplier) + stat._postbonus + stat._offset;
		return stat._final;
	}
	float getFinal() const { return _final; }
	float getBase() const { return _base; }
	void setBase(float newBase) { _base = newBase; }
	static void softReset(Stat& stat);
	void modify(float newVal);
	void zeroStat() { _final = 0; }
};

// This is for weird factories that need to directly modify stats before setting loose an attack with them.
class SuplexFactory;
class StatAttourney {
	static void setFinal(Stat& stat, float newFinal){stat._final = newFinal;}
public:
	friend class SuplexFactory;
};