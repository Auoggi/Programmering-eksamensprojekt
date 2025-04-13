#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "render/renderer.h"
#include "entity/player.h"
#include "grid/grid.h"

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

    Entity *obstacle = new Entity("assets/textures/ball64.png", "obstacle", 64, 64);
    obstacle->pos = glm::vec2(96, 96);

    double currentTime = glfwGetTime();
    double lastTime = currentTime;
    double deltaTime;

    // init grid
    const int tileSize = 64;
    Grid *grid = new Grid(tileSize, (float) mode->width, (float) mode->height);

    float vertices[] = {
        0, 0,  
        (float) mode->width, 0,  
        0, (float) mode->height,  
        (float) mode->width, (float) mode->height
    };

    
    // Create VAO and VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Send VAO and VBO to the gpu, for use in rendering
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 8x8 grid of 64x64 textures for use in rendering map
    Texture tilemap = ResourceManager::loadTexture("assets/textures/tilemap.png", "tilemap");
    int tilemapColumnCount = 8;
    int tilemapRowCount = 8;

    Shader shader = ResourceManager::setShader(GRID_VERTEX_GLSL, MAP_FRAGMENT_GLSL, "map");

    shader.use();
    shader.setInteger("image", 0);
    shader.setFloat("tileSize", tileSize);
    shader.setVector2f("screenSize", (float) mode->width, (float) mode->height);

    shader.setInteger("columnCount", tilemapColumnCount);
    shader.setInteger("rowCount", tilemapRowCount);

    while(!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        
        player->processInput(window, deltaTime);

        glm::mat4 view = player->getView(screenWidth, screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setMatrix4("view", view);

        glActiveTexture(GL_TEXTURE0);
        tilemap.bind();
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        grid->draw(view, floor(player->pos.x / tileSize), floor(player->pos.y / tileSize));
        player->draw(renderer, view);       
        obstacle->draw(renderer, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}