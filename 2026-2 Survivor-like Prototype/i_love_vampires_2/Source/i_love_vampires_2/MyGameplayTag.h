#pragma once

struct MyGameplayTag {
	float _duration = 999;

	MyGameplayTag(float duration) : _duration(duration) {}
	virtual ~MyGameplayTag() = default;
};