#include "engine/Window.hpp"

static int OpenGLVersionMajor = 3;
static int OpenGLVersionMinor = 3;

Window::Window(int windowWidth, int windowHeight, const char* windowTitle, bool isResizable)
{
	m_FPS = 0;

	m_maxFPS = 0;
	m_VSyncEnabled = false;

	m_frames = 0;
	m_secondInterval = 0.0f;

	m_startTime = glfwGetTime();
	m_frameTime = 0.0;

	CRITICAL_ASSERT(glfwInit(), "GLFW didn't initialize");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, isResizable);

	m_sourceWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	CRITICAL_ASSERT(m_sourceWindow, "GLFW didn't create the window");
	glfwMakeContextCurrent(m_sourceWindow);

	CRITICAL_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD didn't initialize");
	enableReportGlErrors();

	glViewport(0, 0, windowWidth, windowHeight);
}

Window::~Window()
{
	glfwDestroyWindow(m_sourceWindow);
	glfwTerminate();
}


void Window::close()
{
	glfwSetWindowShouldClose(m_sourceWindow, GLFW_TRUE);
}

void Window::endFrame()
{
	glfwSwapBuffers(m_sourceWindow);
	glfwPollEvents();

	m_frameTime = glfwGetTime() - m_startTime;

	if (m_maxFPS > 0) {
		double frameTimeLimit = 1.0 / m_maxFPS;

		if (m_frameTime < frameTimeLimit) {
			std::this_thread::sleep_for(std::chrono::duration<double>(frameTimeLimit - m_frameTime));
			m_frameTime = glfwGetTime() - m_startTime;
		}
	}

	m_frames += 1;
	m_secondInterval += m_frameTime;
	
	if (m_secondInterval >= 1.0) {
		m_FPS = m_frames;
		m_frames = 0;
		m_secondInterval = 0.0;
	}

	m_startTime = glfwGetTime();
}

bool Window::isRunning()
{
	return !glfwWindowShouldClose(m_sourceWindow);
}


GLFWwindow* Window::getGLFWWindow()
{
	return m_sourceWindow;
}



void Window::setResizeCallback(GLFWframebuffersizefun callback)
{
	glfwSetFramebufferSizeCallback(m_sourceWindow, callback);
}


bool Window::isKeyPressed(int key)
{
	return glfwGetKey(m_sourceWindow, key) == GLFW_PRESS;
}


void Window::setMousePositionCallback(GLFWcursorposfun callback)
{
	glfwSetCursorPosCallback(m_sourceWindow, callback);
}

void Window::setMouseState(int state)
{
	glfwSetInputMode(m_sourceWindow, GLFW_CURSOR, state);
}

glm::vec2 Window::getMousePosition()
{
	double mouseX, mouseY;
	glfwGetCursorPos(m_sourceWindow, &mouseX, &mouseY);
	return glm::vec2(mouseX, mouseY);
}

void Window::setMousePosition(glm::vec2 newPosition)
{
	glfwSetCursorPos(m_sourceWindow, newPosition.x, newPosition.y);
}


void Window::setVSync(bool VSync)
{
	glfwSwapInterval(VSync ? 1 : 0);
}

void Window::setFPSLimit(unsigned int maxFPS)
{
	NON_CRITICAL_ASSERT(m_VSyncEnabled == false, "Is not good to set FPS limit in VSync mode");
	m_maxFPS = maxFPS;
}

float Window::getFPS()
{
	return m_FPS;
}

float Window::getDeltaTime()
{
	return m_frameTime;
}


void Window::setBackFaceCulling(bool culling)
{
	if (culling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void Window::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void Window::clearColor(float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}