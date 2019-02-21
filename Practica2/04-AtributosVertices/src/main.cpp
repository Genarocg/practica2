//glew include
#include <GL/glew.h>
//incluir math
#include <math.h>
//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shaders code
const GLchar* vertexShaderSource = { "#version 400\n"

"layout(location=0) in vec3 position;\n"  // estos ceros deben corresponder, posicion, color
"layout(location=1) in vec3 color;\n"
"out vec3 ourColor;\n"

"void main(void)\n"
"{\n"
"  gl_Position = vec4(position, 1.0);\n"
"  ourColor = color;\n"
"}\n" };
const GLchar* fragmentShaderSource = { "#version 400\n"

"in vec3 ourColor;\n"
"out vec4 out_Color;\n"

"void main(void)\n"
"{\n"
"  out_Color = vec4(ourColor, 1.0);\n"
"}\n" };


bool render1 = true;

GLuint VBO, VAO,VBO2,VAO2;// crear otra instancia
GLint vertexShader, fragmentShader, shaderProgram;

typedef struct {
	float XYZ[3];
	float RGB[3];
} Vertex;

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
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// Build and compile our shader program
	// Vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}
	// Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}
	// Link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
			<< std::endl;
	}
	
	Vertex vertices[] =
	{//cuadro de la casa
		{ {-0.3f, -0.9f, 0.0f } ,{ 0.6f, 0.9f, 1.1f } },
		{ { 0.3f, -0.9f, 0.0f } ,{  0.6f, 0.9f, 1.1f } },
		{ { 0.3f,  -0.1f, 0.0f  } ,{  0.6f, 0.9f, 1.1f } },
		{ {-0.3f, -0.9f, 0.0f } ,{  0.6f, 0.9f, 1.1f } },
		{ { 0.3f,  -0.1f, 0.0f  } ,{  0.6f, 0.9f, 1.1f } },
		{ {-0.3f,  -0.1f, 0.0f  } ,{  0.6f, 0.9, 1.1f } },
		//techo
		{ {-0.3f,  -0.08f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		{ {0.3f,   -0.08f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		{ {-0.03f, 0.4f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		//cuadro extra para darle forma de la imagen al techo
		{ {-0.3f,  -0.1f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		{ {0.3f,   -0.1f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		{ {0.3f, -0.08f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		{ {-0.3f,  -0.1f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		{ {0.3f,   -0.08f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		{ {-0.3f, -0.08f, 0.0f  } ,{ 1.0f,  0.7f, 0.5f } },
		//rectangulo bajo el techo
		{ {-0.3f, -0.21f, 0.0f  } ,{1.0f,   0.6f, 0.5f } },
		{ {0.3f,  -0.21f, 0.0f   } ,{1.0f,   0.6f, 0.5f } },
		{ {0.3f,  -0.1f, 0.0f   } ,{1.0f,   0.6f, 0.5f } },
		{ {-0.3f, -0.21f, 0.0f   } ,{1.0f,   0.6f, 0.5f } },
		{ {0.3f,  -0.1f, 0.0f  } ,{1.0f,   0.6f, 0.5f } },
		{ {-0.3f, -0.1f, 0.0f   } ,{1.0f,   0.6f, 0.5f } },
		//triangulo encima del techo (izquierdo)
		{ {-0.35f, -0.11f,0.0f} , {1.0f,0.0f,0.0f} },
		{ {-0.32f, -0.11f, 0.0f} , {1.0f,0.0f,0.0f} },
		{ {-0.03f, 0.4f, 0.0f} , {1.0f,0.0f,0.0f } },
		{ {-0.35f, -0.11f,0.0f} , {1.0f,0.0f,0.0f} },
		{ {-0.03f, 0.4f, 0.0f} , {1.0f,0.0f,0.0f} },
		{ {-0.03f, 0.44f, 0.0f} , {1.0f,0.0f,0.0f } },
		//triangulo encima del techo (derecho)
		{ {0.35f, -0.11f,0.0f} , {1.0f,0.0f,0.0f} },
		{ {0.32f, -0.11f, 0.0f} , {1.0f,0.0f,0.0f} },
		{ {-0.03f, 0.4f, 0.0f} , {1.0f,0.0f,0.0f } },
		{ {0.35f, -0.11f,0.0f} , {1.0f,0.0f,0.0f} },
		{ {-0.03f, 0.4f, 0.0f} , {1.0f,0.0f,0.0f} },
		{ {-0.03f, 0.44f, 0.0f} , {1.0f,0.0f,0.0f } },
		//puerta (primer rectangulo).
		{ {-0.25f,-0.9f,0.0f} , {0.9f, 0.4, 0.0f} },
		{ {-0.05f,-0.9f,0.0f} , {0.9f, 0.4, 0.0f} },
		{ {-0.05f, -0.3f,0.0f} , {0.9f, 0.4, 0.0f} },
		{ {-0.25f,-0.9f,0.0f} , {0.9f, 0.4, 0.0f} },
		{ {-0.25f,-0.3f, 0.0f } , {0.9f, 0.4, 0.0f} },
		{ {-0.05f, -0.3f, 0.0f} , {0.9f, 0.4, 0.0f} },
		//segundo rectangulo de la puerta.
		{ {-0.22f,-0.87f, 0.0f } , {1.0f,   0.6f, 0.5f} },
		{ {-0.07f, -0.87f, 0.0f} , {1.0f,   0.6f, 0.5f} },
		{ {-0.07f, -0.35f,0.0f} , {1.0f,   0.6f, 0.5f} },
		{ {-0.22f, -0.87f,0.0f} , {1.0f,   0.6f, 0.5f} },
		{ {-0.22f, -0.35f, 0.0f} , {1.0f,   0.6f, 0.5f} },
		{ {-0.07f,-0.35f, 0.0f } , {1.0f,   0.6f, 0.5f} },
		//ventana techo (primer cuadro)
		{ {-0.06f,0.05f, 0.0f } , {1.0f,   1.0f, 1.0f} },
		{ {0.01f, 0.05f, 0.0f} , {1.0f,   1.0f, 1.0f} },
		{ {0.01f, 0.2f,0.0f} , {1.0f,   1.0f, 1.0f} },
		{ {-0.06f, 0.05f, 0.0f } , {1.0f,   1.0f, 1.0f} },
		{ {0.01f, 0.2f,0.0f} , {1.0f,   1.0f, 1.0f} },
		{ {-0.06f, 0.2f,0.0f} , {1.0f, 1.0f, 1.0f} },
		//segundo cuadro de la ventana
		{ {-0.0515f, 0.059f,0.0f} , {0.0f, 0.8f, 1.0f} },
		{ {0.003f, 0.059f, 0.0f} , {0.0f, 0.8f, 1.0f} },
		{ {0.003f, 0.19f,0.0f} , {0.0f, 0.8f, 1.0f} },
		{ {-0.0515f, 0.059f, 0.0f} , {0.0f, 0.8f, 1.0f} },
		{ {0.003f, 0.19f, 0.0f} , {0.0f, 0.8f, 1.0f} },
		{ {-0.0515f,0.19f,0.0f} , {0.0f, 0.8f, 1.0f} },

		//piso, primera linea
			{ {-0.7f,-0.92f,0.0f},{0.6f,   0.0f, 0.0f} },
			{ {0.7f,-0.92f,0.0f},{0.6f,   0.0f, 0.0f} },
			{ {0.7f, -0.9f,0.0f},{0.6f,   0.0f, 0.0f} },
			{ {-0.7f, -0.92f, 0.0f},{0.6f,   0.0f, 0.0f} },
			{ {0.7f, -0.9,0.0f},{0.6f,   0.0f, 0.0f} },
			{ {-0.7f, -0.9f, 0.0f},{0.6f,   0.0f, 0.0f} },
			//contorno ventana  M
			{ {0.02f,-0.72f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.22f,-0.72f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.22f, -0.31f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.02f,-0.72f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.22f,-0.31f, 0.0f } , {1.0f,   0.6f, 0.5f} },
			{ {0.02f, -0.31f, 0.0f} , {1.0f,   0.6f, 0.5f} },
			//contorno ventana interior
			{ {0.033f,-0.7f,0.0f} , {0.0f, 0.5, 1.0f }},
			{ {0.21f,-0.7f,0.0f} , {0.0f, 0.5, 1.0f} },
			{ {0.21f, -0.33f,0.0f} , {0.0f, 0.5, 1.0f} },
			{ {0.033f,-0.7f,0.0f} , {0.0f, 0.5, 1.0f} },
			{ {0.21f,-0.33f, 0.0f } , {0.0f, 0.5, 1.0f} },
			{ {0.033f, -0.33f, 0.0f} , {0.0f, 0.5, 1.0f} },
			//linea horizontal en la ventana
			{ {0.033f,-0.42f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.21f,-0.42f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.21f, -0.45f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.033f,-0.42f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.21f,-0.45f, 0.0f } , {1.0f,   0.6f, 0.5f} },
			{ {0.033f, -0.45f, 0.0f} , {1.0f,   0.6f, 0.5f} },
			//linea vertical dentro de la ventana
			{ {0.105f,-0.45f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.13f,-0.45f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.13f, -0.7f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.105f,-0.45f,0.0f} , {1.0f,   0.6f, 0.5f} },
			{ {0.13f,-0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
			{ {0.105f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
			//base de la ventana
			{ {-0.01f,-0.72f, 0.0f } , {1.0f,   1.0f, 1.0f} },
			{ {0.25f, -0.72f, 0.0f} , {1.0f,   1.0f, 1.0f} },
			{ {0.25f, -0.78f,0.0f} , {1.0f,   1.0f, 1.0f} },
			{ {-0.01f, -0.72f, 0.0f } , {1.0f,   1.0f, 1.0f} },
			{ {0.25f, -0.78f,0.0f} , {1.0f,   1.0f, 1.0f} },
			{ {-0.01f, -0.78f,0.0f} , {1.0f, 1.0f, 1.0f} },
			//base chimenea
		{ {0.135f,0.2f, 0.0f } , {0.9f,   0.0f, 0.3f} },
		{ {0.22f, 0.2f, 0.0f} , {0.9f,   0.0f, 0.3f} },
		{ {0.22f, 0.3f,0.0f} , {0.9f,   0.0f, 0.3f} },
		{ {0.135f, 0.2f,0.0f} , {0.9f,   0.0f, 0.3f} },
		{ {0.22f, 0.3f, 0.0f} , {0.9f,   0.0f, 0.3f} },
		{ {0.135f,0.3f, 0.0f } , {0.9f,   0.0f, 0.3f} },
		//gorrito de la chimenea
	{ {0.1f,0.3f, 0.0f } , {1.0f,   0.0f, 0.0f} },
	{ {0.25f, 0.3f, 0.0f} , {1.0f,   0.0f, 0.0f} },
	{ {0.25f, 0.37f,0.0f} , {1.0f,   0.0f, 0.0f} },
	{ {0.1f, 0.3f,0.0f} , {1.0f,   0.0f, 0.0f} },
	{ {0.25f, 0.37f, 0.0f} , {1.0f,   0.0f, 0.0f} },
	{ {0.1f,0.37f, 0.0f } , {1.0f,   0.0f, 0.0f} },
	//triangulo base
{ {0.135f,0.2f, 0.0f } , {0.9f,   0.0f, 0.3f} },
{ {0.22f, 0.2f, 0.0f} , {0.9f,   0.0f, 0.3f} },
{ {0.22f, 0.08f,0.0f} , {0.9f,   0.0f, 0.3f} },
//linea derecha de la cerca
{ {0.3f,-0.72f,0.0f},{0.6f,   0.0f, 0.0f} },
{ {0.7f,-0.72f,0.0f},{0.6f,   0.0f, 0.0f} },
{ {0.7f, -0.74f,0.0f},{0.6f,   0.0f, 0.0f} },
{ {0.3f, -0.72f, 0.0f},{0.6f,   0.0f, 0.0f} },
{ {0.7f, -0.74,0.0f},{0.6f,   0.0f, 0.0f} },
{ {0.3f, -0.74f, 0.0f},{0.6f,   0.0f, 0.0f} },
//linea cerca izquierda
{ {-0.3f,-0.72f,0.0f},{0.6f,   0.0f, 0.0f} },
{ {-0.7f,-0.72f,0.0f},{0.6f,   0.0f, 0.0f} },
{ {-0.7f, -0.74f,0.0f},{0.6f,   0.0f, 0.0f} },
{ {-0.3f, -0.72f, 0.0f},{0.6f,   0.0f, 0.0f} },
{ {-0.7f, -0.74f,0.0f},{0.6f,   0.0f, 0.0f} },
{ {-0.3f, -0.74f, 0.0f},{0.6f,   0.0f, 0.0f} },
//rectangulo de una cerca
{ {0.33f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {0.36f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.36f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.33f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.36f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.33f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {0.33f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.36f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.345f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },

/*********************************CERCA******************************************/
{ {0.39f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {0.42f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.42f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.39f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.42f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.39f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {0.39f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.42f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.405f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },
/******************************/
{ {0.45f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {0.48f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.48f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.45f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.48f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.45f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {0.45f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.48f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.465f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },
/********************************************/
{ {0.51f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {0.54f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.54f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.51f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.54f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.51f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {0.51f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.54f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.525f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },
/*********************************************/
{ {0.57f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {0.60f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.60f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.57f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.60f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {0.57f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {0.57f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.60f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {0.585f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },
//cerca izquierda
		//rectangulo de una cerca
{ {-0.33f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {-0.36f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.36f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.33f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.36f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.33f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {-0.33f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.36f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.345f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },/**/


{ {-0.39f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {-0.42f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.42f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.39f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.42f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.39f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {-0.39f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.42f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.405f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },
/******************************/
{ {-0.45f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {-0.48f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.48f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.45f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.48f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.45f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {-0.45f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.48f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.465f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },
/********************************************/
{ {-0.51f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {-0.54f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.54f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.51f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.54f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.51f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {-0.51f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.54f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.525f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },
/*********************************************/
{ {-0.57f,-0.92f, 0.0f } , {1.0f,   0.6f, 0.5f} },
{ {-0.60f, -0.92f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.60f, -0.70f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.57f, -0.92f,0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.60f, -0.7f, 0.0f} , {1.0f,   0.6f, 0.5f} },
{ {-0.57f, -0.7f, 0.0f } , {1.0f,   0.6f, 0.5f} },
//triangulo sobre la cerca
{ {-0.57f, -0.7f, 0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.60f, -0.7f,0.0f},{1.0f,   0.6f, 0.5f} },
{ {-0.585f, -0.65f, 0.0f},{1.0f,   0.6f, 0.5f} },

	//Creando perilla

{ {-0.105f, -0.6f, 0.0f } , {0.0f,   0.0f, 0.0f} },
{ {-0.09f, -0.6f, 0.0f } , {0.0f,   0.0f, 0.0f} },
{ {-0.09f, -0.625f, 0.0f } , {0.0f,   0.0f, 0.0f} },
{ {-0.105f, -0.6f, 0.0f } , {0.0f,   0.0f, 0.0f} },
	{ {-0.09f, -0.625f, 0.0f }, { 0.0f,   0.0f, 0.0f } },
{ {-0.105f, -0.625f, 0.0f } , {0.0f,   0.0f, 0.0f} }

			
	};//18*4
	Vertex vertices2[] =
	{
		//norte
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { 0.0f, 0.45f, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { 0.09f, 0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.0f } },
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { 0.0f, 0.45f, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { -0.09f, 0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.0f } },
	
	{ { -0.09f, 0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.8f } },
	{ { 0.09f, 0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.8f } },
	{ { 0.0f, 0.6f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	//sur
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { 0.0f, -0.45f, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { 0.09f, -0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.0f } },
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { 0.0f, -0.45f, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { -0.09f, -0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.0f } },

	{ { -0.09f, -0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.8f } },
	{ { 0.09f, -0.45f, 0.0f } ,{ 0.9f, 0.9f, 0.8f} },
	{ { 0.0f, -0.6f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	//Este
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { 0.35f, 0.0, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { 0.35f, -0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.0f } },
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { 0.35f, 0.0f, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { 0.35f, 0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.0f } },

	{ { 0.35f, 0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.8f } },
	{ { 0.35f, -0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.8f } },
	{ { 0.45f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	//Oeste
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { -0.35f, 0.0f, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { -0.35f, -0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.0f } },
	{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	{ { -0.35f, 0.0f, 0.0f } ,{ 1.0f, 0.8f, 0.0f } },
	{ { -0.35f, 0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.0f} },

	{ { -0.35f, -0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.8f } },
	{ { -0.35f, 0.13f, 0.0f } ,{ 0.9f, 0.9f, 0.8f} },
	{ { -0.45f, 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },
	};

	/*ertex Cuadrobase[] =
	{

	};*/
	const size_t bufferSize = sizeof(vertices);//18*4 bytes
	const size_t vertexSize = sizeof(vertices[0]);//6*4
	const size_t rgbOffset = sizeof(vertices[0].XYZ);//3*4

	std::cout << "Buffer Size:" << bufferSize << std::endl;
	std::cout << "Vertex Size:" << vertexSize << std::endl;
	std::cout << "Buffer size:" << rgbOffset << std::endl;
	// Esto es para el primer triangulo
	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);// corresponden con los de aqui
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);//0 indica indice primer atributo, cuantos datos tiene 3 flotantes, siempre false para datos tal cual, y el tamaño del vertex size
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,// mismo tamaño, pero otro desplazamiento
		(GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Esto es para el segundo triangulo
	const size_t bufferSize2 = sizeof(vertices2);//18*4 bytes
	const size_t vertexSize2 = sizeof(vertices2[0]);//6*4
	const size_t rgbOffset2 = sizeof(vertices2[0].XYZ);//3*4

	std::cout << "Buffer Size:" << bufferSize2 << std::endl;
	std::cout << "Vertex Size:" << vertexSize2 << std::endl;
	std::cout << "Buffer size:" << rgbOffset2 << std::endl;
	glGenBuffers(1, &VBO2);

	glGenVertexArrays(1, &VAO2);// corresponden con los de aqui
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, bufferSize2, vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize2, 0);//0 indica indice primer atributo, cuantos datos tiene 3 flotantes, siempre false para datos tal cual, y el tamaño del vertex size
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize2,// mismo tamaño, pero otro desplazamiento
		(GLvoid*)rgbOffset2);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();
	glUseProgram(0);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

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
		case GLFW_KEY_C:
			render1 = true;
			break;
		case GLFW_KEY_E:
			render1 = false;
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

bool processInput(bool continueApplication){
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(1.0f, 1.0f, 0.8f, 0.0f);
		
		glUseProgram(shaderProgram);
		if (render1)
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 216);
			//si queremos dibujar más triangulos debemos cambiar el 3, por el numero de vertices.
			
		}
		else {

			glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
			glBindVertexArray(VAO2);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(1100, 650, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

