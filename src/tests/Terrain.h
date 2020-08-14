#pragma once
#include "Test.h"
#include "Model.h"
#include "Shader.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


struct terrainVertices
{
	glm::vec3 position;
	glm::vec3 normals;
};

namespace test {
	class Terrain : public Test {
	private:
		std::vector<terrainVertices> m_vertices;
		float m_fly = 0.0;
		float m_zCoord[100][80];
        unsigned int m_vao;
        unsigned int m_vbo;
        Shader m_program;
        Shader m_programGrid;
		bool m_wireframe = false;
		float xo= 0.1,yo=0.1;
		glm::vec2 height = glm::vec2(-0.5f,0.5f);
	public:
		Terrain();
		~Terrain() ;
		void GenerateTerrain();
		void OnUpdate(float deltatime)override;
		void OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 camPos)override;
		void OnImGuiRender() override;

	};


}