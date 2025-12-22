#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <string>

enum class TileType {
    EMPTY = 0,
    DIRT,
    COBBLE,
    TREE
};

bool passableTile(TileType tile);

class TileMap {
public:
    TileMap(int width, int height);

    void setTile(int x, int y, TileType type);
    TileType getTile(int x, int y) const;

    bool isWalkable(int x, int y) const;
    void setWalkability(int x, int y, bool isWalkable);
    bool isInBounds(int x, int y) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void loadTileMap(const std::string& file);

private:
    int width;
    int height;
    std::vector<TileType> tiles;
    std::vector<bool> walkable;
};

#endif
