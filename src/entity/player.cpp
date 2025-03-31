#include "player.h"

Player::Player() : pos(glm::vec2(100, 100)), speed(200), dash(true), dashSpeed(5000), dashCooldown(5.0f) {}

void Player::processInput(GLFWwindow *window, double deltaTime) {
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
            this->pos += glm::normalize(direction) * this->dashSpeed * (float) deltaTime;
            this->dash = false;
            this->dashCooldown = 5.0f;
        } else { 
            this->pos += glm::normalize(direction) * this->speed * (float) deltaTime;
        }

    // Dash cooldown management
    if(!this->dash) {
        this->dashCooldown -= deltaTime;

        if(0.0f >= this->dashCooldown) {
            this->dash = true; 
        }
    }
}

glm::mat4 Player::getView(int width, int height) {
    return glm::translate(glm::mat4(1.f), glm::vec3(-this->pos.x + width/2, -this->pos.y + height/2, 0));
}

void Player::draw(Renderer *renderer, glm::mat4 view) {
    renderer->drawTexture(this->texture, view, this->pos, glm::vec2(25, 25), 0);
}