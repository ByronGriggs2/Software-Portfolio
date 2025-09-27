#include "PlayerWrap.h"

void PlayerWrap::physics(bool abovePlatform) {
	//grounded
	if (abovePlatform && -0.01 < _velocity.y && 0.01 > _velocity.y && _position.y <= _player->getSpawn().y - 5.0f) {}
	//landing
	else if (abovePlatform && _position.y <= _player->getSpawn().y &&
		     _velocity.y < 0 && 
		     _position.y >= _player->getSpawn().y - 10.0f) {
		_airborne = false;
		_player->ground();
		_arrow->ground();
		_position.y = _player->getPos().y;
		_velocity.y = 0;
	}
	//accelerating down
	else if (_position.y >= _player->getSpawn().y - 100.0f) {
		_velocity.y -= 0.0012f * _timeFactor;
	}
	//restart
	else {
		respawn();
	}

	updatePos();
}

void PlayerWrap::jumpFunc() {
	switch (_lookDir) {
	case Direction::d_left:
		_velocity.y += 0.12f;
		_velocity.x = -0.4f;
		break;
	case Direction::d_upLeft:
		_velocity.y += 0.4f;
		_velocity.x = -0.2f;
		break;
	case Direction::d_up:
		_velocity.y += 0.4f;
		break;
	case Direction::d_upRight:
		_velocity.y += 0.4f;
		_velocity.x = 0.2f;
		break;
	case Direction::d_right:
		_velocity.y += 0.12f;
		_velocity.x = 0.4f;
		break;
	case Direction::d_downRight:
		_velocity.y -= 0.12f;
		_velocity.x = 0.4f;
		break;
	case Direction::d_down:
		_velocity.y -= 0.4f;
		break;
	case Direction::d_downLeft:
		_velocity.y -= 0.12f;
		_velocity.x = -0.4f;
	}
}

void PlayerWrap::newAction(Action act) {
	if (-0.01 >= _velocity.y || 0.01 <= _velocity.y) {
		_airborne = true;
	}
	switch (act) {
	case Action::jump:
		if (!_airborne) {
			jumpFunc();
		}
		break;
	case Action::left:
		if (!_airborne) {
			_velocity.x = -0.2f;
		}
		_lookDir = Direction::d_left;
		break;
	case Action::upLeft:
		if (!_airborne) {
			_velocity.x = -0.2f;
		}
		_lookDir = Direction::d_upLeft;
		break;
	case Action::up:
		if (!_airborne) {
			_velocity.x = 0.0f;
		}
		_lookDir = Direction::d_up;
		break;
	case Action::upRight:
		if (!_airborne) {
			_velocity.x = 0.2f;
		}
		_lookDir = Direction::d_upRight;
		break;
	case Action::right:
		if (!_airborne) {
			_velocity.x = 0.2f;
		}
		_lookDir = Direction::d_right;
		break;
	case Action::downRight:
		if (!_airborne) {
			_velocity.x = 0.2f;
		}
		_lookDir = Direction::d_downRight;
		break;
	case Action::down:
		if (!_airborne) {
			_velocity.x = 0.0f;
		}
		_lookDir = Direction::d_down;
		break;
	case Action::downLeft:
		if (!_airborne) {
			_velocity.x = -0.2f;
		}
		_lookDir = Direction::d_downLeft;
		break;
	case Action::idle:
		if (!_airborne) {
			_velocity.x = 0.0f;
		}
		break;
	case Action::reset:
		respawn();
		_timeFactor = 1.0f;
		break;
	case Action::decreaseSpeed:
		if (_timeFactor > 0.21f) {
			_timeFactor -= 0.2f;
		}
		break;
	case Action::increaseSpeed:
		if (_timeFactor < 2.79f) {
			_timeFactor += 0.2f;
		}
		break;
	default:
		break;
	}
}

void PlayerWrap::respawn() {
	_lookDir = Direction::d_up;
	_airborne = false;
	_position = _player->getSpawn();
	_velocity = XMFLOAT3(0, 0, 0);
	_player->respawn();
	_arrow->respawn();
}