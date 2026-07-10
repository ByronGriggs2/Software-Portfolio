#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include "StatusEffect.h"
#include "CombatantAttributeSetEnum.h"
#include "AttributeModifier.h"

class MyCombatantAttributeSet {
	const static CombatantAttribute::MyEnum _CURRENT_HP = CombatantAttribute::MyEnum::currentHP;

public:
	struct MyAttribute {
		const float _baseValue = 0;
		float _prebonus = 0;
		float _premultiplier = 0;
		float _postbonus = 0;
		MyAttribute(float a, float b, float c, float d) : _baseValue(a), _prebonus(b), _premultiplier(c), _postbonus(d) {}
		MyAttribute() = delete;
	};
private:
	std::unordered_map<CombatantAttribute::MyEnum, MyAttribute> _attributes;
	std::vector<std::unique_ptr<StatusEffect>> _activeStatusEffects;
	std::function<void(CombatantAttribute::MyEnum, float, float)> _onAttributeChangedCallback;
	std::unique_ptr<bool> _burnThisFrame = std::make_unique<bool>(false);

public:
	MyCombatantAttributeSet();
	bool getAttributeValue(CombatantAttribute::MyEnum attributeName, float&) const;
	// Not automatic
	void tick(float deltaTime);
	void inflictStatus(std::unique_ptr<StatusEffect> statusEffect);
	void dealDamage(float damage);
	bool getCurrentAttributes(std::unordered_map<CombatantAttribute::MyEnum, float>& ret) const;
	bool setBaseAttributes(const std::unordered_map<CombatantAttribute::MyEnum, float>& newAttributes);
	void setOnAttributeChangedCallback(std::function<void(CombatantAttribute::MyEnum, float, float)> callback) { _onAttributeChangedCallback = std::move(callback);}
	void clearCallback() { _onAttributeChangedCallback = nullptr; }
	void burnTick();

private:
	void addModifiers(const std::unordered_map<CombatantAttribute::MyEnum, AttributeModifier>& modifiers);
};