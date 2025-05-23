#include "player.h"

Player::Player() : Entity("assets/textures/ball.png", "player", 25, 25, 100), stamina(25.0f), dash(true), isDashing(false),
                            minDashSpeed(1000), maxDashSpeed(1600), dashCooldown(2.0f), dashTimer(0.0f), dashDuration(0.1f),
                            acceleration(125.0f), deacceleration(150.0f),staminaRegenRate(1.0f) {}

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

        if(glfwGetKey(window, GLFW_KEY_SPACE) && this->dash && !this->isDashing && this->stamina > 10) {
            // Start the dash
            this->isDashing = true;
            this->dashTimer = 0.0f;
            this->dash = false;

            // adjust stamina
            this->stamina -= 10;
        }
        
        if(this->isDashing) {
            // easein and easeout mechanic made using sinus
            float progress = glm::clamp(this->dashTimer / this->dashDuration, 0.0f, 1.0f);
            float easedProgress = (sin((progress - 0.5f) * glm::pi<float>()) + 1.0f) / 2.0f;
            this->dashSpeed = glm::mix(this->minDashSpeed, this->maxDashSpeed, easedProgress);

            // apply Dash movement
            this->velocity = glm::normalize(direction) * this->dashSpeed;

            // Increase dashTimer
            this->dashTimer += deltaTime;
            
            // Duration control of dash
            if(this->dashDuration <= this->dashTimer) {
                this->isDashing = false;
                this->dashCooldown = 2.0f;
            }

        } else {  // if not dashing, then regular movement with acceleration
            glm::vec2 targetVelocity = glm::normalize(direction) * this->speed;

            // Accelerate toward desired velocity
            glm::vec2 deltaVelocity = targetVelocity - this->velocity;

            // Accelerate if velocity is not the desired velocity
            if (glm::length(deltaVelocity) != 0) {
                glm::vec2 accelStep = glm::normalize(deltaVelocity) * this->acceleration;

                // Clamp the acceleration step to prevent overshoot
                if (glm::length(accelStep) > glm::length(deltaVelocity)) {
                    accelStep = deltaVelocity;
                }

                this->velocity += accelStep;    
        }
    }

    } else {
        // Deacceleration
        if (glm::length(this->velocity) > 0.01f) {
            glm::vec2 decelStep = glm::normalize(this->velocity) * this->deacceleration;
            
            // Makes sure not to deaccelerate to much
            if (this->deacceleration > glm::length(this->velocity)) {
                this->velocity = glm::vec2(0.0f);
            } else {
                this->velocity -= decelStep;
            }
        } else {
            this->velocity = glm::vec2(0.0f);
        }        
    }

    // apply velocity to postion
    this->pos += this->velocity * (float) deltaTime;

    // Dash cooldown management
    if(!this->dash && !this->isDashing) {
        this->dashCooldown -= deltaTime;

        if(0.0f >= this->dashCooldown) {
            this->dash = true; 
        }
    }

    // Stamina regeneration using deltaTime
    if(this->stamina < 25) {
        this->stamina += this->staminaRegenRate * deltaTime;
        if(this->stamina > 25) this->stamina = 25.0f;
    } 
}

glm::mat4 Player::getView(int screenWidth, int screenHeight) {
    return glm::translate(glm::mat4(1.f), glm::vec3(-this->pos.x + screenWidth/2, -this->pos.y + screenHeight/2, 0));
}
