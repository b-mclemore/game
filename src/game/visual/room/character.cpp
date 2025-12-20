#include <game/visual/room/character.h>

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