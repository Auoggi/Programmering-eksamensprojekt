#include "grid.h"

Grid::Grid(int tileSize) : tileSize(tileSize) {}

glm::ivec2 Grid::getTileCoords(Entity entity) {
    return glm::ivec2(floor(entity.pos.x / this->tileSize), floor(entity.pos.y / this->tileSize));
}