#ifndef RESOURCE_MANAGER_CLASS
#define RESOURCE_MANAGER_CLASS

#include <glad/glad.h>

#include <map>
#include <string>
#include <stdio.h>
#include <sstream>
#include <fstream>

#include "shaders/shader_symbols.h"
#include "texture.h"
#include "shader.h"

class ResourceManager {
public:
    static Shader loadShader(const char *vertexFile, const char *fragmentFile, std::string name);
    static Shader setShader(const char *vertexCode, const char *fragmentCode, std::string name);
    static Shader getShader(std::string name);

    static Texture loadTexture(const char *file, std::string name);
    static Texture getTexture(std::string name);
private:
    ResourceManager() {};

    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> textures;
};

#endif