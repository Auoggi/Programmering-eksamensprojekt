#include "player.h"

Player::Player() : pos(glm::vec2(100, 100)), speed(200), dash(true), dashSpeed(5000), dashCooldown(5.0f), dashTime(0.0f) {}

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

    // Dashing feature and player movement
    if(glm::length(direction) != 0)
        if(glfwGetKey(window, GLFW_KEY_SPACE) && this->dash) {
            this->pos += glm::normalize(direction) * this->dashSpeed * deltaTime.count();
            this->dash = false;
            this->dashTime = 0.0f;
        }
        else { 
            this->pos += glm::normalize(direction) * this->speed * deltaTime.count();
        }

    // Dash cooldown management
    if(!this->dash) {
        this->dashTime += deltaTime.count();

        if(this->dashCooldown <= this->dashTime) {
            this->dash = true;
        }
    }
}
