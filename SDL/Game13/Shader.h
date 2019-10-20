#pragma once

#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader {
public:
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	void SetActive();

	void SetUniformValue(const char* name, Matrix4* value, unsigned count);
	void SetUniformValue(const char* name, const Matrix4& value);
	void SetUniformValue(const char* name, const Vector3& value);
	void SetUniformValue(const char* name, const Vector2& value);
	void SetUniformValue(const char* name, float value);
	void SetUniformValue(const char* name, int value);

private:
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();

private:
	GLuint vertexShader;
	GLuint fragShader;
	GLuint shaderProgram;
};

