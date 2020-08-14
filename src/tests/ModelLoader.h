#pragma once
#include "Test.h"

#include "Model.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imfilebrowser.h"


#include <GLFW/glfw3.h>

namespace test {
	class ModelLoader : public Test {
	private:
		Model m_model;
		Shader m_program;
		bool m_wireframe = false;
        bool m_loaded = false;
        bool m_flippTexture = false;
        bool m_gammaBool = false;
        float m_gamma = 0.75f;
        float m_shine = 32.0f;
        glm::vec4 m_clearColor = glm::vec4(1.0f);
        float m_modelRotationAngles[3] = {0.0f,0.0f,0.0f};
        glm::vec3 m_lightPosition = glm::vec3(1.0f);
        float m_modelSize = 0.4;
        glm::vec3 m_modelPos = glm::vec3(0.0,0.0,0.0);
        ImGui::FileBrowser fileDialogModel;
	public:
		ModelLoader();
		~ModelLoader();
		void OnUpdate(float deltatime)override;
		void OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 camPos)override;
		void OnImGuiRender() override;

	};


}