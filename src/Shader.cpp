#include"Shader.h"
#include<GL/glew.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

Shader::Shader(const std::string& filepath) :m_filepath(filepath), m_ProgramId(0) {
	ShaderSource source = ParseShader(filepath);
	m_ProgramId = CreateShaderSource(source.VertexShader, source.FragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(m_ProgramId);
}

void Shader::Bind() const {
	glUseProgram(m_ProgramId);
}

void Shader::Unbind() const {
	glUseProgram(0);
}


Shader::ShaderSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	enum class Shadertype
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	Shadertype type = Shadertype::NONE;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = Shadertype::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = Shadertype::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}

	}
	return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//glDeleteShader(shader);
	return shader;
}

unsigned int Shader::CreateShaderSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

int Shader::GetUniformLocation(const std::string& variable) {
	if (m_UniformLocationCache.find(variable) != m_UniformLocationCache.end())
		return m_UniformLocationCache[variable];

	int location = glGetUniformLocation(m_ProgramId, variable.c_str());
	if (location == -1)
		std::cout << "Warning Uniform :" << variable << " does not exist!!";
	else
		m_UniformLocationCache[variable] = location;
	return location;
}



void Shader::SetUniform1i(const std::string& variable, int v){
	glUniform1i(GetUniformLocation(variable), v);
}
void Shader::SetUniform2i(const std::string& variable, int v1, int v2 ){
	glUniform2i(GetUniformLocation(variable), v1,v2);
}
void Shader::SetUniform3i(const std::string& variable, int v1, int v2, int v3){
	glUniform3i(GetUniformLocation(variable), v1,v2,v3);
}
void Shader::SetUniform4i(const std::string& variable, int v1, int v2, int v3, int v4 ){
	glUniform4i(GetUniformLocation(variable), v1,v2,v3,v4);
}


void Shader::SetUniform1d(const std::string& variable, double v){
	glUniform1d(GetUniformLocation(variable), v);
}
void Shader::SetUniform2d(const std::string& variable, double v1, double v2){
	glUniform2d(GetUniformLocation(variable), v1, v2);
}
void Shader::SetUniform3d(const std::string& variable, double v1, double v2, double v3){
	glUniform3d(GetUniformLocation(variable), v1, v2, v3);
}
void Shader::SetUniform4d(const std::string& variable, double v1, double v2, double v3, double v4) {
	glUniform4d(GetUniformLocation(variable), v1, v2, v3, v4);
}


void Shader::SetUniform1f(const std::string& variable, float v){
	glUniform1f(GetUniformLocation(variable), v);
}
void Shader::SetUniform2f(const std::string& variable, float v1, float v2) {
	glUniform2f(GetUniformLocation(variable), v1, v2);
}
void Shader::SetUniform3f(const std::string& variable, float v1, float v2, float v3) {
	glUniform3f(GetUniformLocation(variable), v1, v2, v3);
}
void Shader::SetUniform4f(const std::string& variable, float v1, float v2, float v3, float v4){
	glUniform4f(GetUniformLocation(variable), v1, v2, v3, v4);
}


void Shader::SetUniformVec2(const std::string& variable, float x, float y){
	glUniform2f(GetUniformLocation(variable), x, y);
}
void Shader::SetUniformVec3(const std::string& variable, float x, float y, float z){
	glUniform3f(GetUniformLocation(variable), x, y, z);
}
void Shader::SetUniformVec4(const std::string& variable, float x, float y, float z,float w){
	glUniform4f(GetUniformLocation(variable), x, y, z, w);
}


void Shader::SetUniformVec2(const std::string& variable, glm::vec2& vec2Uniform){
	glUniform2fv(GetUniformLocation(variable), 1, &vec2Uniform[0]);
}
void Shader::SetUniformVec3(const std::string& variable, glm::vec3& vec3Uniform){
	glUniform3fv(GetUniformLocation(variable), 1, &vec3Uniform[0]);
}
void Shader::SetUniformVec4(const std::string& variable, glm::vec4& uniform){
	glUniform4fv(GetUniformLocation(variable), 1, &uniform[0]);
}


void Shader::SetUniformMat4(const std::string& variable, glm::mat4& mat4Matrix){
	glUniformMatrix4fv(GetUniformLocation(variable), 1, GL_FALSE, &mat4Matrix[0][0]);
}