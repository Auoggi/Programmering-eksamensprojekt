#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Player {
public:
    glm::vec2 pos;
    float speed;

    Player();

    void processInput(GLFWwindow *window);
    glm::mat4 getView(int width, int height);
};

#endif