#pragma once
#include "CoreMinimal.h"
#include "EffectStruct.h"
#include "BaseAttributes.h"
#include <utility>
#include <memory>
#include "Definitions.h"
#include "helpers.h"
#include <type_traits>

template<typename attrType>
class BaseAttributeWrapper {
	// Objects of type BaseAttributes are meant to be copied and passed around without strict ownership.
	// However, they still need a world context object for some functionality, so that will be
	// stored here, since this object is strictly owned by the attribute-haver.
	TWeakObjectPtr<UObject> _context = nullptr;
	std::shared_ptr<attrType> _core;
	

public:
	//not automatic
	// Status effects are owned by attributeClass' owner. The owner is responsible for managing duration and deletion of elements, as well as handling
	// statusEffect subclass-specific internal state.
	// It will also be responsible for modifiers? Maybe not, we'll see.

	// Alternatively, instead of passing GlobalStatusState, just call MyGameplayStatics::GetCombatantManager->getBurnThisFrame()
	void tick(float delta, const TArray<FEffectStruct>& statusEffects) {
		if (!_core) {
			LOGERROR("Attempting to tick uninitialized BaseAttributeWrapper");
			return;
		}
		_core->tick(_context.Get(), delta, statusEffects);
	}
	//void tick(float delta, const TArray<FEffectStruct>& statusEffects, const CombatantAttributes* combatantAttributes) {
	//	_core->tick(_context.Get(), delta, statusEffects, combatantAttributes);
	//}
	BaseAttributeWrapper() = delete;
	BaseAttributeWrapper(const BaseAttributeWrapper& other) : _context(other._context), _core(other._core) {}
	BaseAttributeWrapper(BaseAttributeWrapper&& other) : _context(std::move(other._context)), _core(other._core) { other._context = nullptr; other._core = nullptr; }
	BaseAttributeWrapper& operator=(const BaseAttributeWrapper& other) = delete;
	BaseAttributeWrapper& operator=(BaseAttributeWrapper&& other) = delete;
	BaseAttributeWrapper(UObject* context, std::shared_ptr<attrType> core);

	std::shared_ptr<const attrType> getCore() const;
	std::shared_ptr<attrType> getCoreNonconst() { return _core; }
	float getMember(Stat attrType::* member) const;
	float getMemberDiscretized(Stat attrType::* member) const;
	template<typename dataType>
	void changeBaseValues(const dataType* newBases);
};

template <typename attrType>
std::shared_ptr<const attrType> BaseAttributeWrapper<attrType>::getCore() const {
	if (!_core) {
		LOGERROR("Attempting to access uninitialized BaseAttributeWrapper");
		return nullptr;
	}
	return _core;
}
template <typename attrType>
float BaseAttributeWrapper<attrType>::getMember(Stat attrType::* member) const {
	if (!_core) {
		LOGERROR("Attempting to access uninitialized BaseAttributeWrapper");
		return 0;
	}
	return (_core.get()->*member).getFinal();
}
template <typename attrType>
float BaseAttributeWrapper<attrType>::getMemberDiscretized(Stat attrType::* member) const {
	if (!_core) {
		LOGERROR("Attempting to access uninitialized BaseAttributeWrapper");
		return 0;
	}
	Stat temp = _core.get()->*member;
	temp.discretize();
	return temp.getFinal();
}
// Helps somewhat with maintainability... what I really want is to run the override function generically on all members of the template, but that's not possible. This runtime check is better than nothing.
template <typename attrType>
BaseAttributeWrapper<attrType>::BaseAttributeWrapper(UObject* context, std::shared_ptr<attrType> core) : _context(context), _core(core) {
	_core->applyToAllStats([](Stat& stat) {
		if (helpers::isInvalidData(stat.getBase()))
			LOGERROR("Non-transient instance of BaseAttributes (via BaseAttributeWrapper) created with invalid data");
		});
}
template <typename attrType>
template <typename dataType>
void BaseAttributeWrapper<attrType>::changeBaseValues(const dataType* newBases) {
	// I'd like to use the is constructible here but the constructors take extra parameters. I'll add a runtime check below.
	static_assert(std::is_base_of<UBaseAttributeData, dataType>::value, "dataType must be a subclass of UBaseAttributeData");
	if (!_core || !IsValid(newBases)) {
		LOGERROR("Invalid parameter");
		return;
	}
	if (!(_core->isCompatibleWith(newBases))) {
		LOGERROR("Incompatible parameter");
		return;
	}
	std::vector<float> newBaseVals;
	newBases->applyToAllStats([&newBaseVals](const Stat& stat) {
		newBaseVals.push_back(stat.getBase());
		});
	int i = 0;
	_core->applyToAllStats([&newBaseVals, &i](Stat& stat) {
		stat.setBase(newBaseVals[i]);
		i++;
		});
}