#include <engine/console_buffer.h>
#include <engine/log.h>

ConsoleBuffer::ConsoleBuffer(int width, int height, int grid_size)
    : width(width), height(height), grid_size(grid_size) {
        line_buff.resize(height);
        source_buff.resize(height, sourceEnum::playerSource);
        for (auto& l : line_buff) {
            l.resize(width, ' ');
        }
}

// adds char-by-char to current active line.
// should only be called by player input
void ConsoleBuffer::addChar(char c) {
    if (char_idx == width) {
        LOG(Error) << "Cannot display more characters on this line";
        return;
    }
    line_buff[active_line][char_idx++] = c;
}

void ConsoleBuffer::deleteChar() {
    if (char_idx > 0) {
        line_buff[active_line][--char_idx] = ' ';
    }
}

void ConsoleBuffer::addLine(const std::string& s, const sourceEnum& src) {
    for (int i = 0; i < width && i < s.length(); i++) {
        line_buff[active_line][i] = s[i];
    }
    nextLine(src);
}

void ConsoleBuffer::nextLine(sourceEnum previous_source) {
    source_buff[active_line] = previous_source;
    active_line = (active_line + 1) % height;
    char_idx = 0;
    std::fill(line_buff[active_line].begin(), line_buff[active_line].end(), ' ');
}

std::string ConsoleBuffer::getActiveLine() {
    last_player_command = active_line;
    return std::string(line_buff[active_line].begin(), line_buff[active_line].end());
}

std::vector<std::pair<std::string, sourceEnum>> ConsoleBuffer::getAllLines() {
    // make the active line the last line retrieved
    std::vector<std::pair<std::string, sourceEnum>> ret;
    ret.resize(height);
    for (int i = 0; i < height; i++) {
        auto offset_i = (i + active_line + 1) % height;
        auto s = std::string(line_buff[offset_i].begin(), line_buff[offset_i].end());
        auto c = source_buff[offset_i];
        ret[height - i - 1] = {s, c};
    }
    return ret;
}