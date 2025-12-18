#include <game/visual/player.h>

PlayerFacing Player::getDir() {
	return dir;
}

void Player::setDir(PlayerFacing d) {
	dir = d;
}

void Player::setPos(int x, int y) {
	lastX = x;
	lastY = y;
}

std::pair<int, int> Player::getPos() {
	return {lastX, lastY};
}

void Player::setAnimFrame(AnimationFrame af) {
	animFrameColumn = af;
}

AnimationFrame Player::getAnimFrame() {
	return animFrameColumn;
}