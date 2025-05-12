#include "projektil.h"

Projektil::Projektil(glm::vec2 startPos, glm::vec2 targetPos) : Entity("assets/textures/ball.png", "player", 10, 10, 0) {
    this->speed = 200;
    this->pos = startPos;
    this->velocity = glm::normalize(targetPos - startPos) * this->speed;
}

void Projektil::tick(Grid *grid, double deltaTime) {

    // Call parrent tick function
    Entity::tick(grid, deltaTime);
}

void Projektil::onCollision(Entity *otherEntity) {
    if(std::strcmp(otherEntity->getType(), "player") != 0);
}