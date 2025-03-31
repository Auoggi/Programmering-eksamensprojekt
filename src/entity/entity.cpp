#include "entity.h"

Entity::Entity(const char *texture, const char *textureName) : pos(glm::vec2(100, 100)), speed(200) {
    this->texture = ResourceManager::loadTexture(texture, textureName);
}

void Entity::draw(Renderer *renderer, glm::mat4 view) {
    renderer->drawTexture(this->texture, view, this->pos, glm::vec2(25, 25), 0);
}