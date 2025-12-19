#include <fstream>
#include <sstream>
#include <stdexcept>
#include <engine/log.h>
#include <game/visual/room/tilemap.h>

TileMap::TileMap(int width, int height)
    : width(width), height(height) {
    tiles.resize(width * height, TileType::EMPTY);
}

void TileMap::setTile(int x, int y, TileType type) {
    if (isInBounds(x, y)) {
        tiles[y * width + x] = type;
    }
}

TileType TileMap::getTile(int x, int y) const {
    if (isInBounds(x, y)) {
        return tiles[y * width + x];
    }
    return TileType::EMPTY;
}

bool TileMap::isWalkable(int x, int y) const {
    if (!isInBounds(x, y)) {
        return false;
    }
    TileType tile = getTile(x, y);
    return tile == TileType::DIRT;
}

bool TileMap::isInBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Loads a tilemap from a file
void TileMap::loadTileMap(const std::string& file) {
    std::ifstream in(file);
    if (!in)
        throw std::runtime_error("Failed to open map file");

    std::string line;

    // read dims
    if (!std::getline(in, line))
        throw std::runtime_error("Failed to read map size line");

    std::istringstream header(line);
    if (!(header >> width >> height))
        throw std::runtime_error("Invalid map size format");

    tiles.resize(width * height);

    // read rows
    for (int y = 0; y < height; ++y) {
        if (!std::getline(in, line))
            throw std::runtime_error("Unexpected end of file at row " + std::to_string(y));

        std::istringstream row(line);

        for (int x = 0; x < width; ++x) {
            int value;
            if (!(row >> value)) {
                throw std::runtime_error(
                    "Invalid tile value at (" +
                    std::to_string(x) + ", " +
                    std::to_string(y) + ")"
                );
            }
            tiles[y * width + x] = static_cast<TileType>(value);
        }
    }
}