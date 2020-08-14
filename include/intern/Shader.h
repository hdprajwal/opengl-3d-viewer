#pragma once
#include <iostream>
#include <unordered_map>
#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"


class Shader {
private:
	struct ShaderSource
	{
		std::string VertexShader;
		std::string FragmentShader;
	};
	//Unique Program ID
	unsigned int m_ProgramId;
	//Filepath for the Shader files
	std::string m_filepath;
	//Caching mechanism for getting Uniform Location using unordered maps
	std::unordered_map<std::string, int > m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void Unbind() const;

	//Different SetUniform Methods
	void SetUniform1i(const std::string& variable, int v);
	void SetUniform2i(const std::string& variable, int v1, int v2);
	void SetUniform3i(const std::string& variable, int v1, int v2, int v3);
	void SetUniform4i(const std::string& variable, int v1, int v2, int v3, int v4);

	void SetUniform1d(const std::string& variable, double v);
	void SetUniform2d(const std::string& variable, double v1, double v2);
	void SetUniform3d(const std::string& variable, double v1, double v2, double v3);
	void SetUniform4d(const std::string& variable, double v1, double v2, double v3, double v4);

	void SetUniform1f(const std::string& variable, float v);
	void SetUniform2f(const std::string& variable, float v1, float v2);
	void SetUniform3f(const std::string& variable, float v1, float v2, float v3);
	void SetUniform4f(const std::string& variable, float v1, float v2, float v3, float v4);

	void SetUniformVec2(const std::string& variable, float x, float y);
	void SetUniformVec3(const std::string& variable, float x, float y, float z);
	void SetUniformVec4(const std::string& variable, float x, float y, float z, float w);
	
	void SetUniformVec2(const std::string& variable, glm::vec2& vec2Uniform);
	void SetUniformVec3(const std::string& variable, glm::vec3& vec3Uniform);
	void SetUniformVec4(const std::string& variable, glm::vec4& vec4uniform);

	void SetUniformMat4(const std::string& variable, glm::mat4& mat4Matrix);

private:
	ShaderSource ParseShader(const std::string& filepath);
	static unsigned int CreateShaderSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& variable);
};