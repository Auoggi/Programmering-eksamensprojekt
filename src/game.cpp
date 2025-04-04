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

    const int tileSize = 64;
    Grid *grid = new Grid(tileSize);

    // --- Temporary grid rendering part 1, TODO: fix non-centered y-axis ---
    Shader grid_d = ResourceManager::setShader(
        _binary_src_render_shaders_grid_vertex_glsl_start, 
        _binary_src_render_shaders_grid_vertex_glsl_end - _binary_src_render_shaders_grid_vertex_glsl_start,
        _binary_src_render_shaders_grid_fragment_glsl_start,
        _binary_src_render_shaders_grid_fragment_glsl_end - _binary_src_render_shaders_grid_fragment_glsl_start,
        "grid"
    );
    
    float vertices[] = {
        0, 0,  
        (float) mode->width, 0,  
        0, (float) mode->height,  
        (float) mode->width, (float) mode->height
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // --- end of part 1 ---

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

        // --- Temporary grid rendering part 2 ---
        grid_d.use();

        glUniform2f(glGetUniformLocation(grid_d.ID, "screenSize"), screenWidth, screenHeight);

        glUniformMatrix4fv(glGetUniformLocation(grid_d.ID, "view"), 1, false, glm::value_ptr(view));
        glUniform1f(glGetUniformLocation(grid_d.ID, "u_tileSize"), 64.f);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // --- end of part 2 ---

        player->draw(renderer, view);

        obstacle->draw(renderer, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}