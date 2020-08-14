#pragma once
#include "Test.h"

namespace test {

	class ProceduralSaders :public Test {
	private:
		int m_currentShader;

	public:
		ProceduralSaders();
		~ProceduralSaders();
		void OnUpdate(float deltatime)override;
		void OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 camPos)override;
		void OnImGuiRender() override;
	};

}