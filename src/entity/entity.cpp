#include "entity.h"

Entity::Entity(const char *texture, const char *textureName, int width, int height) : pos(glm::vec2(0, 0)), currTilePos(glm::ivec2(0, 0)), speed(200), width(width), height(height){
    this->texture = ResourceManager::loadTexture(texture, textureName);
}

glm::ivec2 Entity::getTileCoords(int tileSize) {
    return glm::ivec2(floor(this->pos.x / tileSize), floor(this->pos.y / tileSize));
}

void Entity::tick(Grid *grid) {
    this->prevTilePos = this->currTilePos;
    this->currTilePos = getTileCoords(grid->tileSize);

    // Check if entity has moved
    if(this->prevTilePos != this->currTilePos) {
        // Get pointer to EntityVector at previous tile position from grid->entityMap
        std::vector<Entity*> *tile = grid->entityMap->at(this->prevTilePos); // at() functions as [] would but on pointers
        for(int i = 0; i < tile->size(); i++) {
            if(tile->at(i) == this) {
                tile->erase(tile->begin() + i); // Erase self from previous tile posistion
                break;
            }
        }
        
        // Add self to new tile position
        grid->entityMap->at(this->currTilePos)->push_back(this);
    }


}

void Entity::draw(Renderer *renderer, glm::mat4 view) {
    renderer->drawTexture(this->texture, view, glm::vec2(this->pos.x - this->width/2, this->pos.y - this->height/2), glm::vec2(this->width, this->height), 0);
}