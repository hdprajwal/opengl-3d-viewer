#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <functional>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
namespace test {

	class Test {
	public:
	public:
		Test(){}
		virtual ~Test() {}
		
		virtual void OnUpdate(float deltatime){}
		virtual void OnRender(glm::mat4 proj , glm::mat4 view ,glm::vec3 camPos) {}
		virtual void OnImGuiRender() {}
	};
	class TestMenu : public Test {
	public:
		TestMenu(Test*& currentTestPtr);

		void OnUpdate(float deltatime)override;
		void OnRender(glm::mat4 proj, glm::mat4 view ,glm::vec3 camPos) override;

		std::string inline GetCurrentTestName() const { return m_CurrentTestName; }
		void inline ResetTestName() { m_CurrentTestName = m_TestMenuName; }
		void OnImGuiRender() override;
		template<typename T>
		void RegisterTest(const std::string& name) {
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

	private:
		std::string m_TestMenuName = "OpenGL Test Menu";
		std::string m_CurrentTestName;
		Test*& m_currentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}