#include "NestedAOE.h"

NestedAOEFactory::NestedAOEFactory(ACombatant* owner, const UAOETemplate* primaryTemplate, const TArray<const UAOETemplate*> childTemplates) : AOEFactory(owner, primaryTemplate) {
	for (const auto& childTemplate : childTemplates) {
		_childFactories.push_back(std::make_unique<AOEFactory>(owner, childTemplate));
	}
}

NestedAOEFactory::NestedAOEFactory(NestedAOEFactory&& other) :
	AOEFactory(std::move(other)),
	_childFactories(std::move(other._childFactories))
{}