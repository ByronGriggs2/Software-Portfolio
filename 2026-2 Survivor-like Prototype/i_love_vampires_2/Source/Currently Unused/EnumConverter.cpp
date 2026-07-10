#include "EnumConverter.h"
#include "AOEEnum.h"
#include "AOEUEnum.h"
#include "BulletEnum.h"
#include "BulletUEnum.h"
#include "CombatantAttributeSetEnum.h"
#include "CombatantAttributeSetUEnum.h"
#include "CombatantEnum.h"
#include "CombatantUEnum.h"
#include "SpriteManagerEnum.h"
#include "SpriteManagerUEnum.h"
//#include "WeaponEnum.h"
//#include "WeaponUEnum.h"

////
template<>
const std::unordered_map<AOEShape::MyEnum, EAOEShape> EnumConverter<AOEShape::MyEnum, EAOEShape>::map = []() {
	using enum AOEShape::MyEnum;
	using EMyEnum = EAOEShape;
	return std::unordered_map<AOEShape::MyEnum, EAOEShape>{
		{circle, EMyEnum::circle}
	};
}();
////
template<>
const std::unordered_map<ProjectileShape::MyEnum, EProjectileShape> EnumConverter<ProjectileShape::MyEnum, EProjectileShape>::map = []() {
	using enum ProjectileShape::MyEnum;
	using EMyEnum = EProjectileShape;
	return std::unordered_map<ProjectileShape::MyEnum, EProjectileShape>{
		{circle, EMyEnum::circle}
	};
}();
template<>
const std::unordered_map<AttackShape::MyEnum, EAttackShape> EnumConverter<AttackShape::MyEnum, EAttackShape>::map = []() {
	using enum AttackShape::MyEnum;
	using EMyEnum = EAttackShape;
	return std::unordered_map<AttackShape::MyEnum, EAttackShape>{
		{fan, EMyEnum::fan}
	};
}();
template<>
const std::unordered_map<ProjectileTargeting::MyEnum, EProjectileTargeting> EnumConverter<ProjectileTargeting::MyEnum, EProjectileTargeting>::map = []() {
	using enum ProjectileTargeting::MyEnum;
	using EMyEnum = EProjectileTargeting;
	return std::unordered_map<ProjectileTargeting::MyEnum, EProjectileTargeting>{
		{skillshot, EMyEnum::skillshot},
		{ closest, EMyEnum::closest },
		{ random, EMyEnum::random }
	};
}();
////
template<>
const std::unordered_map<CombatantAttribute::MyEnum, ECombatantAttribute> EnumConverter<CombatantAttribute::MyEnum, ECombatantAttribute>::map = []() {
	using enum CombatantAttribute::MyEnum;
	using EMyEnum = ECombatantAttribute;
	return std::unordered_map<CombatantAttribute::MyEnum, ECombatantAttribute>{
		{maxHP, EMyEnum::maxHP},
		{ currentHP, EMyEnum::currentHP },
		{ damageReduction_flat, EMyEnum::damageReduction_flat },
		{ damageReduction_percent, EMyEnum::damageReduction_percent },
		{ healthRegen_flat, EMyEnum::healthRegen_flat },
		{ healthRegen_percent, EMyEnum::healthRegen_percent },
		{ critChance, EMyEnum::critChance },
		{ critMultiplier, EMyEnum::critMultiplier },
		{ attackSpeed, EMyEnum::attackSpeed },
		{ bonusBounces, EMyEnum::bonusBounces },
		{ bonusPierce, EMyEnum::bonusPierce },
		{ bonusProjectiles, EMyEnum::bonusProjectiles },
		{ projectileSpeed, EMyEnum::projectileSpeed },
		{ projectileSize, EMyEnum::projectileSize },
		{ movementSpeed, EMyEnum::movementSpeed },
		{ range, EMyEnum::range },
		{ contactDamage, EMyEnum::contactDamage },
		{ selfSize, EMyEnum::selfSize },
		{ iFrameDuration, EMyEnum::iFrameDuration }
	};
}();
////
template<>
const std::unordered_map<Status::MyEnum, EStatus> EnumConverter<Status::MyEnum, EStatus>::map = []() {
	using enum Status::MyEnum;
	using EMyEnum = EStatus;
	return std::unordered_map<Status::MyEnum, EStatus>{
		{damage, EMyEnum::damage},
		{ bleed, EMyEnum::bleed },
		{ burn, EMyEnum::burn }
	};
}();
////
template<>
const std::unordered_map<Sprite::MyEnum, ESprite> EnumConverter<Sprite::MyEnum, ESprite>::map = []() {
	using enum Sprite::MyEnum;
	using EMyEnum = ESprite;
	return std::unordered_map<Sprite::MyEnum, ESprite>{
		{testPlayer, EMyEnum::testPlayer},
		{ testEnemy, EMyEnum::testEnemy },
		{ testBullet, EMyEnum::testBullet },
		{ testShard, EMyEnum::testShard }
	};
}();
////
//template<>
//const std::unordered_map<WeaponAttribute::MyEnum, EWeaponAttribute> EnumConverter<WeaponAttribute::MyEnum, EWeaponAttribute>::map = []() {
//	using enum WeaponAttribute::MyEnum;
//	using EMyEnum = EWeaponAttribute;
//	return std::unordered_map<Sprite::MyEnum, ESprite>{
//		{ warmup, EMyEnum::warmup},
//		{ damage, EMyEnum::damage },
//		{ critChance, EMyEnum::critChance },
//		{ critMultiplier, EMyEnum::critMultiplier }
//	};
//	}();