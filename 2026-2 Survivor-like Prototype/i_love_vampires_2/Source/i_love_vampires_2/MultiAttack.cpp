#include "MultiAttack.h"
#include "Definitions.h"
#include "helpers.h"
#include "Active.h"

void UMultiAttackTemplate::replaceOverrides() {
	if (helpers::isInvalidData(_delayBetweenAttacks)) {
		_delayBetweenAttacks = _defaults._delayBetweenAttacks;
	}
	for (auto& attack : _attacks) {
		if (IsValid(attack)) {
			attack->replaceOverrides();
		}
		else {
			LOGERROR("MultiAttackTemplate has null attack in _attacks array");
			continue;
		}
	}
}

void UMultiAttackTemplate::dynamicDeepCopy(UObject* context) {
	for (auto& attack : _attacks) {
		if (IsValid(attack)) {
			attack->dynamicDeepCopy(context);
		}
		else {
			LOGERROR("MultiAttackTemplate has null attack in _attacks array");
			continue;
		}
	}
}

MultiAttackFactory::MultiAttackFactory(MultiAttackFactory&& other) : BaseAttackFactory(std::move(other)) {
	_factories = std::move(other._factories);
	_delayBetweenAttacks = other._delayBetweenAttacks;
}

MultiAttackFactory::MultiAttackFactory(ACombatant* owner, const UMultiAttackTemplate* temp) : BaseAttackFactory(owner) {
	if (!IsValid(temp)) {
		LOGERROR("MultiAttackFactory::MultiAttackFactory - received null template");
		return;
	}
	_delayBetweenAttacks = temp->_delayBetweenAttacks;
	int levels = -1;
	if (temp->_attacks.Num() == 0) {
		LOGERROR("MultiAttackFactory::MultiAttackFactory - empty template");
		return;
	}
	for (auto& attack : temp->_attacks) {
		if (!IsValid(attack)) {
			LOGERROR("MultiAttackFactory::MultiAttackFactory - MultiAttackTemplate has null attack in _attacks array");
			return;
		}
		_factories.push_back(attack->createFactory(owner));
		if (levels == -1) {
			levels = _factories.back()->getMaxLevel();
		}
		else if (levels != _factories.back()->getMaxLevel()) {
			LOGERROR("MultiAttackFactory::MultiAttackFactory = MultiAttackTemplate has attacks with different max levels");
			return;
		}
	}
}

void MultiAttackFactory::launchAttack(const FVector& forward) {
	if (_isFiring)
		return;
	_isFiring = true;
	_lastForward = forward;
}

void MultiAttackFactory::tick(float delta) {
	BaseAttackFactory::tick(delta);
	for (auto& factory : _factories) {
		factory->tick(delta);
	}
	if (_isFiring) {
		if (_currentAttackIndex >= _factories.size()) {
			_isFiring = false;
			_currentAttackIndex = 0;
			return;
		}
		if (_timeSinceLastAttack >= _delayBetweenAttacks) {
			_factories[_currentAttackIndex]->launchAttack(_lastForward);
			_currentAttackIndex++;
			_timeSinceLastAttack = 0;
		}
		_timeSinceLastAttack += delta;
	}
}

int MultiAttackFactory::getMaxLevel() const {
	return _factories[0]->getMaxLevel();
}

void MultiAttackFactory::setLevel(int newLevel) {
	for (auto& factory : _factories) {
		factory->setLevel(newLevel);
	}
}

bool MultiAttackFactory::canFire() const {
	if (_isFiring) {
		return false;
	}
	for (auto& factory : _factories) {
		if (!factory->canFire()) {
			return false;
		}
	}
	return true;
}