#include "tilemap.h"

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
