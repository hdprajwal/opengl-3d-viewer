#include "Test.h"
#include "imgui/imgui.h"
#include <GL/glew.h>
namespace test {

	TestMenu::TestMenu(Test*& currentTestPtr) :m_CurrentTestName(m_TestMenuName),
		m_currentTest(currentTestPtr)
	{
	}

	void TestMenu::OnUpdate(float deltatime)
	{

	}

	void TestMenu::OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 camPos)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void TestMenu::OnImGuiRender()
	{
		for (unsigned int i = 0; i < m_Tests.size(); i++)
		{
			if (i > 0 && i % 4 != 0)
				ImGui::SameLine();
			else
				ImGui::Separator();

			auto testPair = m_Tests.at(i);

			if (ImGui::Button(testPair.first.c_str()))
			{
				m_currentTest = testPair.second();
				m_CurrentTestName = testPair.first;
			}
		}
	}
}