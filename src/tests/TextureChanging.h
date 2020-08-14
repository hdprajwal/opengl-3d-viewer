#pragma once
#include "Test.h"

namespace test {

	class TextureChanging : public Test {
	private:

	public:
		TextureChanging();
		~TextureChanging();
		void OnUpdate(float deltatime)override;
		void OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 camPos)override;
		void OnImGuiRender() override;
	};
}