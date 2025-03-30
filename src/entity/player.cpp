#include "player.h"

Player::Player() : pos(glm::vec2(100, 100)),dash(true), dashSpeed(150), speed(7) {}

void Player::processInput(GLFWwindow *window, std::chrono::duration<float> deltaTime) {
    glm::vec2 direction = glm::vec2(0, 0);

    // If statements that detects user input
    if(glfwGetKey(window, GLFW_KEY_W)) {
        direction += glm::vec2(0, -1);
    }
    if(glfwGetKey(window, GLFW_KEY_S)) {
        direction += glm::vec2(0, 1);
    }
    if(glfwGetKey(window, GLFW_KEY_D)) {
        direction += glm::vec2(1, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_A)){
        direction += glm::vec2(-1, 0);
    }

    // Dashing feature
    if(glfwGetKey(window, GLFW_KEY_SPACE) && this->dash) {
        if(glm::length(direction) != 0) {
            this->pos += glm::normalize(direction) * this->dashSpeed;
            this->dash = false;
        } 
    }

    // If statement that avoids zero division 
    if(glm::length(direction) != 0) {
        this->pos += glm::normalize(direction) * this->speed;
    }
}

glm::mat4 Player::getView(int width, int height) {
    return glm::translate(glm::mat4(1.f), glm::vec3(-this->pos.x + width/2, -this->pos.y + height/2, 0));
}

void Player::draw(Renderer *renderer, glm::mat4 view) {
    renderer->drawTexture(this->texture, view, this->pos, glm::vec2(25, 25), 0);
}