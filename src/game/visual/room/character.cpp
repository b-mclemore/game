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

void Character::setRenderPos(Vector2 rp) {
	renderPos = rp;
}
Vector2 Character::getRenderPos() {
	return renderPos;
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

void Character::setMoveFrom(Vector2 v) {
	moveFrom = v;
}
Vector2 Character::getMoveFrom() {
	return moveFrom;
}

void Character::setMoveTo(Vector2 v) {
	moveTo = v;
}
Vector2 Character::getMoveTo() {
	return moveTo;
}

void Character::setMoveProgress(float f) {
	moveProgress = f;
}
float Character::getMoveProgress() {
	return moveProgress;
}

void Character::setHalfStepTriggered(bool b) {
	halfStepTriggered = b;
}
bool Character::getHalfStepTriggered() {
	return halfStepTriggered;
}