#include "Shader.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

bool Shader::Load(const std::string& vertName, const std::string& fragName) {
	if (!CompileShader(vertName, GL_VERTEX_SHADER, vertexShader)
		|| !CompileShader(fragName, GL_FRAGMENT_SHADER, fragShader))
		return false;
	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	return IsValidProgram();
}

void Shader::Unload() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragShader);
	glDeleteShader(vertexShader);
}

void Shader::SetActive() {
	glUseProgram(shaderProgram);
}

void Shader::SetUniformValue(const char* name, const Matrix4& value) {
	const auto loc = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(value.mat));
}

void Shader::SetUniformValue(const char* name, const Vector3& value) {
	const auto loc = glGetUniformLocation(shaderProgram, name);
	glUniform3fv(loc, 1, &value.x);
}

void Shader::SetUniformValue(const char* name, const float value) {
	const auto loc = glGetUniformLocation(shaderProgram, name);
	glUniform1f(loc, value);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) {
	std::ifstream shaderFile{ fileName };

	if (!shaderFile.is_open()) {
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	std::stringstream sstream;
	sstream << shaderFile.rdbuf();
	const auto contents = sstream.str();
	const auto* contentsChar = contents.c_str();

	outShader = glCreateShader(shaderType);
	glShaderSource(outShader, 1, &contentsChar, nullptr);
	glCompileShader(outShader);

	if (!IsCompiled(outShader)) {
		SDL_Log("Failed to compile shader %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buf[512]{ 0, };
		glGetShaderInfoLog(shader, 511, nullptr, buf);
		SDL_Log("GLSL compile failed:\n%s", buf);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram() {
	GLint status;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		char buf[512]{ 0, };
		glGetProgramInfoLog(shaderProgram, 511, nullptr, buf);
		SDL_Log("GLSL link status:\n%s", buf);
		return false;
	}

	return true;
}