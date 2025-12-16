#include <engine/console_buffer.h>
#include <engine/log.h>

ConsoleBuffer::ConsoleBuffer(int width, int height, int grid_size)
    : width(width), height(height), grid_size(grid_size) {
        line_buff.resize(height);
        for (auto& l : line_buff) {
            l.resize(width, ' ');
        }
}

void ConsoleBuffer::addChar(char c) {
    if (char_idx == width) {
        LOG(Error) << "Cannot display more characters on this line";
        return;
    }
    line_buff[active_line][char_idx++] = c;
}

void ConsoleBuffer::nextLine() {
    active_line = (active_line + 1) % height;
    char_idx = 0;
}

std::string ConsoleBuffer::getActiveLine() {
    return std::string(line_buff[active_line].begin(), line_buff[active_line].end());
}

std::vector<std::string> ConsoleBuffer::getAllLines() {
    // make the active line the last line retrieved
    std::vector<std::string> ret;
    ret.resize(height);
    for (int i = 0; i < height; i++) {
        auto offset_i = (i + active_line + 1) % height;
        ret[height - i - 1] = std::string(line_buff[offset_i].begin(), line_buff[offset_i].end());
    }
    return ret;
}