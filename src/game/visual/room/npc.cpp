#include <game/visual/room/npc.h>

Npc::Npc(InteractionType typ, const std::string& d, int x, int y) {
    opensMinigame = (typ == InteractionType::Chess);
    dialog = d;
    setPos(x, y);
}

// Room for creativity here. Could use the npc class to construct
// "[The (type)] or [Name] [verbs] '[dialog]'"
InteractionResult Npc::interact(Facing f) {
    // point towards player
    setDir(oppositeDir(f));
    // only one type of minigame right now
    if (opensMinigame) {
        return { InteractionType::Chess, dialog };
    }
    return { InteractionType::Talk, dialog };
}

// Used to check if (player) coords are adjacent to the npc
// and facing them
bool Npc::isAdjacentAndFacing(int px, int py, Facing f) {
    auto [cx, cy] = getPos();
    return (
        (cx == px-1 && cy == py && f == Facing::W) ||
        (cx == px+1 && cy == py && f == Facing::E) ||
        (cx == px && cy == py-1 && f == Facing::S) ||
        (cx == px && cy == py+1 && f == Facing::N));
}