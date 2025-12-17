#include <game/textual/text_colors.h>
// old text from player
Color activePlayerColor = Color(128, 128, 32, 255);
Color inactivePlayerColor = Color(64, 64, 32, 255);
// current text from player
Color activeConsoleColor = Color(32, 196, 32, 255);
Color inactiveConsoleColor = Color(32, 96, 32, 255);
// text from textual side (e.g. help message)
Color activeGenericColor = Color(32, 32, 112, 255);
Color inactiveGenericColor = Color(32, 32, 56, 255);
// text from visual side, the npc messages
Color activeNpcColor = Color(112, 32, 56, 255);
Color inactiveNpcColor = Color(64, 32, 64, 255);

Color errorColor = Color(255, 0, 0);

Color getColor(sourceEnum src, bool is_active) {
    switch (src) {
        case sourceEnum::playerSource:
            return (is_active ? activePlayerColor : inactivePlayerColor);
        case sourceEnum::consoleSource:
            return (is_active ? activeConsoleColor : inactiveConsoleColor);
        case sourceEnum::genericSource:
            return (is_active ? activeGenericColor : inactiveGenericColor);
        case sourceEnum::npcSource:
            return (is_active ? activeNpcColor : inactiveNpcColor);
        default:
            return errorColor;
    }
}
