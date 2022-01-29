#include "resource_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::unordered_map<std::string, Shader*> ResourceManager::resources = std::unordered_map<std::string, Shader*>();

void ResourceManager::LoadShader(std::string shaderName, std::string vertexFile,
                                                        std::string geometricFile,
                                                        std::string fragmentFile) {
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream gShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexFile);
		gShaderFile.open(geometricFile);
		fShaderFile.open(fragmentFile);

		std::stringstream vShaderStream, gShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		gShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		geometryCode = gShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ -->" << shaderName << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int id = CompileShader(vShaderCode, gShaderCode , fShaderCode);
	resources[shaderName] = new Shader(id);
}

void ResourceManager::LoadShader(std::string shaderName, std::string vertexFile,
                                                        std::string fragmentFile) {
    std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ -->" << shaderName << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int id = CompileShader(vShaderCode, fShaderCode);
	resources[shaderName] = new Shader(id);
}

unsigned int ResourceManager::CompileShader(const char* vcode, const char* gcode, const char* fcode) {
	unsigned int id = glCreateProgram();

	if (!id)
	{
		std::cout << "Error creating shader program!" << std::endl;
		return id;
	}

	unsigned int vertex = AddShader(id, vcode, GL_VERTEX_SHADER);
	unsigned int geometry = AddShader(id, gcode, GL_GEOMETRY_SHADER);
	unsigned int fragment = AddShader(id, fcode, GL_FRAGMENT_SHADER);

	int result = 0;
	char eLog[1024] = { 0 };

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: " << eLog << std::endl;
		return id;
	}

	glValidateProgram(id);
	glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program: " << eLog << std::endl;
		return id;
	}
	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);
	return id;
}

unsigned int ResourceManager::CompileShader(const char* vcode, const char* fcode) {
	unsigned int id = glCreateProgram();

	if (!id)
	{
		std::cout << "Error creating shader program!" << std::endl;
		return id;
	}

	unsigned int vertex = AddShader(id, vcode, GL_VERTEX_SHADER);
	unsigned int fragment = AddShader(id, fcode, GL_FRAGMENT_SHADER);

	int result = 0;
	char eLog[1024] = { 0 };

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: " << eLog << std::endl;
		return id;
	}

	glValidateProgram(id);
	glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program: " << eLog << std::endl;
		return id;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return id;
}

unsigned int ResourceManager::AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType) {
	unsigned int theShader = glCreateShader(shaderType);

	glShaderSource(theShader, 1, &shaderCode, NULL);
	glCompileShader(theShader);

	int result = 0;
	char eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the shader: " << eLog << std::endl;
		return theShader;
	}

	glAttachShader(theProgram, theShader);
	return theShader;
}

void ResourceManager::Clear() {
	for(auto it: resources) {
		delete it.second;
	}
	resources.clear();
}