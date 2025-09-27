//This class binds together the arrow (look direction) and player objects so that they can be treated as a single entity.

#pragma once
#include <memory>
#include <vector>
#include "DirectXMath.h"
#include "CompleteObject.h"

enum class Action {
	idle,
	jump,
	left,
	upLeft,
	up,
	upRight,
	right,
	downRight,
	down,
	downLeft,

	reset,
	increaseSpeed,
	decreaseSpeed
};

enum class Direction {
	d_left,
	d_upLeft,
	d_up,
	d_upRight,
	d_right,
	d_downRight,
	d_down,
	d_downLeft
};

class PlayerWrap final{
	bool _airborne;
	Direction _lookDir;
	XMFLOAT3 _position;
	XMFLOAT3 _velocity;
	float _timeFactor;
	std::unique_ptr<CompleteObject> _player;
	std::unique_ptr<CompleteObject> _arrow;

	void updatePos() {
		const XMFLOAT3 tempVelocity = XMFLOAT3(_velocity.x * _timeFactor, _velocity.y * _timeFactor, _velocity.z * _timeFactor);
		_player->setVelocity(tempVelocity);
		_arrow->setVelocity(tempVelocity);
		_player->updatePos();
		_arrow->updatePos();
		_position.x += tempVelocity.x; _position.y += tempVelocity.y; _position.z += tempVelocity.z;
	}
	void respawn();

public:
	

	PlayerWrap() : _airborne(false), _lookDir(Direction::d_up), _position(XMFLOAT3(0, 0, 0)), _velocity(XMFLOAT3(0, 0, 0)), _timeFactor(1.0f),
		_player(nullptr), _arrow(nullptr) {}
	PlayerWrap(std::unique_ptr<CompleteObject> pPlayer, std::unique_ptr<CompleteObject> pArrow) : _airborne(false), _lookDir(Direction::d_up),
		_position(XMFLOAT3(0, 0, 0)), _velocity(XMFLOAT3(0, 0, 0)), _timeFactor(1.0f), _player(std::move(pPlayer)),
		_arrow(std::move(pArrow)) {
		_position = _player->getPos();
	}
	void physics(bool);
	void newAction(Action);
	void jumpFunc();
	XMFLOAT3 getPos() const { return _position; }
	XMFLOAT3 getPlayerSpawn() const { return _player->getSpawn(); }
	XMFLOAT3 getArrowSpawn() const { return _arrow->getSpawn(); }
	XMFLOAT3 getArrowPos() const { return _arrow->getPos(); }
	Direction look() const { return _lookDir; }
	CompleteObject& getPlayer() const { return *_player; }
	CompleteObject& getArrow() const { return *_arrow; }
};