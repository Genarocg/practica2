//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint VBO, VAO, EBO;
float fovyz = -4.0f;
float fovyy = 0.0f;
float fovyx = 0.0f;
struct Vertex {
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
};

// This is for the render with index element
Vertex vertices[] =
{
	//primera cara colo rojo
	{ glm::vec3(-0.5f, -0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },//0
	{ glm::vec3(0.5f , -0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },//1
	{ glm::vec3(0.5f ,  0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },//2
	{ glm::vec3(-0.5f,  0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },//3


	//segunda cara verde
	{ glm::vec3(0.5f , -0.5f, 0.5f) , glm::vec3(0.0f, 1.0f, 0.0f) },//4
	{ glm::vec3(0.5f , -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },//5
	{ glm::vec3(0.5f ,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },//6
	{ glm::vec3(0.5f ,  0.5f, 0.5f) , glm::vec3(0.0f, 1.0f, 0.0f) },//7

	//tercera cara azul
	{ glm::vec3(-0.5f, -0.5f, 0.5f) , glm::vec3(0.0f, 0.0f, 1.0f) },//8
	{ glm::vec3(-0.5f,  0.5f, 0.5f) , glm::vec3(0.0f, 0.0f, 1.0f) },//9
	{ glm::vec3(-0.5f , 0.5f, -0.5f) ,glm::vec3(0.0f, 0.0f, 1.0f) },//10
	{ glm::vec3(-0.5f , -0.5f, -0.5f),glm::vec3(0.0f, 0.0f, 1.0f) },//11

	//cuarta cara
	{ glm::vec3(-0.5f, -0.5f, 0.5f) , glm::vec3(1.0f, 1.0f, 1.0f) },//12
	{ glm::vec3(0.5f , -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f) },//13
	{ glm::vec3(0.5f , -0.5f, 0.5f) , glm::vec3(1.0f, 1.0f, 1.0f) },//14
	{ glm::vec3(-0.5f , -0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f) },//15

	//quinta cara

	{ glm::vec3(-0.5f,  0.5f, 0.5f) , glm::vec3(0.0f, 0.0f, 0.0f) },//16
	{ glm::vec3(0.5f ,  0.5f, 0.5f) , glm::vec3(0.0f, 0.0f, 0.0f) },//17
	{ glm::vec3(0.5f ,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f) },//18
	{ glm::vec3(-0.5f , 0.5f, -0.5f) ,glm::vec3(0.0f, 0.0f, 0.0f) },//19
	//sexta cara
	{ glm::vec3(0.5f , -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f) },//20
	{ glm::vec3(0.5f ,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f) },//21
	{ glm::vec3(-0.5f , 0.5f, -0.5f) ,glm::vec3(1.0f, 0.0f, 1.0f) },//22
	{ glm::vec3(-0.5f , -0.5f, -0.5f),glm::vec3(1.0f, 0.0f, 1.0f) },//23






};

GLuint indices[] = {  // Note that we start from 0!
	0, 1, 2,
	0, 2, 3,

	4, 5, 6,
	4, 6, 7,

	8, 9, 10,
	8, 10, 11,

	12, 13, 14, 
	12, 15, 13,

	16, 17, 18, 
	16, 18, 19,

	20, 21, 22,
	20, 22, 23

};

Shader shader;

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);
void cubo();

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	shader.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");
	cubo();
}

void cubo() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// This is for the render with index element
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// This is for the render with index element
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
		(GLvoid*)sizeof(vertices[0].m_Pos));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shader.destroy();

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		case GLFW_KEY_1:
			fovyz += 0.1;
			break;
		case GLFW_KEY_2:
			fovyz -= 0.1;
			break;
		case GLFW_KEY_3:
			fovyy += 0.1;
			break;
		case GLFW_KEY_4:
			fovyy -= 0.1;
			break;
		case GLFW_KEY_5:
			fovyx += 0.1;
			break;
		case GLFW_KEY_6:
			fovyx -= 0.1;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();
	//glm::vec3 cubePositions[] = {
	//	glm::vec3(-2.0f, -2.5f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(1.0f, -1.0f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f)
	//
	//
	//	/*glm::vec3(4.4f, -0.4f, -3.5f), glm::vec3(-6.7f, 3.0f, -7.5f),
	//	glm::vec3(-4.3f, -3.0f, -2.5f), glm::vec3(3.5f, 8.0f, -2.5f),
	//	glm::vec3(-1.7f, -0.7f, -1.5f), glm::vec3(3.3f, 3.0f, -1.5f)*/
	//};
	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		shader.turnOn();

		GLuint modelLoc = shader.getUniformLocation("model");
		GLuint viewLoc = shader.getUniformLocation("view");
		GLuint projLoc = shader.getUniformLocation("projection");

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(fovyx, fovyy, fovyz));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//	glm::mat4 model = glm::mat4(1.0f);
			//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);


		//pie izquierdo
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -2.5f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.6, 0.2, 0.5));
		GLuint locModel = shader.getUniformLocation("model");
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		// This is for the render with index element
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);


		//pie derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.7f, -2.5f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.6, 0.2, 0.5));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);


		//pierna derecha
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.26f, -1.8f, 0.44f));
		model = glm::rotate(model, (float)-1.58f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9, 0.3, 0.5));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//pierna izquierda
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.73f, -1.8f, 0.44f));
		model = glm::rotate(model, (float)-1.58f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9, 0.3, 0.5));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//cintura
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.85f, -1.055f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.59f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8, 0.3, 0.4));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//Torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.84f, -0.44f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.008f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8, 0.8, 0.4));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//cuello
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.84f, 0.025f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.59f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25, 0.3, 0.1));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//cabeza
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.84f, 0.3f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)-0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)-0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.3));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//brazo izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -0.15f, 1.0f));
		model = glm::rotate(model, (float)1.57f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, (float)-0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.45, 0.2, 0.2));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//brazo derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.25f, -0.15f, 1.0f));
		model = glm::rotate(model, (float)1.55f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, (float)-0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.49, 0.3, 0.2));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//antebrazo izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.75f, -0.713f, 0.44f));
		model = glm::rotate(model, (float)-1.58f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9, 0.25, 0.5));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//antebrazo derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.22f, -0.713f, 0.44f));
		model = glm::rotate(model, (float)-1.58f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9, 0.25, 0.5));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//espada larga
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f, -1.2f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.59f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.15, 0.04, 0.15));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//espada corta
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.93f, -1.2f, 1.0f));
		model = glm::rotate(model, (float)1.57f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.59f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.06, 0.15));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//cuerpo perrito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.008f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1, 0.3, 0.9));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//cabeza perrito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.35f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.59f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3, 0.9, 0.4));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//orejas perrito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.62f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.3f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08, 0.12, 0.2));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//nariz perrito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.28f, 0.3f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3, 0.1, 0.1));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//cola perrito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.8f, 0.1f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.59f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4, 0.4, 0.1));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//pata izquierda perrito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.55f, -0.38f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.56f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.22, 0.9, 0.45));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//pata derecha perrito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.48f, -0.38f, 1.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)1.56f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.22, 0.55, 0.45));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		glBindVertexArray(0);

		shader.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(1000, 650, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}