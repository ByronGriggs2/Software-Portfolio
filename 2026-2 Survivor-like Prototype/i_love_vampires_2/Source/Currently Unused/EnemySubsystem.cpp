#include "EnemySubsystem.h"
#include "GameFramework/Actor.h"
#include <cstdlib>
#include <time.h>

UEnemySubsystem::UEnemySubsystem() {
	srand(time(NULL));
}

int UEnemySubsystem::registerEnemy(const AActor* enemy) {
	MyStruct newStruct;
	newStruct.key = _nextKey;
	_nextKey += 1;
	newStruct.value = TWeakObjectPtr<const AActor>(enemy);
	_enemyReferences.push_back(newStruct);
	return newStruct.key;
}

void UEnemySubsystem::removeFromRegister(int key) {
	for (std::vector<MyStruct>::iterator it = _enemyReferences.begin(); it != _enemyReferences.end();)
	{
		if ((*it).key == key) {
			_enemyReferences.erase(it);
			return;
		}
		else
			++it;
	}
}

TWeakObjectPtr<const AActor> UEnemySubsystem::getRandomEnemyPtr() {
	if (_enemyReferences.empty()) {
		TWeakObjectPtr<const AActor> ret = nullptr;
		return ret;
	}
	int roll = rand() % _enemyReferences.size();
	return _enemyReferences[roll].value;
}