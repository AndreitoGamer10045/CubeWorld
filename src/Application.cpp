#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "engine/Graphics.hpp"

static void onWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static float lastX = 400.0f;
static float lastY = 400.0f;
static float yaw = -90.0f;
static float pitch = 0.0f;
static bool firstMouse = true;
static glm::vec3 cameraFront;

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false; // Evita que se ejecute más de una vez
	}

	float xoffset = xpos - lastX; // Calcular el desplazamiento del mouse
	float yoffset = lastY - ypos; // Invertir el eje Y
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f; // Sensibilidad del mouse
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;   // Actualiza el ángulo de giro
	pitch += yoffset; // Actualiza el ángulo de elevación

	// Limita el pitch para evitar un giro completo
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Actualiza la dirección de la cámara
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front); // Asigna la dirección normalizada
}

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	float fov;
	float aspectRatio;

	Camera(glm::vec3 startPosition, float fov, float aspectRatio)
		: position(startPosition), front(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), fov(fov), aspectRatio(aspectRatio) {}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	glm::mat4 getProjectionMatrix() {
		return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
	}
};

static struct Vertex {
	glm::ivec3 position;
	glm::vec4 color;
	glm::vec2 uv;
};

static enum FaceDirections {
	FRONT = 0,
	BACK,
	UP,
	DOWN,
	RIGHT,
	LEFT
};

static void addFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const glm::ivec3& position, FaceDirections direction)
{
	std::vector<Vertex> faceVertices;
	glm::vec4 faceColor(1.0f, 1.0f, 1.0f, 1.0f);

	switch (direction) {
	case FRONT: // Front face
		faceVertices = {
			{ glm::ivec3(0, 0, 0) + position, faceColor, glm::vec2(0, 0) }, // Inferior izquierdo
			{ glm::ivec3(1, 0, 0) + position, faceColor, glm::vec2(1, 0) }, // Inferior derecho
			{ glm::ivec3(1, 1, 0) + position, faceColor, glm::vec2(1, 1) }, // Superior derecho
			{ glm::ivec3(0, 1, 0) + position, faceColor, glm::vec2(0, 1) }  // Superior izquierdo
		};
		break;

	case BACK: // Back face
		faceVertices = {
			{ glm::ivec3(0, 1, -1) + position, faceColor, glm::vec2(1, 1) }, // Superior izquierdo
			{ glm::ivec3(1, 1, -1) + position, faceColor, glm::vec2(0, 1) }, // Superior derecho
			{ glm::ivec3(1, 0, -1) + position, faceColor, glm::vec2(0, 0) }, // Inferior derecho
			{ glm::ivec3(0, 0, -1) + position, faceColor, glm::vec2(1, 0) }  // Inferior izquierdo
		};
		break;

	case UP: // Top face
		faceVertices = {
			{ glm::ivec3(0, 1, 0) + position, faceColor, glm::vec2(0, 0) },
			{ glm::ivec3(1, 1, 0) + position, faceColor, glm::vec2(1, 0) },
			{ glm::ivec3(1, 1, -1) + position, faceColor, glm::vec2(1, 1) },
			{ glm::ivec3(0, 1, -1) + position, faceColor, glm::vec2(0, 1) }
		};
		break;

	case DOWN: // Bottom face
		faceVertices = {
			{ glm::ivec3(0, 0, -1) + position, faceColor, glm::vec2(1, 1) },
			{ glm::ivec3(1, 0, -1) + position, faceColor, glm::vec2(0, 1) },
			{ glm::ivec3(1, 0, 0) + position, faceColor, glm::vec2(0, 0) },
			{ glm::ivec3(0, 0, 0) + position, faceColor, glm::vec2(1, 0) }
		};
		break;

	case RIGHT: // Right face
		faceVertices = {
			{ glm::ivec3(1, 0, 0) + position, faceColor, glm::vec2(0, 0) }, // Inferior izquierdo
			{ glm::ivec3(1, 0, -1) + position, faceColor, glm::vec2(1, 0) }, // Superior izquierdo
			{ glm::ivec3(1, 1, -1) + position, faceColor, glm::vec2(1, 1) }, // Superior derecho
			{ glm::ivec3(1, 1, 0) + position, faceColor, glm::vec2(0, 1) }  // Inferior derecho
		};
		break;

	case LEFT: // Left face
		faceVertices = {
			{ glm::ivec3(0, 0, -1) + position, faceColor, glm::vec2(0, 0) }, // Inferior derecho
			{ glm::ivec3(0, 0, 0) + position, faceColor, glm::vec2(1, 0) }, // Superior derecho
			{ glm::ivec3(0, 1, 0) + position, faceColor, glm::vec2(1, 1) }, // Superior izquierdo
			{ glm::ivec3(0, 1, -1) + position, faceColor, glm::vec2(0, 1) }  // Inferior izquierdo
		};
		break;

	}

	vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());

	unsigned int baseIndex = vertices.size() - faceVertices.size();

	indices.push_back(baseIndex + 0); // Vértice 0
	indices.push_back(baseIndex + 1); // Vértice 1
	indices.push_back(baseIndex + 2); // Vértice 2
	indices.push_back(baseIndex + 0); // Vértice 0
	indices.push_back(baseIndex + 2); // Vértice 2
	indices.push_back(baseIndex + 3); // Vértice 3
}

static void addCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const glm::ivec3& position)
{
	addFace(vertices, indices, position, FRONT);
	addFace(vertices, indices, position, BACK);
	addFace(vertices, indices, position, UP);
	addFace(vertices, indices, position, DOWN);
	addFace(vertices, indices, position, RIGHT);
	addFace(vertices, indices, position, LEFT);
}

int main()
{
	const int windowWidth = 800;
	const int windowHeight = 800;
	const char* windowTitle = "CubeWorld V0.1";

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	const int CHUNK_SIZE = 32;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				addCube(vertices, indices, { x, y, z });
			}
		}
	}

	#pragma region OpenGL context and window

	Window window(windowWidth, windowHeight, windowTitle, false);

	window.setResizeCallback(onWindowResize);
	window.setMousePositionCallback(mouse_callback);

	window.setVSync(true);
	//window.setFPSLimit(60);
	window.setBackFaceCulling(true);
	window.enableDepthTest();

	window.setMouseState(GLFW_CURSOR_HIDDEN);
	window.setMouseState(GLFW_CURSOR_DISABLED);

	#pragma endregion

	#pragma region Image Loading

	Texture texture(RESOURCES_PATH "textures/urmom.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);

	#pragma endregion

	#pragma region Shaders
	
	Shader vertexShader(GL_VERTEX_SHADER, RESOURCES_PATH "shaders/default.vert");
	Shader fragmentShader(GL_FRAGMENT_SHADER, RESOURCES_PATH "shaders/default.frag");

	vertexShader.compile();
	fragmentShader.compile();

	ShaderProgram shaderProgram;

	shaderProgram.attachShader(vertexShader);
	shaderProgram.attachShader(fragmentShader);
	shaderProgram.link();
	shaderProgram.use();

	#pragma endregion
	
	#pragma region Buffers

	VertexArray VAO;
	
	Buffer VBO(GL_ARRAY_BUFFER), EBO(GL_ELEMENT_ARRAY_BUFFER);

	VAO.bind();

	VBO.bind();
	VBO.setData(sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	EBO.bind();
	EBO.setData(sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	#pragma endregion

	#pragma region Vertex Attributes

	vertexAttributes::add(0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, Vertex::position));
	vertexAttributes::add(1, 4, GL_FLOAT, true, sizeof(Vertex), offsetof(Vertex, Vertex::color));
	vertexAttributes::add(2, 2, GL_FLOAT, true, sizeof(Vertex), offsetof(Vertex, Vertex::uv));

	#pragma endregion

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 80.0f, (float)windowWidth / (float)windowHeight);
	float cameraSpeed = 25.0f;

	while (window.isRunning())
	{
		float deltaTime = window.getDeltaTime();
		float fps = window.getFPS();

		std::cout << fps << "\n";

		#pragma region Update

		if (window.isKeyPressed(GLFW_KEY_ESCAPE)) {
			window.setMouseState(GLFW_CURSOR_NORMAL);
			window.close();
		}

		if (window.isKeyPressed(GLFW_KEY_W)) {
			camera.position += cameraSpeed * cameraFront * deltaTime;
		}
		if (window.isKeyPressed(GLFW_KEY_S)) {
			camera.position -= cameraSpeed * cameraFront * deltaTime;
		}
		if (window.isKeyPressed(GLFW_KEY_A)) {
			camera.position -= glm::normalize(glm::cross(cameraFront, camera.up)) * cameraSpeed * deltaTime;
		}
		if (window.isKeyPressed(GLFW_KEY_D)) {
			camera.position += glm::normalize(glm::cross(cameraFront, camera.up)) * cameraSpeed * deltaTime;
		}

		glm::mat4 view = glm::lookAt(camera.position, camera.position + cameraFront, camera.up);
		glm::mat4 projection = camera.getProjectionMatrix();

		shaderProgram.setUniformMatrix4("view", view);
		shaderProgram.setUniformMatrix4("projection", projection);

		#pragma endregion

		#pragma region Drawing

		VBO.bind();
		VBO.updateData(vertices.size() * sizeof(Vertex), vertices.data());

		EBO.bind();
		EBO.updateData(indices.size() * sizeof(unsigned int), indices.data());

		window.clearColor(0.7f, 0.8f, 0.9f, 1.0f);
		window.clearDepthBuffer();

		shaderProgram.use();

		texture.activate(0);

		VAO.bind();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		#pragma endregion

		window.endFrame();
	}
}