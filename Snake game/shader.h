#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<glm/glm.hpp>

#include<string>

class Shader {
private:
    unsigned int ID;

public:
    Shader();
    Shader(const unsigned int &id);
    ~Shader();

    void UseShader();

    //Functions to set uniform variable
    void set_int(const std::string &name, const int &val) const;
    void set_float(const std::string &name, const float &val) const;
    void set_vec2(const std::string &name, const glm::vec2 &val) const;
    void set_vec3(const std::string &name, const glm::vec3 &val) const;
};
#endif