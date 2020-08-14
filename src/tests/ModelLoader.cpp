#include "ModelLoader.h"
#include <filesystem>


namespace test {
	ModelLoader::ModelLoader():

//            m_model("/home/prajwal/CLionProjects/openglProject/Resources/Models/backpack/backpack.obj",true,true),
		m_program("/home/prajwal/CLionProjects/openglProject/Resources/Shaders/ModelShader/model.shader")
{

    fileDialogModel.SetTitle("File Browser");
}
	ModelLoader::~ModelLoader()
	{
	}
	void ModelLoader::OnUpdate(float deltatime)
	{
	}
	void ModelLoader::OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 camPos)
	{
        glClearColor(m_clearColor.x,m_clearColor.y,m_clearColor.z,m_clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(m_loaded){

		if (m_wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glm::mat4 projection = proj;
		glm::mat4 mview = view;
		glm::mat4 model = glm::mat4(1.0f);
        m_program.Bind();
        m_program.SetUniformMat4("projection", projection);
        m_program.SetUniformMat4("view", mview);
        m_program.SetUniformVec3("lightPos", m_lightPosition);
        m_program.SetUniformVec3("viewPos", camPos);
		// render the loaded model
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_modelPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(m_modelSize,m_modelSize,m_modelSize));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(m_modelRotationAngles[0]),glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(m_modelRotationAngles[1]),glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model,glm::radians(m_modelRotationAngles[2]),glm::vec3(0.0f,0.0f,1.0f));
        m_program.SetUniformMat4("model", model);
        m_program.SetUniform1f("u_gamma", m_gamma);
        m_program.SetUniform1f("u_shine", m_shine);
		m_model.Draw(m_program);

    }

    }
	void ModelLoader::OnImGuiRender()
	{
        if(m_loaded){
            if (ImGui::Button(m_wireframe ? "Disable WireFrame" : "Enable Wireframe")) {
            m_wireframe = !m_wireframe;
        }
        ImGui::SliderFloat3("Background Color", &m_clearColor.x, 0.0, 1.0);
        ImGui::SliderFloat("Gamma", &m_gamma, 0.0, 1.0);
        ImGui::SliderFloat("Specular Shine", &m_shine, 0.0, 64.0);
        ImGui::SliderFloat3("Light Position", &m_lightPosition.x, -10.0, 10.0);
        ImGui::SliderFloat3("ModelPos", &m_modelPos.x, -2.0, 2.0);
        ImGui::SliderFloat("Model Rotation x", &m_modelRotationAngles[0], 0.0, 360.0);
        ImGui::SliderFloat("Model Rotation y", &m_modelRotationAngles[1], 0.0, 360.0);
        ImGui::SliderFloat("Model Rotation z", &m_modelRotationAngles[2], 0.0, 360.0);
        ImGui::SliderFloat("Model Size", &m_modelSize, 0.0, 1.0);
    } else{
            if(ImGui::Button("Select 3D Model")) {
                fileDialogModel.Open();
            }
            ImGui::Checkbox("Gamma Correction",&m_gammaBool);
            ImGui::Checkbox("Flip Texture",&m_flippTexture);
            fileDialogModel.Display();
            if(fileDialogModel.HasSelected())
            {
//                std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
                bool x = m_model.Init(fileDialogModel.GetSelected().string(),m_gammaBool,m_flippTexture);
                if(x){
                    m_loaded = true;
                }
                fileDialogModel.ClearSelected();
            }
        }
    }

}
