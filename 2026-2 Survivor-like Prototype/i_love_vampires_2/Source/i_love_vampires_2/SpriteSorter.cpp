#include "SpriteSorter.h"
#include "EnemyBase.h"
#include "MyPlayer.h"
#include "AttackActor.h"
#include "Definitions.h"
#include "PaperFlipbookComponent.h"
#include "ExperienceShard.h"
#include "SummonAttack.h"

using namespace USpriteSorterSpace;
using enum USpriteSorterSpace::layerType;

namespace {
	constexpr int _MAX_PRIORITY = -10000;
	constexpr int _PLAYER_PRIORITY = 0;
	constexpr int _COMBATANT_SIZE = 2;
	constexpr int _PLAYER_SIZE = _COMBATANT_SIZE;
	constexpr int _ENEMY_PRIORITY = _PLAYER_PRIORITY - _PLAYER_SIZE;
	// Size is no longer stored here as constexpr. Rather, it's determined by the size of the passed in TArray. If an AActor wants to give too many or too few
	// flipbooks, thats its business.
	template<typename T>
	constexpr layerType LayerMapping = player;
	template<>
	constexpr layerType LayerMapping<AMyPlayer> = player;
	template<>
	constexpr layerType LayerMapping<AEnemyBase> = enemy;
	template<>
	constexpr layerType LayerMapping<ASummon> = enemy;
	template<>
	constexpr layerType LayerMapping<AAttackActor> = attack;
	template<>
	constexpr layerType LayerMapping<AExperienceShard> = attack;
}

template<layerType T>
int InefficientDatabase<T>::getStartPriority() const {
	static_assert(always_false_v<T>, "getStartPriority not implemented for this type");
	return 0;
}
template<layerType T>
void InefficientDatabase<T>::incrementPriority(int& priority, int amount) {
	static_assert(always_false_v<T>, "incrementPriority not implemented for this type");
}
template<layerType T>
bool USpriteSorter::sortSprite_internal(const TArray<UPaperFlipbookComponent*>& flipbooks) {
	static_assert(always_false_v<T>, "sortSprite_internal not implemented for this type");
	return false;
}
///////////////////////////////
// Player layer
template<>
int InefficientDatabase<player>::getStartPriority() const { return _PLAYER_PRIORITY; }
template<>
void InefficientDatabase<player>::incrementPriority(int& priority, int amount) { priority -= amount; }
template<>
bool USpriteSorter::sortSprite_internal<player>(const TArray<UPaperFlipbookComponent*>& flipbooks) {
	_playerData.addSprite(flipbooks);
	return true;
}
///////////////////////////////
// Enemy layer
template<>
int InefficientDatabase<enemy>::getStartPriority() const { return _ENEMY_PRIORITY; }
template<>
void InefficientDatabase<enemy>::incrementPriority(int& priority, int amount) { priority -= amount; }
template<>
bool USpriteSorter::sortSprite_internal<enemy>(const TArray<UPaperFlipbookComponent*>& flipbooks) {
	_enemyData.addSprite(flipbooks);
	return true;
}
////////////////////////////////
// Attack Layer
template<>
int InefficientDatabase<attack>::getStartPriority() const { return _MAX_PRIORITY; }
template<>
void InefficientDatabase<attack>::incrementPriority(int& priority, int amount) { priority += amount; }
template<>
bool USpriteSorter::sortSprite_internal<attack>(const TArray<UPaperFlipbookComponent*>& flipbooks) {
	_attackData.addSprite(flipbooks);
	return true;
}
////////////////////////////////
// May or may not resolve linker errors if the compiler makes them. Right now it isn't so I'm not bothering.
//template class InefficientDatabase<layerType::player>;
//template class InefficientDatabase<layerType::enemy>;
//template class InefficientDatabase<layerType::attack>;

template<layerType T>
bool InefficientDatabase<T>::valid(int index) {
	for (auto& flipWrap : _database[index]) {
		if (!flipWrap.valid()) {
			return false;
		}
	}
	return true;
}

template <layerType T>
bool InefficientDatabase<T>::addSprite(const TArray<UPaperFlipbookComponent*>& flipbooks) {
	int workingPriority = getStartPriority();
	auto placeHere = [&workingPriority, &flipbooks, this](int index) {
		_database[index].clear();
		for (auto& flipbook : flipbooks) {
			_database[index].emplace_back(flipbook);
			flipbook->TranslucencySortPriority = workingPriority;
			incrementPriority(workingPriority);
		}
		};
	for (auto i = 0; i < _database.size(); i++) {
		if (_database[i].size() == 0 || !valid(i)) {
			placeHere(i);
			return true;
		}
		incrementPriority(workingPriority, _database[i].size());
	}
	_database.emplace_back();
	placeHere(_database.size() - 1);
	return true;
}

template<typename T>
bool USpriteSorter::sortSprite(const TArray<UPaperFlipbookComponent*>& flipbooks) {
	for (auto i = 0; i < flipbooks.Num(); i++) {
		if (!IsValid(flipbooks[i])) {
			LOGERROR("USpriteSorter::sortSprite - invalid flipbook passed in");
			return false;
		}
	}
	return sortSprite_internal<LayerMapping<T>>(flipbooks);
}
template bool USpriteSorter::sortSprite<AMyPlayer>(const TArray<UPaperFlipbookComponent*>& flipbooks);
template bool USpriteSorter::sortSprite<AEnemyBase>(const TArray<UPaperFlipbookComponent*>& flipbooks);
template bool USpriteSorter::sortSprite<ASummon>(const TArray<UPaperFlipbookComponent*>& flipbooks);
template bool USpriteSorter::sortSprite<AAttackActor>(const TArray<UPaperFlipbookComponent*>& flipbooks);
template bool USpriteSorter::sortSprite<AExperienceShard>(const TArray<UPaperFlipbookComponent*>& flipbooks);