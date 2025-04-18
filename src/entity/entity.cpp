#include "entity.h"

Entity::Entity(const char *texture, const char *textureName, int width, int height, int health) : pos(glm::vec2(0, 0)), currTilePos(glm::ivec2(0, 0)),
                 speed(300), width(width), height(height), velocity(glm::vec2(0, 0)) {
    this->texture = ResourceManager::loadTexture(texture, textureName);
}

glm::ivec2 Entity::getTileCoords(Grid *grid) {
    return glm::ivec2(floor(this->pos.x / grid->tileSize), floor(this->pos.y / grid->tileSize));
}

void Entity::tick(Grid *grid) {
    this->prevTilePos = this->currTilePos;
    this->currTilePos = getTileCoords(grid);

    // Check if entity has moved
    if(this->prevTilePos != this->currTilePos) {
        // Get pointer to EntityVector at previous tile position from grid->entityMap
        std::shared_ptr<std::vector<Entity*>> tile = grid->getEntityList(this->prevTilePos);
        // tile->size() gets entity count of previous tile
        if(tile->size() > 1) {
            for(int i = 0; i < tile->size(); i++) {
                if(tile->at(i) == this) {
                    tile->erase(tile->begin() + i); // Erase self from previous tile posistion
                    break;
                }
            }
        } else {
            grid->entityMap.erase(this->prevTilePos); // Delete list if this was the last entity or nothing was in it
        }
        
        // Add self to new tile position
        grid->getEntityList(this->currTilePos)->push_back(this);
    }
}

bool Entity::collisionDetection(Entity *entity) {
    // if stateement that controls collision
    if((this->pos.x - this->width/2 < entity->pos.x + entity->width/2 && this->pos.x + this->width/2 > entity->pos.x - entity->width/2) &&
        (this->pos.y - this->height/2 < entity->pos.y + entity->height/2 && this->pos.y + this->height/2 > entity->pos.y - entity->height/2)) {
        return true;
    }
    return false;
}

void Entity::draw(Renderer *renderer, glm::mat4 view) {
    renderer->drawTexture(this->texture, view, glm::vec2(this->pos.x - this->width/2, this->pos.y - this->height/2), glm::vec2(this->width, this->height), 0);
}