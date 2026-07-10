#include "AOESpawner_line.h"
#include "AOE.h"
#include "Definitions.h"
#include "unrealHelpers.h"
#include "MyPlayer.h"

AOESpawner_line::AOESpawner_line(AActor* owner, const AOEInitStruct& spawnTemplate) : AOESpawner(owner, spawnTemplate) {
	if (!_owner.IsValid()) {
		LOGERROR("AOESpawner_line::AOESpawner_line - owner invalid on construction");
		return;
	}
	APawn* player = UGameplayStatics::GetPlayerPawn(_owner.Get(), 0);
	AMyPlayer* castedPlayer = Cast<AMyPlayer>(player);
	if (!IsValid(castedPlayer))
		return;
	const float length = spawnTemplate._AOEAttributes._range.getFinal();
	FVector start;
	unrealHelpers::makeRandomLineSegment(castedPlayer, length, start, _direction);
	_owner->SetActorLocation(start, false, nullptr, ETeleportType::TeleportPhysics);
}

FVector AOESpawner_line::getAttackLocation() {
	if (!_owner.IsValid())
		return FVector(1, 0, 0);
	FVector start = _owner->GetActorLocation();
	const int currentIndex = getTotalAttacks() - getRemainingAttacks();
	const float length = getSpawnTemplate()._AOEAttributes._range.getFinal();
	return start + _direction * (length * (currentIndex / static_cast<float>(getTotalAttacks() - 1)));
}