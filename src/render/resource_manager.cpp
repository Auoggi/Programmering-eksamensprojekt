#include "resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;

Shader ResourceManager::loadShader(const char *vertexFile, const char *fragmentFile, std::string name) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    try {
        // open files
        std::ifstream vertexShaderFile(vertexFile);
        std::ifstream fragmentShaderFile(fragmentFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::exception e) {
        fprintf(stdout, "Shader read error: failed to read shader files\n");
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode);

    shaders[name] = shader;
    return shaders[name];
}

Shader ResourceManager::setShader(const char *vertexCode, const char *fragmentCode, std::string name) {
    Shader shader;
    shader.compile(vertexCode, fragmentCode);

    shaders[name] = shader;
    return shaders[name];
}

Shader ResourceManager::getShader(std::string name) {
    return shaders[name];
}

Texture ResourceManager::loadTexture(const char *file, std::string name) {
    // create texture object
    Texture texture;
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);

    textures[name] = texture;
    return textures[name];
}

Texture ResourceManager::getTexture(std::string name) {
    return textures[name];
}