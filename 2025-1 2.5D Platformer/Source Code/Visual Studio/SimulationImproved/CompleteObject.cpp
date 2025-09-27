#include "CompleteObject.h"

void CompleteObject::updatePos() {
	_position.x += _velocity.x;
	_position.y += _velocity.y;
	_position.z += _velocity.z;
}

void CompleteObject::posInit() {
	if (_vertices.size() == 0)
		return;
	for (auto& vertex : _vertices) {
		vertex.Pos.x += _position.x; vertex.Pos.y += _position.y; vertex.Pos.z += _position.z;
	}
}