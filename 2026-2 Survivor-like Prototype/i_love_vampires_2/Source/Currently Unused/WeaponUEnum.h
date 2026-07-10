#include "CoreMinimal.h"
#include "AttackEnum.h"
#include "AttackEnumFull.generated.h"

UENUM(BlueprintType)
enum class EAoeShape : uint8
{
	circle      UMETA(DisplayName = "circle")
};
UENUM(BlueprintType)
enum class EProjectileShape : uint8
{
	circle      UMETA(DisplayName = "circle")
};
UENUM(BlueprintType)
enum class EAttackShape : uint8
{
	fan      UMETA(DisplayName = "circle")
};
UENUM(BlueprintType)
enum class EProjectileTargeting : uint8
{
	skillshot      UMETA(DisplayName = "skillshot"),
	closest		   UMETA(DisplayName = "closest"),
	random		   UMETA(DisplayName = "random")
};
namespace AOEShape {
	using EMyEnum = ESpriteEnum;
	const std::unordered_map<MyEnum, EMyEnum> CONVERSION_MAP = {
		{testPlayer, EMyEnum::testPlayer},
		{testEnemy, EMyEnum::testEnemy},
		{testBullet, EMyEnum::testBullet},
		{testShard, EMyEnum::testShard}
	};
	class Converter {
	public:
		static EMyEnum toEEnum(const MyEnum& input) {
			auto it = CONVERSION_MAP.find(input);
			if (it == CONVERSION_MAP.end()) {
				LOGERROR("Failed to find UEnum for SpriteEnum in Converter::toUEnum");
				return static_cast<EMyEnum>(0); // Return a default value or handle this case as needed
			}
			return it->second;
		}
		static MyEnum toEnum(const EMyEnum& input) {
			auto it = std::find_if(CONVERSION_MAP.begin(), CONVERSION_MAP.end(), [input](const auto& pair) { return pair.second == input; });
			if (it == CONVERSION_MAP.end()) {
				LOGERROR("Failed to find SpriteEnum for UEnum in Converter::toEnum");
				return static_cast<MyEnum>(0); // Return a default value or handle this case as needed
			}
			return it->first;
		}
	};
}