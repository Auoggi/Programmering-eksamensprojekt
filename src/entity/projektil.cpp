#include "projektil.h"

Projektil::Projektil() : Entity("assets/textures/ball.png", "player", 10, 10, 0) {}

void Projektil::tick(double deltaTime, Grid *grid) {


    // Call parrent tick function
    Entity::tick(grid, deltaTime);
}

void Projektil::onCollision(Entity *otherEnity) {

}