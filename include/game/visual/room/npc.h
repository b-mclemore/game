#ifndef NPC_H
#define NPC_H

#include <engine/gamestate.h>
#include <game/game.h>
#include <game/visual/room/character.h>

enum class InteractionType {
    Talk,
    Chess,
};

struct InteractionResult {
    InteractionType type;
    std::string dialog;
};

class Npc : public Character {
public:
    Npc(InteractionType typ, const std::string& dialog, int x, int y);
    // Called when the player interacts with the npc. Returns an interaction type
    // to convey the result of the interaction - simple conversation, start game, etc.
    // Takes the direction of the player so that the npc can change which direction
    // it faces
    InteractionResult interact(Facing f);
    bool isAdjacent(int x, int y);

private:
    std::string dialog;
    bool opensMinigame = false;
};

#endif