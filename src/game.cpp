#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "render/renderer.h"
#include "entity/player.h"
#include "entity/enemy.h"
#include "grid/grid.h"
#include "map/map.h"

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Close window if escape is pressed, should be changed
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
    glfwSetErrorCallback(error_callback);

    if(!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primary);
    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "Game", primary, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer *renderer = Renderer::setupRenderer(mode->width, mode->height);
    Player *player = new Player();

    Enemy *obstacle = new Enemy("assets/textures/ball64.png", "obstacle", 64, 64);
    obstacle->pos = glm::vec2(96, 96);

    double currentTime = glfwGetTime();
    double lastTime = currentTime;
    double deltaTime;

    // init grid
    const int tileSize = 64;
    Grid *grid = new Grid(tileSize, (float) mode->width, (float) mode->height);

    int mapArray[25] = {
        0, 5, 9, 2, 0,
        0, 8, 1, 14, 2,
        0, 8, 1, 1, 6,
        0, 4, 7, 7, 3,
        0, 0, 0, 0, 0,
    };

    Map *map = new Map(tileSize, (float) mode->width, (float) mode->height, 5, 5, mapArray);

    while(!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        
        player->tick(window, deltaTime, grid);

        obstacle->tick(player, deltaTime, grid);

        glm::mat4 view = player->getView(screenWidth, screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT);

        map->draw(view);

        //grid->draw(view, floor(player->pos.x / tileSize), floor(player->pos.y / tileSize));
        player->draw(renderer, view);       
        obstacle->draw(renderer, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}