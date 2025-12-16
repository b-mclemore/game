#ifndef CONSOLE_BUFFER_H
#define CONSOLE_BUFFER_H

#include <vector>
#include <memory>

class ConsoleBuffer {
public:
    ConsoleBuffer(int width, int height, int grid_size);
    void addChar(char c);
    void nextLine(); // just progresses the active_line
    std::string getActiveLine();
    std::vector<std::string> getAllLines();

private:
    int width;
    int height;
    int grid_size;
    int active_line = 0;
    int char_idx = 0;

    std::vector<std::vector<char>> line_buff;
};

#endif
