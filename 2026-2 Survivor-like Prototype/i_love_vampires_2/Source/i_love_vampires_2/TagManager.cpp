#include "TagManager.h"
#include "helpers.h"
#include "Definitions.h"

void UTagManager::Tick(float DeltaTime) {
	for (auto it = _tags.begin(); it != _tags.end();) {
		auto tag = it->get();
		if (helpers::nearEq(tag->_duration, INFINITE_DURATION)) {
			continue;
		}
		tag->_duration -= DeltaTime;


		if (tag->_duration <= 0) {
			it = _tags.erase(it);
		}
		else {
			++it;
		}
	}
}