#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <type_traits>
#include <vector>

#include "EnemyBase.h"
#include "Summon.h"
#include "ExperienceShard.h"
#include "MyPlayer.h"
#include "AttackActor.h"
#include "PaperFlipbookComponent.h"

#include "SpriteSorter.generated.h"

class UPaperFlipbookComponent;

// I (kind of) finally figured out how to put template function implementations in the .cpp file. Not gonna go back and change all my existing stuff tho.
// Basically sometimes the linker will complain about a function and sometimes it won't. 
// If it does, and it's a templated function (not a templated class), just put "template <function signature>" after the implementation and specialization
// in the .cpp file.
// If it is a templated class, you might be able to fix it with "template class <class signature>". Not sure because the compiler isn't complaining
// about this particular file.
// Obviously because my only evidence is "it works right now" it might not work in all cases.

namespace USpriteSorterSpace {
	enum class layerType {
		player,
		enemy,
		attack
	};
}

// An efficient version would be RAM
template<USpriteSorterSpace::layerType T>
class InefficientDatabase {
	// allows UCLASS to be stored in a std::vector, as TArray does not permit nesting. For some reason.
	struct flipWrap {
		TWeakObjectPtr<UPaperFlipbookComponent> _flipbook;
		flipWrap(UPaperFlipbookComponent* flipbook) : _flipbook(flipbook) {}
		bool valid() const { return _flipbook.IsValid(); }
	};

	std::vector<std::vector<flipWrap>> _database;
	int getStartPriority() const;
	void incrementPriority(int& priority, int amount = 1);
	bool valid(int index);

public:
	bool addSprite(const TArray<UPaperFlipbookComponent*>& flipbooks);
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API USpriteSorter : public UObject
{
	GENERATED_BODY()

	InefficientDatabase<USpriteSorterSpace::layerType::player> _playerData;
	InefficientDatabase<USpriteSorterSpace::layerType::enemy> _enemyData;
	InefficientDatabase<USpriteSorterSpace::layerType::attack> _attackData;

	template<USpriteSorterSpace::layerType T>
	bool sortSprite_internal(const TArray<UPaperFlipbookComponent*>& flipbooks);

public:
	template<typename T>
	bool sortSprite(const TArray<UPaperFlipbookComponent*>& flipbooks);
};