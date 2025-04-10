#include "player.h"

Player::Player() : Entity("assets/textures/ball.png", "player", 25, 25), dash(true), isDashing(false),
                            dashSpeed(1500), dashCooldown(5.0f), dashTimer(0.0f), dashDuration(0.2f), dashSpeedChange(100) {}

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
    if(glfwGetKey(window, GLFW_KEY_A)) {
        direction += glm::vec2(-1, 0);
    }

    // Dashing feature and player movement
    if(glm::length(direction) != 0) {

        if(glfwGetKey(window, GLFW_KEY_SPACE) && this->dash && !this->isDashing) {
            // Start the dash
            this->isDashing = true;
            this->dashTimer = 0.0f;
            this->dash = false;
        }
        
        if(isDashing){
            // apply Dash movement
            this->pos += glm::normalize(direction) * this->dashSpeed * (float) deltaTime;

            // Increase dashTimer
            this->dashTimer += deltaTime;

            // easein and easeout mechanic
            if(this->dashSpeed >= 2500) this->dashSpeedChange *= -1;
            this->dashSpeed += this->dashSpeedChange;
            
            // Duration control of dash
            if(this->dashDuration <= dashTimer) {
                this->isDashing = false;
                this->dashCooldown = 5.0f;
                this->dashSpeedChange *= -1;
            }

        } else { // if not dashing, then regular movement
            this->pos += glm::normalize(direction) * this->speed * (float) deltaTime;
        }
    }

    // Dash cooldown management
    if(!this->dash && !this->isDashing) {
        this->dashCooldown -= deltaTime;

        if(0.0f >= this->dashCooldown) {
            this->dash = true; 
        }
    }
}

glm::mat4 Player::getView(int screenWidth, int screenHeight) {
    return glm::translate(glm::mat4(1.f), glm::vec3(-this->pos.x + screenWidth/2, -this->pos.y + screenHeight/2, 0));
}
