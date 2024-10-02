#pragma once

#include <thread>
#include <chrono>

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "engine/DebugHelp.hpp"

class Window
{
public:
	Window(int width, int height, const char* title, bool isResizable);
	~Window();

	bool isRunning();
	void endFrame();
	void close();

	GLFWwindow* getGLFWWindow();
	
	void setResizeCallback(GLFWframebuffersizefun callback);

	bool isKeyPressed(int key);

	void setMousePositionCallback(GLFWcursorposfun callback);
	void setMouseState(int state);
	glm::vec2 getMousePosition();
	void setMousePosition(glm::vec2 newPosition);

	void setVSync(bool VSync);
	void setFPSLimit(unsigned int maxFPS);
	float getFPS();
	float getDeltaTime();

	void setBackFaceCulling(bool culling);
	void enableDepthTest();

	void clearColor(float red, float green, float blue, float alpha);
	void clearDepthBuffer();

private:
	GLFWwindow* m_sourceWindow;

	float m_FPS;

	int m_maxFPS;
	bool m_VSyncEnabled;

	int m_frames;
	float m_secondInterval;

	double m_startTime;
	double m_frameTime;
};