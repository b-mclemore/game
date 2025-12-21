#ifndef CHARACTER_H
#define CHARACTER_H

#include <engine/gamestate.h>
#include <game/game.h>

// Which direction the Character faces
enum Facing {
    N = 2,
    W = 1,
    E = 0,
    S = 3
};

Facing oppositeDir(Facing f);

// which frame of animation
// matches the column in the sprite atlas for the Character
enum AnimationFrame {
	swingLeft = 0,
	noSwing = 1,
	swingRight = 2
};

class Character {
public:
    Character() = default;

	Facing getDir();
	void setDir(Facing d);

	void setPos(int x, int y);
	std::pair<int, int> getPos();

	void setRenderPos(Vector2 pos);
	Vector2 getRenderPos();

	void setAnimFrame(AnimationFrame af);
	AnimationFrame getAnimFrame();

	void setIsMoving(bool moving);
	bool getIsMoving();

	void setMoveFrom(Vector2 v);
	Vector2 getMoveFrom();

	void setMoveTo(Vector2 v);
	Vector2 getMoveTo();

	void setMoveProgress(float f);
	float getMoveProgress();

	void setHalfStepTriggered(bool b);
	bool getHalfStepTriggered();

private:
	// Movement state tracking
    Facing dir = Facing::S;
	bool isMoving = false;
	int lastX = 0;
	int lastY = 0;
	Vector2 renderPos;
	Vector2 moveFrom;
	Vector2 moveTo;
	float moveProgress; // [0, 1]
	Facing moveDir;
	bool halfStepTriggered;

	// Animation state tracking
	AnimationFrame animFrameColumn = AnimationFrame::noSwing;
};

#endif