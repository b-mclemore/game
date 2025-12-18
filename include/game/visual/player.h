#ifndef PLAYER_H
#define PLAYER_H

#include <engine/gamestate.h>
#include <game/game.h>

// Which direction the player faces
enum PlayerFacing {
    N = 0, // default
    W = 1,
    E = 2,
    S = 3
};

// which frame of animation
// matches the column in the sprite atlas for the player
enum AnimationFrame {
	swingLeft = 0,
	noSwing = 1,
	swingRight = 2
};

class Player {
public:
    Player() = default;

	PlayerFacing getDir();
	void setDir(PlayerFacing d);

	void setPos(int x, int y);
	std::pair<int, int> getPos();

	void setAnimFrame(AnimationFrame af);
	AnimationFrame getAnimFrame();

private:
	// Movement state tracking
    PlayerFacing dir = PlayerFacing::S;
	bool isMoving = false;
	int lastX = 0;
	int lastY = 0;

	// Animation state tracking
	AnimationFrame animFrameColumn = AnimationFrame::noSwing;
};

#endif