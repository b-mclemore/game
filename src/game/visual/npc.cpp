#include <game/visual/npc.h>

Npc::Npc(InteractionType typ, const std::string& d, int x, int y) {
    opensMinigame = (typ == InteractionType::Chess);
    dialog = d;
    setPos(x, y);
}

// Room for creativity here. Could use the npc class to construct
// "[The (type)] or [Name] [verbs] '[dialog]'"
InteractionResult Npc::interact() const {
    // only one type of minigame right now
    if (opensMinigame) {
        return { InteractionType::Chess, dialog };
    }
    return { InteractionType::Talk, dialog };
}

// Used to check if (player) coords are adjacent to the npc
bool Npc::isAdjacent(int px, int py) {
    auto [cx, cy] = getPos();
    return ((cx == px && (cy-1 == py || cy+1 == py)) ||
        (cy == py && (cx-1 == px || cx+1 == px)));
}