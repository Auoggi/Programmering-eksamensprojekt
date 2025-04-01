#include "entity.h"

Entity::Entity(const char *texture, const char *textureName, int W, int H) : pos(glm::vec2(100, 100)), speed(200), width(W), height(H){
    this->texture = ResourceManager::loadTexture(texture, textureName);
}

void Entity::draw(Renderer *renderer, glm::mat4 view) {
    renderer->drawTexture(this->texture, view, glm::vec2(this->pos.x - this->width/2, this->pos.y - this->height/2), glm::vec2(this->width, this->height), 0);
}