#include <game/visual/room/character.h>

Facing oppositeDir(Facing f) {
	switch (f) {
		case Facing::N: return Facing::S;
		case Facing::S: return Facing::N;
		case Facing::E: return Facing::W;
		default: return Facing::E;
	}
}

Facing Character::getDir() {
	return dir;
}

void Character::setDir(Facing d) {
	dir = d;
}

void Character::setPos(int x, int y) {
	lastX = x;
	lastY = y;
}

std::pair<int, int> Character::getPos() {
	return {lastX, lastY};
}

void Character::setAnimFrame(AnimationFrame af) {
	animFrameColumn = af;
}

AnimationFrame Character::getAnimFrame() {
	return animFrameColumn;
}

void Character::setIsMoving(bool moving) {
	isMoving = moving;
}

bool Character::getIsMoving() {
	return isMoving;
}