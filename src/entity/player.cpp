#include "player.h"


Player::Player() : pos(glm::vec2(100, 100)), speed(20) {
    
}

void Player::processInput(GLFWwindow *window) {
    glm::vec2 direction = glm::vec2(0, 0);

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

    if(glm::length(direction) != 0){
        this->pos += glm::normalize(direction) * this->speed;
    }
        
    
}
