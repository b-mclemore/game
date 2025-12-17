#ifndef CONSOLE_BUFFER_H
#define CONSOLE_BUFFER_H

#include <vector>
#include <memory>

enum sourceEnum {
    genericSource, // text from the Textual side (e.g. help)
    playerSource,  // old text from the player, previously typed
    npcSource,     // text from the Visual game
    consoleSource, // the text currently being typed
};

class ConsoleBuffer {
public:
    ConsoleBuffer(int width, int height, int grid_size);
    // add/del char, should be used only by player
    void addChar(char c);
    void deleteChar();

    // progresses the active line and clears the new current line
    // sets the source type of the previous active line
    void nextLine(sourceEnum previous_source);
    
    // pulls out the string that's currently active
    std::string getActiveLine();

    // get all of the lines and their sources
    std::vector<std::pair<std::string, sourceEnum>> getAllLines();

    // add an entire line, should be used by the system
    void addLine(const std::string& s, const sourceEnum& src);

private:
    int width;
    int height;
    int grid_size;
    int active_line = 0;
    int char_idx = 0;
    int last_player_command = 0;

    std::vector<std::vector<char>> line_buff;
    // keeps track of where lines come from, e.g. system output vs user input
    std::vector<sourceEnum> source_buff;
};

#endif
