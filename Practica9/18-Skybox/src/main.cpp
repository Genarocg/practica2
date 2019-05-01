//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

//glfw include
#include <GLFW/glfw3.h>

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"
// Geometry primitives
#include "Headers/sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
//Texture includes
#include "Headers/Texture.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere(20, 20);
Sphere sphere2(20, 20);

Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);

bool render1 = true, render2 = false, render3 = false;
Box box;

Shader shader;
Shader shaderColor;
Shader shaderTexture;
Shader shaderCubeTexture;
Shader shaderMateriales;
Shader shaderDirectionLight;
Shader shaderPointLight;
Shader shaderSpotLight;
Shader shader_cambio = shaderSpotLight;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6,
textureID7, textureID8, textureID9, textureID10, textureID11, textureID12, textureID13, textureID14, textureID15, textureID16,
textureID17, textureID18, textureID19, textureID20, textureID21, textureID22, textureID23, textureCubeTexture;
GLuint cubeTextureID;

GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

std::string fileNames[6] = {
	"../../Textures/mp_rainforest/rainforest_ft.tga",
	"../../Textures/mp_rainforest/rainforest_bk.tga",
	"../../Textures/mp_rainforest/rainforest_up.tga",
	"../../Textures/mp_rainforest/rainforest_dn.tga",
	"../../Textures/mp_rainforest/rainforest_rt.tga",
	"../../Textures/mp_rainforest/rainforest_lf.tga"
};

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX, offsetX;
int lastMousePosY, offsetY;

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
void animate(void);




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

	shaderColor.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");
	shaderTexture.initialize("../../Shaders/texturizado_res.vs", "../../Shaders/texturizado_res.fs");
	shaderCubeTexture.initialize("../../Shaders/cubeTexture.vs", "../../Shaders/cubeTexture.fs");
	shaderMateriales.initialize("../../Shaders/iluminacion_materiales_res.vs", "../../Shaders/iluminacion_materiales_res.fs");
	shaderDirectionLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/directionalLight.fs");
	shaderPointLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/pointLight.fs");
	shaderSpotLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/spotLight.fs");
	sphere2.init();

	sphere.init();
	cylinder.init();


	box.init();
	box.setShader(&shaderTexture);
	box.setColor(glm::vec3(0.2, 0.8, 0.4));

	camera->setPosition(glm::vec3(0.0f, 0.0f, -0.0f));
	//Creacion de la casita
	// base mesa
	int imageWidth, imageHeight;
	Texture texture("../../Textures/base_mesa.jpg");
	FIBITMAP* bitmap = texture.loadImage(false);
	unsigned char * data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID1);
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);
	//mesa tapiz
	texture = Texture("../../Textures/mesa.png");
	bitmap = texture.loadImage(true);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID9);
	glBindTexture(GL_TEXTURE_2D, textureID9);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);
	//base sillon
	texture = Texture("../../Textures/asiento_sillon.jpg");
	bitmap = texture.loadImage(true);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID10);
	glBindTexture(GL_TEXTURE_2D, textureID10);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture piso casa
	texture = Texture("../../Textures/piso.jpg");
	bitmap = texture.loadImage(true);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID2);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	// Texture pared interna
	texture = Texture("../../Textures/interior.jpg");
	bitmap = texture.loadImage(true);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID4);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//cuarto interno

	// Texture pared interior
	texture = Texture("../../Textures/pared_a.jpg");
	bitmap = texture.loadImage(true);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID5);
	glBindTexture(GL_TEXTURE_2D, textureID5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture puerta
	texture = Texture("../../Textures/puerta.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID6);
	glBindTexture(GL_TEXTURE_2D, textureID6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture base mueble
	texture = Texture("../../Textures/base_mueble.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID11);
	glBindTexture(GL_TEXTURE_2D, textureID11);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);
	// Texture bocina
	texture = Texture("../../Textures/bocina.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID12);
	glBindTexture(GL_TEXTURE_2D, textureID12);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);
	// Texture caja bocina
	texture = Texture("../../Textures/caja.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID13);
	glBindTexture(GL_TEXTURE_2D, textureID13);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture tapa bocina
	texture = Texture("../../Textures/negro.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID14);
	glBindTexture(GL_TEXTURE_2D, textureID14);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture cara stereo
	texture = Texture("../../Textures/stereo.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID15);
	glBindTexture(GL_TEXTURE_2D, textureID15);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture cara television
	texture = Texture("../../Textures/tele.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID16);
	glBindTexture(GL_TEXTURE_2D, textureID16);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture planta 
	texture = Texture("../../Textures/planta.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID17);
	glBindTexture(GL_TEXTURE_2D, textureID17);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);
	// Texture planta 
	texture = Texture("../../Textures/tapa_planta.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID18);
	glBindTexture(GL_TEXTURE_2D, textureID18);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture cuadro
	texture = Texture("../../Textures/cuadro.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID19);
	glBindTexture(GL_TEXTURE_2D, textureID19);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	// Texture bote
	texture = Texture("../../Textures/bote.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID20);
	glBindTexture(GL_TEXTURE_2D, textureID20);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);
	// Texture cocina
	texture = Texture("../../Textures/cocina.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID21);
	glBindTexture(GL_TEXTURE_2D, textureID21);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture pared externa
	texture = Texture("../../Textures/pared.jpg");
	bitmap = texture.loadImage(true);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID22);
	glBindTexture(GL_TEXTURE_2D, textureID22);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);
	// Texture porton
	texture = Texture("../../Textures/porton.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID23);
	glBindTexture(GL_TEXTURE_2D, textureID23);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	/******************/



	// Textura cuadritos
	texture = Texture("../../Textures/test.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID3);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	glGenTextures(1, &cubeTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		texture = Texture(fileNames[i]);
		FIBITMAP* bitmap = texture.loadImage(true);
		data = texture.convertToData(bitmap, imageWidth, imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		texture.freeImage(bitmap);
	}

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shaderColor.destroy();
	shaderTexture.destroy();
	shaderCubeTexture.destroy();
	shaderMateriales.destroy();
	shaderDirectionLight.destroy();
	shaderPointLight.destroy();
	shaderSpotLight.destroy();
	sphere.destroy();
	cylinder.destroy();
	box.destroy();
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
			render1 = true;
			render2 = false;
			render3 = false;
			break;
		case GLFW_KEY_2:
			render1 = false;
			render2 = true;
			render3 = false;
			break;
		case GLFW_KEY_3:
			render1 = false;
			render2 = false;
			render3 = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
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
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	glm::vec3 objPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	float angle = 0.0;
	float ratio = 5.0;


	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Matrix de proyeccion en perspectiva
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		// matrix de vista
		glm::mat4 view = camera->getViewMatrix();

		glm::mat4 cubeModelMatrix = glm::translate(glm::mat4(1.0f), objPosition);
		//piso
		glm::mat4 matrix0 = glm::mat4(1.0f);//matriz con diagonal unitaria

		matrix0 = glm::translate(matrix0, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 matrix1 = glm::translate(matrix0, glm::vec3(0.0f, 21.4f, 0.0f));
		glm::mat4 matrix2_1 = glm::translate(matrix0, glm::vec3(-4.995f, 21.4f, 0.0f));
		glm::mat4 sol = glm::translate(matrix0, glm::vec3(0.0f, 8.0f, -30.0f));
		glm::mat4 matrix2_2 = glm::translate(matrix0, glm::vec3(4.995f, 21.4f, 0.0f));
		glm::mat4 matrix2_3 = glm::translate(matrix0, glm::vec3(0.0f, 21.4f, -34.995f));
		glm::mat4 matrix3 = glm::translate(matrix0, glm::vec3(-3.0f, 21.4f, 0.0f));
		glm::mat4 matrix4 = glm::translate(matrix0, glm::vec3(-3.0f, 21.4f, -25.0f));
		glm::mat4 matrix5 = glm::translate(matrix0, glm::vec3(3.0f, 21.4f, 0.0f));
		glm::mat4 matrix6 = glm::translate(matrix0, glm::vec3(3.0f, 21.4f, -25.0f));
		glm::mat4 matrix7 = glm::translate(matrix0, glm::vec3(0.0f, 21.4f, 0.0f));
		glm::mat4 matrix8 = glm::translate(matrix0, glm::vec3(0.0f, -0.3f, -30.0f));
		glm::mat4 matrix9 = glm::translate(matrix8, glm::vec3(0.0f, 0.7f, 3.0f));
		glm::mat4 matrix10 = glm::translate(matrix0, glm::vec3(-4.3f, 4.0f, -20.0f));
		glm::mat4 matrix11 = glm::translate(matrix0, glm::vec3(-4.7f, 8.0f, -20.0f));
		glm::mat4 matrix12 = glm::translate(matrix0, glm::vec3(-2.0f, 4.0f, -31.0f));
		glm::mat4 matrix13 = glm::translate(matrix0, glm::vec3(-2.0f, 8.0f, -33.6f));
		glm::mat4 matrix14 = glm::translate(matrix0, glm::vec3(4.0f, 4.0f, -24.5f));
		glm::mat4 matrix15 = glm::translate(matrix0, glm::vec3(4.0f, 3.7f, -29.7f));
		glm::mat4 matrix16 = glm::translate(matrix0, glm::vec3(4.0f, 3.7f, -29.0f));
		glm::mat4 matrix17 = glm::translate(matrix0, glm::vec3(4.0f, 3.7f, -28.3f));
		glm::mat4 matrix18 = glm::translate(matrix0, glm::vec3(4.9f, 7.0f, -26.0f));
		glm::mat4 matrix19 = glm::translate(matrix0, glm::vec3(4.4f, 0.0f, -34.0f));
		glm::mat4 matrix20 = glm::translate(matrix0, glm::vec3(-4.8f, 7.0f, -24.0f));
		glm::mat4 matrix21 = glm::translate(matrix0, glm::vec3(-4.7f, 2.9f, -24.0f));
		glm::mat4 matrix22 = glm::translate(matrix0, glm::vec3(3.0f, 16.0f, -34.7f));
		glm::mat4 matrix23 = glm::translate(matrix0, glm::vec3(-10.0f, 0.0f, 55.0f));
		glm::mat4 matrix24 = glm::translate(matrix0, glm::vec3(-10.0f, 53.0f, 55.0f));
		glm::mat4 matrix25 = glm::translate(matrix0, glm::vec3(-5.0f, 53.0f, 55.05f));
		glm::mat4 matrix26 = glm::translate(matrix0, glm::vec3(-5.05f, 53.0f, 37.6f));
		glm::mat4 matrix27 = glm::translate(matrix0, glm::vec3(-10.0f, 53.0f, -35.05f));
		//piso cuarto detallado
		matrix0 = glm::scale(matrix0, glm::vec3(10.0f, 0.1f, 10.0f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		//box.render(matrix0);
		//piso demás cuartos
		matrix23 = glm::scale(matrix23, glm::vec3(30.0f, 0.1f, 30.0f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix23);
		//fachada externa
		matrix24 = glm::scale(matrix24, glm::vec3(30.005f, 10.0f, 30.0f));
		glBindTexture(GL_TEXTURE_2D, textureID22);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0,24, matrix24);
		//porton
		matrix25 = glm::scale(matrix25, glm::vec3(10.0f, 10.0f, 30.0f));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 6, matrix25);
		//separaciones
		matrix26 = glm::scale(matrix26, glm::vec3(0.05f, 10.0f, 25.0f));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix26);

		matrix27 = glm::scale(matrix27, glm::vec3(30.0f, 10.0f, 0.05f));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix27);

		//cuarto externo
		matrix1 = glm::scale(matrix1, glm::vec3(10.005f, 4.0f, 10.005f));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(6, 18, matrix1);

		///cuarto interno
		//por paredes
		//izquierda
		matrix2_1 = glm::scale(matrix2_1, glm::vec3(0.005f, 4.0f, 10.005f));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix2_1);
		//derecha
		matrix2_2 = glm::scale(matrix2_2, glm::vec3(0.005f, 4.0f, 10.005f));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix2_2);

		//trasera
		matrix2_3 = glm::scale(matrix2_3, glm::vec3(10.005f, 4.0f, 0.005f));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix2_3);

		//pared izquierda frontal exterior
		matrix3 = glm::scale(matrix3, glm::vec3(4.0f, 4.0f, 9.995f));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 6, matrix3);

		//pared izquierda frontal interior
		matrix4 = glm::scale(matrix4, glm::vec3(4.0f, 4.0f, 0.005f));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix4);

		//pared derecha frontal exterior
		matrix5 = glm::scale(matrix5, glm::vec3(4.0f, 4.0f, 9.995f));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 6, matrix5);
		//pared derecha frontal interior
		matrix6 = glm::scale(matrix6, glm::vec3(4.0f, 4.0f, 0.005f));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix6);

		//puerta frontal
		matrix7 = glm::scale(matrix7, glm::vec3(2.0f, 4.0f, 10.005f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 6, matrix7);
		//base de mesa
		matrix8 = glm::scale(matrix8, glm::vec3(0.3f, 0.3f, 0.3f));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.setShader(&shaderTexture);
		cylinder.render(matrix8);
		//mesa 
		matrix9 = glm::scale(matrix9, glm::vec3(1.0f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix9);

		//Sillon asiento
		matrix10 = glm::scale(matrix10, glm::vec3(1.0f, 0.8f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix10);
		//Sillon recargadera
		matrix11 = glm::scale(matrix11, glm::vec3(0.25f, 1.5f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix11);
		//Sillon asiento
		matrix12 = glm::scale(matrix12, glm::vec3(3.0f, 0.8f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix12);

		//Sillon recargadera
		matrix13 = glm::scale(matrix13, glm::vec3(3.0f, 1.5f, 0.25f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix13);
		//base mueble 
		matrix14 = glm::scale(matrix14, glm::vec3(1.0f, 0.8f, 2.0f));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(matrix14);
		//estereo izquierdo del mueble 
		matrix15 = glm::scale(matrix15, glm::vec3(0.333f, 0.6f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 18, matrix15);
		glBindTexture(GL_TEXTURE_2D, textureID12);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(18, 6, matrix15);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(24, 6, matrix15);

		//estereo centro del mueble 
		matrix16 = glm::scale(matrix16, glm::vec3(0.333f, 0.6f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 18, matrix16);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(24, 6, matrix16);
		glBindTexture(GL_TEXTURE_2D, textureID15);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(18, 6, matrix16);

		//estereo derecho del mueble 
		matrix17 = glm::scale(matrix17, glm::vec3(0.333f, 0.6f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 18, matrix17);
		glBindTexture(GL_TEXTURE_2D, textureID12);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(18, 6, matrix17);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(24, 6, matrix17);

		//television
		matrix18 = glm::scale(matrix18, glm::vec3(0.1f, 1.0f, 1.5f));
		glBindTexture(GL_TEXTURE_2D, textureID16);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(18, 6, matrix18);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 18, matrix18);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(24, 12, matrix18);
		//planta
		matrix19 = glm::scale(matrix19, glm::vec3(0.3f, 1.0f, 0.3f));
		glBindTexture(GL_TEXTURE_2D, textureID17);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.setShader(&shaderTexture);
		cylinder.render(0, cylinder.getSlices() * cylinder.getStacks() * 2 * 3, matrix19);
		glBindTexture(GL_TEXTURE_2D, textureID18);
		cylinder.render(cylinder.getSlices() * cylinder.getStacks() * 2 * 3, cylinder.getSlices() * 3, matrix19);

		//cuadro
		matrix20 = glm::scale(matrix20, glm::vec3(0.1f, 1.0f, 1.5f));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(6, 6, matrix20);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 6, matrix20);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(12, 24, matrix20);

		//bote de basura
		matrix21 = glm::scale(matrix21, glm::vec3(0.333f, 0.6f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 24, matrix21);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(30, 6, matrix21);

		//puerta copcina
		matrix22 = glm::scale(matrix22, glm::vec3(1.5f, 3.0f, 0.1f));
		glBindTexture(GL_TEXTURE_2D, textureID21);
		box.setProjectionMatrix(projection);//proyeccion
		box.setViewMatrix(view);//vista
		box.render(0, 6, matrix22);

		//menu de cambios de luz
		if (render1) {
			// Iluminación

			shaderDirectionLight.turnOn();
			//glUniform3fv(shaderSpotLight.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, -15.0)));
			glUniform3fv(shaderDirectionLight.getUniformLocation("light.direction"), 1, glm::value_ptr(glm::vec3(-1.0f, -30.0f, -1.0f)));
			//glUniform3fv(shaderSpotLight.getUniformLocation("light.position"), 1, glm::value_ptr(camera->getPosition()));
			//glUniform3fv(shaderSpotLight.getUniformLocation("light.direction"), 1, glm::value_ptr(camera->getFront()));
			//glUniform3fv(shaderDirectionLight.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3f(shaderDirectionLight.getUniformLocation("light.ambient"), 0.4, 0.4, 0.4);
			glUniform3f(shaderDirectionLight.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
			glUniform3f(shaderDirectionLight.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
			//glUniform3f(shaderDirectionLight.getUniformLocation("material.ambient"), 1.0, 0.2, 0.6);
			//glUniform3f(shaderDirectionLight.getUniformLocation("material.diffuse"), 0.4, 0.5, 0.8);
			//glUniform3f(shaderDirectionLight.getUniformLocation("material.specular"), 0.5, 0.3, 0.2);
			//glUniform1f(shaderSpotLight.getUniformLocation("light.cutOff"), glm::cos(glm::radians(11.0f)));
			//glUniform1f(shaderSpotLight.getUniformLocation("light.outerCutOff"), glm::cos(glm::radians(15.0f)));
			//glUniform1f(shaderSpotLight.getUniformLocation("light.constant"), 1.0f);
			//glUniform1f(shaderSpotLight.getUniformLocation("light.linear"), 0.7f);
			//glUniform1f(shaderSpotLight.getUniformLocation("light.quadratics"), 1.8f);
			//glUniform1f(shaderSpotLight.getUniformLocation("material.shininess"), 48.0);
			box.setShader(&shaderDirectionLight);
			cylinder.setShader(&shaderDirectionLight);
			box.setPosition(glm::vec3(0.0f, -5.0f, -3.0f));
			angle += 0.5;
			sol = glm::rotate(sol, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
			sol = glm::scale(sol, glm::vec3(1.0f, 1.0f, 1.0f));
			sphere.setShader(&shaderColor);
			sphere.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
			sphere.setProjectionMatrix(projection);
			sphere.setViewMatrix(view);
			sphere.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
			sphere.enableWireMode();
			sphere.render(sol);
			shaderDirectionLight.turnOff();


		}
		else if (render2)
		{
			// Iluminación

			shaderPointLight.turnOn();
			//glUniform3fv(shaderMateriales.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(lightModelmatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))));
			//glUniform3fv(shaderMateriales.getUniformLocation("light.direction"), 1, glm::value_ptr(glm::vec3(-0.3f, -1.0f, -0.2f)));
			glUniform3fv(shaderPointLight.getUniformLocation("light.position"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3fv(shaderPointLight.getUniformLocation("light.direction"), 1, glm::value_ptr(camera->getFront()));
			glUniform3fv(shaderPointLight.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3f(shaderPointLight.getUniformLocation("light.ambient"), 0.2, 0.2, 0.2);
			glUniform3f(shaderPointLight.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
			glUniform3f(shaderPointLight.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
			glUniform3f(shaderPointLight.getUniformLocation("material.ambient"), 1.0, 0.2, 0.6);
			glUniform3f(shaderPointLight.getUniformLocation("material.diffuse"), 0.4, 0.5, 0.8);
			glUniform3f(shaderPointLight.getUniformLocation("material.specular"), 0.5, 0.3, 0.2);
			//glUniform1f(shaderSpotLight.getUniformLocation("light.cutOff"), glm::cos(glm::radians(11.0f)));
			//glUniform1f(shaderSpotLight.getUniformLocation("light.outerCutOff"), glm::cos(glm::radians(15.0f)));
			glUniform1f(shaderPointLight.getUniformLocation("light.constant"), 1.0f);
			glUniform1f(shaderPointLight.getUniformLocation("light.linear"), 0.0014f);
			glUniform1f(shaderPointLight.getUniformLocation("light.quadratics"), 0.000007f);
			glUniform1f(shaderPointLight.getUniformLocation("material.shininess"), 48.0);
			box.setShader(&shaderPointLight);
			cylinder.setShader(&shaderPointLight);
			box.setPosition(glm::vec3(0.0f, -5.0f, -3.0f));
			shaderPointLight.turnOff();
		}
		else if (render3) {
			// Iluminación

			shaderSpotLight.turnOn();
			/*glUniform3fv(shaderSpotLight.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, -15.0)));
			glUniform3fv(shaderSpotLight.getUniformLocation("light.direction"), 1, glm::value_ptr(glm::vec3(-0.3f, -1.0f, -0.2f)));*/
			glUniform3fv(shaderSpotLight.getUniformLocation("light.position"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3fv(shaderSpotLight.getUniformLocation("light.direction"), 1, glm::value_ptr(camera->getFront()));
			glUniform3fv(shaderSpotLight.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3f(shaderSpotLight.getUniformLocation("light.ambient"), 0.2, 0.2, 0.2);
			glUniform3f(shaderSpotLight.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
			glUniform3f(shaderSpotLight.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
			glUniform3f(shaderSpotLight.getUniformLocation("material.ambient"), 1.0, 0.2, 0.6);
			glUniform3f(shaderSpotLight.getUniformLocation("material.diffuse"), 0.4, 0.5, 0.8);
			glUniform3f(shaderSpotLight.getUniformLocation("material.specular"), 0.5, 0.3, 0.2);
			glUniform1f(shaderSpotLight.getUniformLocation("light.cutOff"), glm::cos(glm::radians(11.0f)));
			glUniform1f(shaderSpotLight.getUniformLocation("light.outerCutOff"), glm::cos(glm::radians(15.0f)));
			glUniform1f(shaderSpotLight.getUniformLocation("light.constant"), 1.0f);
			glUniform1f(shaderSpotLight.getUniformLocation("light.linear"), 0.7f);
			glUniform1f(shaderSpotLight.getUniformLocation("light.quadratics"), 1.8f);
			glUniform1f(shaderSpotLight.getUniformLocation("material.shininess"), 48.0);
			box.setShader(&shaderSpotLight);
			cylinder.setShader(&shaderSpotLight);
			box.setPosition(glm::vec3(0.0f, -5.0f, -3.0f));
			shaderSpotLight.turnOff();
		}
		glBindTexture(GL_TEXTURE_2D, 0);




		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderCubeTexture.turnOn();
		GLuint cubeTextureId = shaderCubeTexture.getUniformLocation("skybox");
		glUniform1f(cubeTextureId, 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		sphere.setShader(&shaderCubeTexture);
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(glm::mat4(glm::mat3(view)));
		sphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));
		sphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		shaderCubeTexture.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
