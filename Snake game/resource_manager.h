#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <glad/glad.h>
#include "shader.h"
#include <unordered_map>
#include <string>

class ResourceManager {
private:
    static unsigned int CompileShader(const char* vcode, const char* fcode);
    static unsigned int CompileShader(const char* vcode, const char* gcode, const char* fcode);
    static unsigned int AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);

public:
    static std::unordered_map<std::string, Shader*> resources;

public:
    static void LoadShader(std::string shaderName,   std::string vertexFile,
                                                    std::string geometricFile,
                                                    std::string fragmentFile);
    static void LoadShader(std::string shaderName,   std::string vertexFile,
                                                    std::string fragmentFile);
    static void Clear();
};
#endif