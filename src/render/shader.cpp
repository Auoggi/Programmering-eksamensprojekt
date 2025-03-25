#include "shader.h"

void Shader::use() {
    glUseProgram(this->ID);
}

void Shader::compile(const char *vertexCode, const char *fragmentCode) {
    unsigned int sVertex, sFragment;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexCode, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentCode, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);   
}

void Shader::checkCompileErrors(unsigned int object, const char *type) {
    int succes;
    char infoLog[1024];

    if(type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &succes);
        if(!succes) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            fprintf(stdout, "Shader compile-time error: \nType: %s\n%s\n", type, infoLog);
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &succes);
        if(!succes) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            fprintf(stdout, "Shader link-time error: \nType: %s\n%s\n", type, infoLog);
        }
    }
}