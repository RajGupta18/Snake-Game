#include "shader.h"
#include <iostream>

Shader::Shader() {
    ID = 0;
}

Shader::Shader(const unsigned int &id) {
    ID = id;
}

void Shader::UseShader() {
    if(ID) glUseProgram(ID);
}

void Shader::set_int(const std::string &name, const int &val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::set_float(const std::string &name, const float &val) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::set_vec2(const std::string &name, const glm::vec2 &val) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &val[0]);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &val) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &val[0]);
}

Shader::~Shader() {
    if(ID) glDeleteProgram(ID);
}