#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "PerspectiveCamera.h"
#include "tests/ModelLoader.h"
#include "tests/Terrain.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void GLAPIENTRY printError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

float screenWidth = 1600;
float screenHeight = 900;
PerspectiveCamera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// Initialize the library
	std::cout << "Initaializing GLFW" << std::endl;
	if (!glfwInit())
		return -1;

	//Setup the OpenGL version to use ===== Using version 3.3 Core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a windowed mode window and its OpenGL context 
	std::cout << "Creating a GLFW Window " << std::endl;
	GLFWwindow* window;
	window = glfwCreateWindow((int)screenWidth,(int)screenHeight, "OpenGLProject", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create a GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// GLFW input callback binding 
    std::cout << "Setting GLFW Window Callbacks" << std::endl;
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glewExperimental = 1;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//Initializing GLEW
	std::cout << "Initializing GLEW" << std::endl;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return -1;
	}
	std::cout << "OpenGL Version:" << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "OpenGL Vendor:" << glGetString(GL_VENDOR) << std::endl;


	//Setup Viewport 
	glViewport(0, 0, screenWidth, screenHeight);

	//Enable Debug Output and assign a callback function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(printError, nullptr);

	//Enable Depth test
	glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_FLAT);
	//glDisable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	//Create an ImGui Context
	std::cout << "Creating an ImGui Context" << std::endl;
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");



	test::Test* currentTest = nullptr;
	auto menu = new test::TestMenu(currentTest);
	currentTest = menu;

    menu->RegisterTest<test::ModelLoader>("Modelloader");
    menu->RegisterTest<test::Terrain>("Terrain");

	// Loop until the user closes the window 
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 cam_pos = camera.Position;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
        if(currentTest){

        currentTest->OnUpdate(0.0f);
        currentTest->OnRender(projection, view,cam_pos);
        ImGui::Begin(menu->GetCurrentTestName().c_str());

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        if (currentTest != menu && ImGui::Button("<-- Back")) {
            delete currentTest;
            currentTest = menu;
            menu->ResetTestName();
        }
        currentTest->OnImGuiRender();
        ImGui::End();
        }



		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);
		// Poll for and process events
		glfwPollEvents();

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


void GLAPIENTRY printError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {

	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_MIDDLE)) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}