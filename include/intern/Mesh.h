#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

#include <string>
#include <vector>

struct Vertex
{
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

public:
	Mesh(const std::vector<Vertex> vertices,const std::vector<unsigned int> indices,const std::vector<Texture> textures);
	void Draw(Shader &shader);

private:
	unsigned int VBO, EBO;

private:
	void setupMesh();
};
