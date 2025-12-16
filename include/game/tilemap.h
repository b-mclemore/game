#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>

enum class TileType {
    EMPTY = 0,
    DIRT = 1,
    COBBLE = 2
};

class TileMap {
public:
    TileMap(int width, int height);

    void setTile(int x, int y, TileType type);
    TileType getTile(int x, int y) const;
    bool isWalkable(int x, int y) const;
    bool isInBounds(int x, int y) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width;
    int height;
    std::vector<TileType> tiles;
};

#endif
