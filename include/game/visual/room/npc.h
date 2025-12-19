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
    InteractionResult interact() const;
    bool isAdjacent(int x, int y);

private:
    std::string dialog;
    bool opensMinigame = false;
};

#endif