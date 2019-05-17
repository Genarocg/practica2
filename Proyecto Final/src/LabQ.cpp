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
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
//Texture includes
#include "Headers/Texture.h"
//Model includes
#include "Headers/Model.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Box box;
Box box2;
Box box3;
Box box4;
Box box5;
Box box6;
Box box7;

Box box01;
Box box012;
Box box013;
Box box014;

Box boxWater;
//Para el cambio de camara
bool vistafront = false;
float vx = 10.0f, vy = 0.0f, vz = 20.0f;



Shader shaderColor;
Shader shaderTexture;
Shader shaderCubeTexture;
Shader shaderMateriales;
Shader shaderDirectionLight;
Shader shaderPointLight;
Shader shaderSpotLight;
//shader que tiene multiples luces
Shader shaderLighting;
Model arbol;
Model arbol2;
Model avion;
Model modelsilla;
Model modelmonitor;
Model modelproyector;
Model modelac;
Model modelmueble;
Model modeldesk;
Model modelmouse;
Model pajaro2;
Model avion2;
Model perro;
Model audi;
Model persona;
Model modelmariposa;




//Aqui declaramos los identificadores de las Texturas
GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7,
textureID8, textureID9, textureID10, textureID11, textureID12, textureID13, textureID14, textureID15,
textureID16, textureID17, textureID18, textureID19, textureID20, textureID21, textureID22, textureID23,
textureID24, textureID25, textureID26, textureID27, textureID28, textureID29, textureID30,
textureID31, textureID32, textureID33, textureID34, textureID35, textureID36,textureID71, textureID72, textureID73, textureID74, textureID75, textureID76, textureID715, textureID716,
textureID717, textureID718, textureID719, textureID7110, textureID7111, textureID7112, textureID7113, textureID7114,
textureID7115, textureID7116, textureID7117, textureID7118, textureID7119, textureID7120, textureID7121,
texturePisoID, textureWaterID,texturehojas, textureCubeTexture;
GLuint cubeTextureID;

GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};
//Archivos para el skybox
std::string fileNames[6] = { 
	"../Textures/ely_cloudtop/cloudtop_ft.tga",
	"../Textures/ely_cloudtop/cloudtop_bk.tga",
	"../Textures/ely_cloudtop/cloudtop_up.tga",
	"../Textures/ely_cloudtop/cloudtop_dn.tga",
	"../Textures/ely_cloudtop/cloudtop_rt.tga",
	"../Textures/ely_cloudtop/cloudtop_lf.tga"
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

//leer el movimiento

std::vector<std::vector<glm::mat4>> getKeyFrames(std::string fileName) { //Vector de vectores en donde se guardan los frames
	std::vector<std::vector<glm::mat4>> keyFrames;
	std::string line;
	std::ifstream infile(fileName);
	std::string s = line;
	while (std::getline(infile, line))
	{
		std::vector<glm::mat4> transforms;
		s = line;
		size_t pos1 = 0;
		std::string token1;
		std::string delimiter1 = "|";
		while ((pos1 = s.find(delimiter1)) != std::string::npos) {
			token1 = s.substr(0, pos1);
			//std::cout << token1 << std::endl;

			size_t pos2 = 0;
			std::string token2;
			std::string delimiter2 = ",";

			int i = 0;
			int j = 0;
			glm::mat4 transform;
			float aaa[16];
			while ((pos2 = token1.find(delimiter2)) != std::string::npos) {
				token2 = token1.substr(0, pos2);
				aaa[i++] = atof(token2.c_str());
				/*if (j == 0)
				transform[i].x = atof(token2.c_str());
				if (j == 1)
				transform[i].y = atof(token2.c_str());
				if (j == 2)
				transform[i].z = atof(token2.c_str());
				if (j == 3)
				transform[i].w = atof(token2.c_str());*/
				/*j++;
				if (j > 3) {
				i++;
				j = 0;
				}*/
				token1.erase(0, pos2 + delimiter2.length());
			}
			aaa[i++] = atof(token1.c_str());
			transform = glm::make_mat4(aaa);
			transforms.push_back(transform);
			s.erase(0, pos1 + delimiter1.length());
		}
		keyFrames.push_back(transforms);
	}
	return keyFrames;
}



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

	shaderColor.initialize("../Shaders/transformaciones.vs", "../Shaders/transformaciones.fs");
	shaderTexture.initialize("../Shaders/texturizado_res.vs", "../Shaders/texturizado_res.fs");
	shaderCubeTexture.initialize("../Shaders/cubeTexture.vs", "../Shaders/cubeTexture.fs");
	shaderMateriales.initialize("../Shaders/iluminacion_materiales_res.vs", "../Shaders/iluminacion_materiales_res.fs");
	shaderDirectionLight.initialize("../Shaders/typeLight.vs", "../Shaders/directionalLight.fs");
	shaderPointLight.initialize("../Shaders/typeLight.vs", "../Shaders/pointLight.fs");
	shaderSpotLight.initialize("../Shaders/typeLight.vs", "../Shaders/spotLight.fs");
	shaderLighting.initialize("../Shaders/typeLight.vs", "../Shaders/multipleLights.fs");

	//siempre que declares una nueva caja a la que tendremos diferente numero de texturas, iniciar su constructor
	sphere.init();
	cylinder.init();
	box.init();
	box2.init();
	box3.init();
	box4.init();
	box5.init();
	box6.init();
	box7.init();

	box01.init();
	box012.init();
	box013.init();
	box014.init();
	boxWater.init();
	//se escala las coordenadas de textura, es decir, cuantas veces se repite la textura/**********************************/
	box.scaleUVS(glm::vec3(20.0f, 20.0f, 20.0f));
	box2.scaleUVS(glm::vec3(6.0f, 1.0f, 6.0f));
	box3.scaleUVS(glm::vec3(1.0f, 1.0f, 1.0f));
	box4.scaleUVS(glm::vec3(4.0f, 1.0f, 1.0f));
	box5.scaleUVS(glm::vec3(2.0f, 1.0f, 2.0f));
	box6.scaleUVS(glm::vec3(1.0f, 1.0f, 6.0f));
	box7.scaleUVS(glm::vec3(10.0f, 1.0f, 10.0f));
	boxWater.scaleUVS(glm::vec3(4.0f, 3.0f, 4.0f));
	arbol.loadModel("../models/arbol/Tree.obj");
	arbol2.loadModel("../models/arbol2/Tree.obj");
	modelsilla.loadModel("../models/sillas/Vitra03_OBJ.obj");
	modelmonitor.loadModel("../models/monitor/dell/LCD TV.obj");
	modelproyector.loadModel("../models/proyector/Optoma/Proektor_(0).obj");
	modelac.loadModel("../models/ac/AirConditioner.obj");
	modelmueble.loadModel("../models/mueble/Obj format/Free model Drawer(Final) .obj");
	modeldesk.loadModel("../models/teclado/teclado/10105_Computer Keyboard_v1_L3.obj");
	modelmouse.loadModel("../models/mouse/MouseThermaltakeBlack.obj");
	avion.loadModel("../models/Heli/10621_CoastGuardHelicopter.obj");
	pajaro2.loadModel("../models/ave/12214_Bird_v1max_l3.obj");
	avion2.loadModel("../models/avion/11803_Airplane_v1_l1.obj");
	perro.loadModel("../models/dog/12228_Dog_v1_L2.obj");
	audi.loadModel("../models/auto/12353_Automobile_V1_L2.obj");
	persona.loadModel("../models/person/UrbanPoliceOfficer.obj");
	modelmariposa.loadModel("../models/Eastern_Tiger_Swallowtail_Butterfly_v2_L194114d7-634e-4570-8782-fa4c92de274e/19920_Eastern_Tiger_Swallowtail_Butterfly_v2_NEW.obj");
		camera->setPosition(glm::vec3(10.0f, 15.0f, 15.0f));

	// Textura Piso
	int imageWidth, imageHeight;
	Texture texture = Texture("../Textures/Piso_2.jpg");
	FIBITMAP* bitmap = texture.loadImage(false);
	unsigned char * data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &texturePisoID);
	glBindTexture(GL_TEXTURE_2D, texturePisoID);
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
	//****copiar desde aqui para declarar una textura*//////////////////////
	texture = Texture("../Textures/detalle.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
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

	/**********************Hasta aqui, y solo cambiarle los nombres del archivo, y del identificador*/
	//Ejemplo

	texture = Texture("../Textures/rayas.jpg");
	bitmap = texture.loadImage(false);
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

	//textura salon
	texture = Texture("../Textures/puertab.jpg");
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
	//agregas

	//textura techo
	texture = Texture("../Textures/techo.jpg");
	bitmap = texture.loadImage(false);
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

	//textura placa
	texture = Texture("../Textures/q.jpg");
	bitmap = texture.loadImage(false);
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

	//textura bordes
	texture = Texture("../Textures/detalle.jpg");
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


	//textura ventana
	texture = Texture("../Textures/ventana.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7);
	glBindTexture(GL_TEXTURE_2D, textureID7);
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

	//textura techo rojo
	texture = Texture("../Textures/techor.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID8);
	glBindTexture(GL_TEXTURE_2D, textureID8);
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

	//textura ventana frontal
	texture = Texture("../Textures/ventana2.png");
	bitmap = texture.loadImage(false);
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

	//textura ventana entrada
	texture = Texture("../Textures/entrada_c.png");
	bitmap = texture.loadImage(false);
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

	//textura cartel central
	texture = Texture("../Textures/cartel_c.jpg");
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

	//textura cartel izquierdo
	texture = Texture("../Textures/cartel_i.jpg");
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
	//textura edif der a
	texture = Texture("../Textures/edif_a.jpg");
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

	//textura edif der b
	texture = Texture("../Textures/edif_b.jpg");
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

	//textura edif der c
	texture = Texture("../Textures/edif_c.jpg");
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

	//textura edif izquierdo puerta al centro bajo
	texture = Texture("../Textures/puerta_c.jpg");
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
	//textura edif izquierdo ventana
	texture = Texture("../Textures/Pared_i.jpg");
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


	//textura cara de estatua
	texture = Texture("../Textures/escultura.png");
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
	//textura tierra
	texture = Texture("../Textures/tierra.jpg");
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

	//textura piedra
	texture = Texture("../Textures/piedra.jpg");
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

	//textura figura azul
	texture = Texture("../Textures/fig1.png");
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

	//textura figura amarilla
	texture = Texture("../Textures/fig2.png");
	bitmap = texture.loadImage(false);
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

	//textura arbusto
	texture = Texture("../Textures/arbusto.png");
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

	//textura arbusto_b
	texture = Texture("../Textures/arbusto_b.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID24);
	glBindTexture(GL_TEXTURE_2D, textureID24);
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


	//textura maceta roja
	texture = Texture("../Textures/maceta_r.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID25);
	glBindTexture(GL_TEXTURE_2D, textureID25);
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

	//textura rampa a
	texture = Texture("../Textures/rampa.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID26);
	glBindTexture(GL_TEXTURE_2D, textureID26);
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

	//textura bebedero frontal
	texture = Texture("../Textures/bebederof.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID27);
	glBindTexture(GL_TEXTURE_2D, textureID27);
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
	//textura bebedero lateral
	texture = Texture("../Textures/bebederol.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID28);
	glBindTexture(GL_TEXTURE_2D, textureID28);
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

	//textura entrada laboratorios
	texture = Texture("../Textures/Puertag.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID29);
	glBindTexture(GL_TEXTURE_2D, textureID29);
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

	//textura entrada derecha laboratorios
	texture = Texture("../Textures/Puertag2.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID30);
	glBindTexture(GL_TEXTURE_2D, textureID30);
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

	//textura baños hombres
	texture = Texture("../Textures/Pared_in.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID31);
	glBindTexture(GL_TEXTURE_2D, textureID31);
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

	//textura piso interior
	texture = Texture("../Textures/Piso_i.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID32);
	glBindTexture(GL_TEXTURE_2D, textureID32);
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
	//textura pared baño 2
	texture = Texture("../Textures/Pared_in2.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID33);
	glBindTexture(GL_TEXTURE_2D, textureID33);
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
	//textura entrada trabajadores
	texture = Texture("../Textures/entrada_d.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID34);
	glBindTexture(GL_TEXTURE_2D, textureID34);
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
	//textura entrada lab izquierda
	texture = Texture("../Textures/entrada_i.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID35);
	glBindTexture(GL_TEXTURE_2D, textureID35);
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

	//salon ***************************************************

	texture = Texture("../Textures/madera.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID71);
	glBindTexture(GL_TEXTURE_2D, textureID71);
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


	texture = Texture("../Textures/pisol.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID72);
	glBindTexture(GL_TEXTURE_2D, textureID72);
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

	//textura techo
	texture = Texture("../Textures/pared1.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID74);
	glBindTexture(GL_TEXTURE_2D, textureID74);
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


	texture = Texture("../Textures/cpu1.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID75);
	glBindTexture(GL_TEXTURE_2D, textureID75);
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

	texture = Texture("../Textures/white.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID76);
	glBindTexture(GL_TEXTURE_2D, textureID76);
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

	texture = Texture("../Textures/pisol.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID715);
	glBindTexture(GL_TEXTURE_2D, textureID715);
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

	texture = Texture("../Textures/techo.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID716);
	glBindTexture(GL_TEXTURE_2D, textureID716);
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

	texture = Texture("../Textures/negro.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID717);
	glBindTexture(GL_TEXTURE_2D, textureID717);
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

	texture = Texture("../Textures/monitor1.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID718);
	glBindTexture(GL_TEXTURE_2D, textureID718);
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

	texture = Texture("../Textures/acero.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID719);
	glBindTexture(GL_TEXTURE_2D, textureID719);
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

	texture = Texture("../Textures/metal.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7110);
	glBindTexture(GL_TEXTURE_2D, textureID7110);
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


	texture = Texture("../Textures/botiquin1.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7111);
	glBindTexture(GL_TEXTURE_2D, textureID7111);
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

	texture = Texture("../Textures/botiquins.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7112);
	glBindTexture(GL_TEXTURE_2D, textureID7112);
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

	texture = Texture("../Textures/riesgo.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7113);
	glBindTexture(GL_TEXTURE_2D, textureID7113);
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


	texture = Texture("../Textures/vidrio.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7114);
	glBindTexture(GL_TEXTURE_2D, textureID7114);
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


	texture = Texture("../Textures/cemento.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7115);
	glBindTexture(GL_TEXTURE_2D, textureID7115);
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



	texture = Texture("../Textures/cortinas.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7116);
	glBindTexture(GL_TEXTURE_2D, textureID7116);
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

	texture = Texture("../Textures/tele.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7117);
	glBindTexture(GL_TEXTURE_2D, textureID7117);
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

	texture = Texture("../Textures/ac.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7118);
	glBindTexture(GL_TEXTURE_2D, textureID7118);
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

	texture = Texture("../Textures/muebles.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7119);
	glBindTexture(GL_TEXTURE_2D, textureID7119);
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

	texture = Texture("../Textures/lamp.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7120);
	glBindTexture(GL_TEXTURE_2D, textureID7120);
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

	texture = Texture("../Textures/regla.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7121);
	glBindTexture(GL_TEXTURE_2D, textureID7121);
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

	//textura hojas cayendo
	texture = Texture("../Textures/hojas.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &texturehojas);
	glBindTexture(GL_TEXTURE_2D, texturehojas);
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
	/***************************************fin ejemplo****************/
/***************************************fin ejemplo****************/

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
	//Tambien colocar su destructor de la nueva caja si utilizaras una nueva que ya hayas declarado su constructor
	box2.destroy();
	box3.destroy();

	//salon

	box01.destroy();
	box012.destroy();
	box013.destroy();
	box014.destroy();
	boxWater.destroy();
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
			camera->setPosition(glm::vec3(12.0f, 4.0f, 20.0f));
			break;
		case GLFW_KEY_2:
			camera->setPosition(glm::vec3(12.0f, 20.0f, 28.0f));
			break;
		case GLFW_KEY_3:
			camera->setPosition(glm::vec3(20.0f, 2.0f, -19.0f));
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
	float angle = 0.0;
	float ratio = 20.0;
	float movX = -20.0;
	float radio = 20.0;
	float posX = 0.0;
	float movZ = sqrt(pow(radio,2) - pow(movX,2));
	float velocidad = 0.0;
	float rotavion = 0.0;
	bool inicio = true,
		rotacionoff = false;
	/****************************Variables para animacion****************************************/
	float aircraftZ = 0.0;
	float pokemonZ = 0.0;
	float pokemonX = 0.0;
	float rotationAirCraft = 0.0;
	float rotationPokemon = 0.0;
	bool finishRotation = true;
	bool direcionAirCraft = true;
	bool finalRotacion = true;
	bool direccionPokemonZ = true;
	bool direccionPokemonX = true;
	float pajaroZ = 0.0;
	float pajaroY = 0.0;
	float perroX = 0.0;
	float perroZ = 0.0;
	float avion2X = 0.0;
	float avion2Y = 0.0;
	float Vini = 10.0;
	float ang = 60.0 * 3.1416 / 180;
	float g = 9.81;
	float t = 0.0;
	float temp = 0.0f;
	int bandera = 0;
	int banderaPerro = 0;
	float rotacionavion = 0.0;
	float rotationPajaro = 0.0;
	float rotacionPerro = 0.0;
	bool finishRotation3 = true;
	bool finishRotation2 = true;
	bool finishRotation4 = true;
	bool direcionavion = true;
	bool direcionPajaro = true;
	bool direcionPerro = true;
	bool direcionPerroX = true;
	//Se obtienen los frames del brazo
	std::vector<std::vector<glm::mat4>> keyFramesBrazo = getKeyFrames("../animaciones/animationMano.txt");
	//Numero de pasos para alcanzar  del frame i-1 al frame i
	int numPasosAnimBrazo = 200;
	int numPasosAnimBrazoCurr = 0;

	//Indices del arreglo keyframeBrazo el actual y el siguiente
	int indexKeyFrameBrazoCurr = 0;
	int indexKeyFrameBrazoNext = 1;
	float interpolation = 0.0;
	/*************************************************************************/
	float xm = 0.0f;
	float ym = 0.0f;
	float tm = 0.0f;
	bool a = true;


	while (psi) {
		psi = processInput(true);
		//variablespara la animacion


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Matrix de proyeccion en perspectiva
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		// matrix de vista
		glm::mat4 view = camera->getViewMatrix();



			// Iluminación
		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix, glm::vec3(0.0f, 0.0f, -ratio));

		shaderMateriales.turnOn();
		glUniform3fv(shaderMateriales.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(lightModelmatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))));
		glUniform3fv(shaderMateriales.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
		glUniform3f(shaderMateriales.getUniformLocation("light.ambient"), 0.2, 0.2, 0.2);
		glUniform3f(shaderMateriales.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
		glUniform3f(shaderMateriales.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
		glUniform3f(shaderMateriales.getUniformLocation("material.ambient"), 1.0, 0.2, 0.6);
		glUniform3f(shaderMateriales.getUniformLocation("material.diffuse"), 0.4, 0.5, 0.8);
		glUniform3f(shaderMateriales.getUniformLocation("material.specular"), 0.5, 0.3, 0.2);
		glUniform1f(shaderMateriales.getUniformLocation("material.shininess"), 32.0);
		shaderMateriales.turnOff();
		shaderLighting.turnOn();
		glUniform3fv(shaderLighting.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
		//Directional light
		//se envian los valores de las componentes ambientales difusa y especular
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.ambient"), 0.025, 0.025, 0.025);
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.diffuse"), 0.2, 0.2, 0.2);
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.specular"), 0.15, 0.15, 0.15);
		glUniform3fv(shaderLighting.getUniformLocation("directionalLight.direction"), 1, glm::value_ptr(glm::vec3(0, -1.0, 0.0)));
		//Numero de luces spot y point
		int locCount = shaderLighting.getUniformLocation("pointLightCount");
		glUniform1i(shaderLighting.getUniformLocation("pointLightCount"), 0);
		glUniform1i(shaderLighting.getUniformLocation("spotLightCount"), 1);
		// Point light
		glUniform3fv(shaderLighting.getUniformLocation("pointLights[0].position"), 1, glm::value_ptr(glm::vec3(lightModelmatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))));
		glUniform1f(shaderLighting.getUniformLocation("pointLights[0].constant"), 1.0f);
		glUniform1f(shaderLighting.getUniformLocation("pointLights[0].linear"), 0.14f);
		glUniform1f(shaderLighting.getUniformLocation("pointLights[0].quadratics"), 0.07f);
		glUniform3f(shaderLighting.getUniformLocation("pointLights[0].light.ambient"), 0.025, 0.025, 0.025);
		glUniform3f(shaderLighting.getUniformLocation("pointLights[0].light.diffuse"), 0.2, 0.3, 0.15);
		glUniform3f(shaderLighting.getUniformLocation("pointLights[0].light.specular"), 0.5, 0.1, 0.2);
		// Spot light
		glUniform3fv(shaderLighting.getUniformLocation("spotLights[0].position"), 1, glm::value_ptr(camera->getPosition()));
		glUniform3fv(shaderLighting.getUniformLocation("spotLights[0].direction"), 1, glm::value_ptr(camera->getFront()));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].outerCutOff"), glm::cos(glm::radians(15.0f)));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].constant"), 1.0f);
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].linear"), 0.14f);
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].quadratics"), 0.07f);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.ambient"), 0.2, 0.2, 0.2);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.diffuse"), 0.2, 0.3, 0.6);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.specular"), 0.5,0.3,0.2);

		shaderLighting.turnOff();

		shaderDirectionLight.turnOn();
		glUniform3fv(shaderDirectionLight.getUniformLocation("light.direction"), 1, glm::value_ptr(glm::vec3(0.0f, -10.0f, -20.0f)));
		glUniform3f(shaderDirectionLight.getUniformLocation("light.ambient"), 0.4, 0.4, 0.4);
		glUniform3f(shaderDirectionLight.getUniformLocation("light.diffuse"), 0.6, 0.6, 0.6);
		glUniform3f(shaderDirectionLight.getUniformLocation("light.specular"), 0.5, 0.5, 0.5);

		shaderDirectionLight.turnOff();



		//piso
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrix0 = glm::mat4(1.0f);//matriz con diagonal unitaria
		glm::mat4 tempavion = matrix0;

		matrix0 = glm::translate(matrix0, glm::vec3(0.0f, 0.0f, 0.0f));//Matriz del piso
		glm::mat4 matrix1 = glm::translate(matrix0, glm::vec3(21.0f, 6.0f, -14.5f));//edificio externo derecho que se traslada desde el centro del piso
		glm::mat4 temp06 = matrix1;
		glm::mat4 matrix2 = glm::translate(matrix0, glm::vec3(21.0f, 6.0f, -14.5f));//edificio interno derecho que se traslada desde el centro del piso
		glm::mat4 temp08 = matrix2;
		glm::mat4 matrix3 = glm::translate(matrix0, glm::vec3(21.4f, 1.5f, -21.4f));//sala B que se traslada desde el centro del piso
		glm::mat4 temp09 = matrix3;
		glm::mat4 matrix04 = glm::translate(matrix0, glm::vec3(22.4f, 1.5f, -14.4f));//sala B que se traslada desde el centro del piso 
		glm::mat4 matrix05 = glm::translate(matrix0, glm::vec3(21.4f, 1.5f, -7.5f));//sala A
		glm::mat4 matrix01 = glm::translate(matrix1, glm::vec3(-2.5f, 2.0f, 10.6f));//placa q, que se traslada a partir del centro del edificio derecho
		glm::mat4 matrix02 = glm::translate(matrix1, glm::vec3(-4.25f, 0.0f, 10.25f));//detalles
		glm::mat4 matrix06 = glm::translate(matrix1, glm::vec3(-4.25f, 0.0f, 7.25f));//detalles
		glm::mat4 matrix07 = glm::translate(matrix1, glm::vec3(-4.25f, 0.0f, 4.25f));//detalles
		glm::mat4 matrix08 = glm::translate(matrix1, glm::vec3(-4.25f, -3.25f, 7.25f));//detalles
		glm::mat4 matrix09 = glm::translate(matrix1, glm::vec3(-4.25f, -0.0f, 7.25f));//detalles horizontal
		glm::mat4 matrix010 = glm::translate(matrix1, glm::vec3(-4.25f, 3.25f, 7.25f));//detalles
		glm::mat4 matrix019 = glm::translate(matrix1, glm::vec3(-4.25f, 6.0f, 7.25f));//detalles
		glm::mat4 matrix020 = glm::translate(matrix1, glm::vec3(-4.25f, -6.0f, 0.0f));//detalles
		glm::mat4 matrix011 = glm::translate(matrix1, glm::vec3(-4.0f, -4.5f, 8.5f));//ventanas
		glm::mat4 temp04 = matrix011;
		glm::mat4 matrix012 = glm::translate(matrix1, glm::vec3(-4.0f, -4.5f, 5.5f));//ventanas
		glm::mat4 matrix013 = glm::translate(matrix1, glm::vec3(-4.0f, -1.5f, 8.5f));//ventanas
		glm::mat4 matrix014 = glm::translate(matrix1, glm::vec3(-4.0f, -1.5f, 5.5f));//ventanas
		glm::mat4 matrix015 = glm::translate(matrix1, glm::vec3(-4.0f, 1.5f, 8.5f));//ventanas
		glm::mat4 matrix016 = glm::translate(matrix1, glm::vec3(-4.0f, 1.5f, 5.5f));//ventanas
		glm::mat4 matrix017 = glm::translate(matrix1, glm::vec3(-4.0f, 4.5f, 8.5f));//ventanas
		glm::mat4 matrix018 = glm::translate(matrix1, glm::vec3(-4.0f, 4.5f, 5.5f));//ventanas
		/**/
		glm::mat4 temp272 = glm::translate(matrix0, glm::vec3(0.0f, 0.0f, 0.0f));

		glm::mat4 matrix021 = glm::translate(matrix09, glm::vec3(-5.0f, 0.0f, -7.25f));//edificio med exterior 
		glm::mat4 temp05 = matrix021;
		glm::mat4 matrix037 = glm::translate(temp05, glm::vec3(0.0f, -5.5f, 8.0f));
		
		//que se traslada desde el centro de un barra horizontal.
		glm::mat4 matrix022 = glm::translate(matrix09, glm::vec3(-5.0f, 0.0f, -7.25f));//edificio med int
		glm::mat4 temp07 = matrix022;
		glm::mat4 matrix023 = glm::translate(matrix021, glm::vec3(-9.25f, 0.0f, 8.0f));//edificio izq ext
		glm::mat4 matrix024 = glm::translate(matrix021, glm::vec3(-9.25f, 0.0f, 8.0f));//edificio izq int
		glm::mat4 matrix025 = glm::translate(matrix09, glm::vec3(-0.375f, 0.0f, -3.0f));//detalles edificio medio 
		glm::mat4 matrix026 = glm::translate(matrix025, glm::vec3(-3.25f, 0.0f, 0.0f));
		glm::mat4 matrix027 = glm::translate(matrix026, glm::vec3(-3.25f, 0.0f, 0.0f));
		glm::mat4 matrix028 = glm::translate(matrix027, glm::vec3(-3.25f, 0.0f, 0.0f));
		glm::mat4 matrix029 = glm::translate(matrix021, glm::vec3(0.0f, -5.9f, 4.25f));
		glm::mat4 matrix030 = glm::translate(matrix029, glm::vec3(0.0f, 2.625f, 0.0f));
		glm::mat4 matrix031 = glm::translate(matrix030, glm::vec3(0.0f, 3.25f, 0.0f));
		glm::mat4 matrix032 = glm::translate(matrix031, glm::vec3(0.0f, 3.25f, 0.0f));
		glm::mat4 matrix033 = glm::translate(matrix032, glm::vec3(0.0f, 2.75f, 0.0f));
		glm::mat4 matrix034 = glm::translate(matrix028, glm::vec3(0.125f, 0.0f, 0.5f));
		glm::mat4 temp01 = matrix034;
		glm::mat4 matrix035 = glm::translate(matrix029, glm::vec3(-5.0f, 0.0f, 8.75f));
		glm::mat4 temp02 = matrix035;
		//ventanas edificio medio
		glm::mat4 matrix036 = glm::translate(matrix021, glm::vec3(-3.375f, -4.5f, 4.02f));
		glm::mat4 temp03 = matrix036;
		

		//como en clase de valencia, siempre se traslada antes de escalar, para evitar degrados en las texturas y en los cubos
	//primero se escala la matriz del piso y se le coloca la textura
		matrix0 = glm::scale(matrix0, glm::vec3(50.0f, 0.01f, 50.0f));
		glBindTexture(GL_TEXTURE_2D, texturePisoID);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrix0);
		//Despues escalo la matriz del edificio, 
		//Lado derecho del edificio
		//parte de fuera con luz direccional
		matrix1 = glm::scale(matrix1, glm::vec3(8.0f, 12.0f, 21.0f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(0, 18, matrix1);
		box.render(30, 6, matrix1);
		glBindTexture(GL_TEXTURE_2D, textureID8);
		box.render(24, 6, matrix1);
		glBindTexture(GL_TEXTURE_2D, textureID2);
		matrix1 = glm::translate(temp06, glm::vec3(0.0f, 0.0f, -7.25));
		matrix1 = glm::scale(matrix1, glm::vec3(8.0f, 12.0f, 6.5f));
		box.render(18,6, matrix1);
		glBindTexture(GL_TEXTURE_2D, textureID32);
		//box.setShader(&shaderTexture);
		matrix1 = glm::translate(temp06, glm::vec3(0.0f, -2.9f,0.0f ));
		matrix1 = glm::scale(matrix1, glm::vec3(8.0f, 0.01f, 21.0f));
		box.render(24, 6, matrix1);
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box.render(30,6, matrix1);
		//Parte de adentro con luz
		matrix2 = glm::scale(matrix2, glm::vec3(7.9f, 11.9f, 20.9f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(0,18, matrix2);
		box.render(24, 6, matrix2);
		glBindTexture(GL_TEXTURE_2D, textureID32);
		box.render(30, 6, matrix2);
		glBindTexture(GL_TEXTURE_2D, textureID2);
		matrix2 = glm::translate(temp08, glm::vec3(0.1f, 0.0f, -7.25));
		matrix2 = glm::scale(matrix2, glm::vec3(8.0f, 12.0f, 6.5f));
		box.render(18, 6, matrix2);

		//Creacion del salon B
		matrix3 = glm::scale(matrix3, glm::vec3(7.0f, 3.0f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		box2.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.render(0, 24, matrix3);
		//pizarron
		matrix3 = glm::translate(temp09, glm::vec3(0.0f, 0.5f, -3.4));
		matrix3 = glm::scale(matrix3, glm::vec3(2.2f, 1.5f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID76);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 24, matrix3);
		//acero suelo
		matrix3 = glm::translate(temp09, glm::vec3(-0.8f, -1.25f, 0.0f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.2f, 0.1f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID719);
		box6.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box6.setProjectionMatrix(projection);
		box6.setViewMatrix(view);
		box6.render(0, 24, matrix3);
		box6.render(24, 6, matrix3);
		///proyector
		matrix3 = glm::translate(temp09, glm::vec3(0.45f, 1.40f, -1.0f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.04f, 0.17f, 0.1f));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);

		glm::mat4 matrixAirCraft7173 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
		matrixAirCraft7173 = glm::translate(temp09, glm::vec3(0.0, 1.3, -1.0));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(90.0f), glm::vec3(0, 1, 0));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelproyector.setScale(glm::vec3(0.005, 0.005, 0.005));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelproyector.setShader(&shaderTexture);
		modelproyector.setProjectionMatrix(projection);
		modelproyector.setViewMatrix(view);
		modelproyector.render(matrixAirCraft7173);
		//VENTANAS

		matrix3 = glm::translate(temp09, glm::vec3(3.49f, 0.60f, 0.0f));
		glm::mat4 temp010 = matrix3;
		matrix3 = glm::scale(matrix3, glm::vec3(0.01f, 1.8f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7114);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);

		matrix3 = glm::translate(temp010, glm::vec3(-0.01f, 0.0f, 0.25f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.01f, 1.7f, 2.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7116);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);

		matrix3 = glm::translate(temp010, glm::vec3(-0.01f, 0.0f, 2.5f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.01f, 1.7f, 2.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7116);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);

		matrix3 = glm::translate(temp010, glm::vec3(-0.01f, 0.0f, -2.25f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.01f, 1.7f, 2.3f));
		glBindTexture(GL_TEXTURE_2D, textureID7116);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);
		//cemento


		matrix3 = glm::translate(temp09, glm::vec3(3.49f, -0.877f, 0.0f));
		temp010 = matrix3;
		matrix3 = glm::scale(matrix3, glm::vec3(0.01f, 1.2f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7115);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);


		matrix3 = glm::translate(temp010, glm::vec3(-0.01f, 0.54f, 0.0f));
		temp010 = matrix3;
		matrix3 = glm::scale(matrix3, glm::vec3(0.1f, 0.3f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7115);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);
		//castillo y botiquin
		matrix3 = glm::translate(temp09, glm::vec3(3.45f, -0.25f, -0.9f));
		temp010 = matrix3;
		matrix3 = glm::scale(matrix3, glm::vec3(0.10f, 2.5f, 0.21f));
		glBindTexture(GL_TEXTURE_2D, textureID7110);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);

		matrix3 = glm::translate(temp010, glm::vec3(-0.08f, 0.55f, 0.0f));
		temp010 = matrix3;
		matrix3 = glm::scale(matrix3, glm::vec3(0.09f, 0.4f, 0.19f));
		glBindTexture(GL_TEXTURE_2D, textureID7111);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);

		matrix3 = glm::translate(temp010, glm::vec3(0.0268f, 0.52f, 0.0f));
		temp010 = matrix3;
		matrix3 = glm::scale(matrix3, glm::vec3(0.001f, 0.18f, 0.17f));
		glBindTexture(GL_TEXTURE_2D, textureID7112);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);


		matrix3 = glm::translate(temp010, glm::vec3(0.0f, 0.0f, 0.17f));
		temp010 = matrix3;
		matrix3 = glm::scale(matrix3, glm::vec3(0.001f, 0.18f, 0.17f));
		glBindTexture(GL_TEXTURE_2D, textureID7113);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);


		//glm::mat4 matrixAirCraft7173 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
		matrixAirCraft7173 = glm::translate(temp010, glm::vec3(0.0, 0.1, 0.4));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(90.0f), glm::vec3(0, 1, 0));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelac.setScale(glm::vec3(0.02, 0.009, 0.0014));
		glBindTexture(GL_TEXTURE_2D, textureID7118);
		modelac.setShader(&shaderTexture);
		modelac.setProjectionMatrix(projection);
		modelac.setViewMatrix(view);
		modelac.render(matrixAirCraft7173);



		matrixAirCraft7173 = glm::translate(temp09, glm::vec3(0.0, 10.4, 20.7));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(180.0f), glm::vec3(0, 1, 0));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmueble.setScale(glm::vec3(0.3, 0.5, 0.14));
		glBindTexture(GL_TEXTURE_2D, textureID7119);
		modelmueble.setShader(&shaderTexture);
		modelmueble.setProjectionMatrix(projection);
		modelmueble.setViewMatrix(view);
		modelmueble.render(matrixAirCraft7173);



		//lamparas

		matrix3 = glm::translate(temp09, glm::vec3(2.0f, 1.49f, 2.5f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.5f, 0.01f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID7120);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);

		//reglamento

		matrix3 = glm::translate(temp09, glm::vec3(1.97f, 0.4f, 3.49f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.5f, 0.9f, 0.001f));
		glBindTexture(GL_TEXTURE_2D, textureID7121);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix3);
		//Salon medio
		matrix04 = glm::scale(matrix04, glm::vec3(5.0f, 3.0f, 6.8f));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		box.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(0, 24, matrix04);
		//Sala A
		matrix05 = glm::scale(matrix05, glm::vec3(7.0f, 3.0f, 6.9f));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		box2.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.render(0, 24, matrix05);
		//techo
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.render(24, 6, matrix3);
		box2.render(24, 6, matrix04);
		box2.render(24, 6, matrix05);
		//placa edificio Q
		matrix01 = glm::scale(matrix01, glm::vec3(1.0f, 1.2f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(0, 6, matrix01);

		//detalles edificio derecho
		matrix02 = glm::scale(matrix02, glm::vec3(0.5f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix02);

		matrix06 = glm::scale(matrix06, glm::vec3(0.5f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix06);

		matrix07 = glm::scale(matrix07, glm::vec3(0.5f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix07);
		//---

		matrix08 = glm::scale(matrix08, glm::vec3(0.5f, 0.5f, 6.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix08);

		matrix09 = glm::scale(matrix09, glm::vec3(0.5f, 0.5f, 6.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix09);

		matrix010 = glm::scale(matrix010, glm::vec3(0.5f, 0.5f, 6.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix010);

		matrix019 = glm::scale(matrix019, glm::vec3(0.5f, 0.5f, 6.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix019);

		matrix020 = glm::scale(matrix020, glm::vec3(0.5f, 0.05f, 21.0f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix020);
		//ventanas

		matrix011 = glm::translate(temp04, glm::vec3(0.0f, -1.25f, 0.5f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(-0.25f, 2.0f, 0.5f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(-0.25f, 5.15f, 0.5f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(-0.25f, 8.2f, 0.5f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		box3.render(18, 6, matrix011);
		/**************************/
		matrix011 = glm::translate(temp04, glm::vec3(-0.25f, -1.24f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(-0.25f, 2.0f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(-0.25f, 5.15f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(-0.25f, 8.2f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 3.0f));
		box3.render(18, 6, matrix011);
		/*************/

		matrix011 = glm::translate(temp04, glm::vec3(0.0f, -0.25f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box4.setShader(&shaderTexture);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.render(18, 6, matrix011);

		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 2.9f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		box4.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 6.1f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		box4.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 9.0f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		box4.render(18, 6, matrix011);


		/**************************/
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, -0.24f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		box4.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 2.9f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		box4.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 6.1f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		box4.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 9.0f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 1.0f, 2.5f));
		box4.render(18, 6, matrix011);

		/***************/
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 0.6f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 3.8f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 7.1f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		box3.render(18, 6, matrix011);
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 9.9f, 0.25f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		box3.render(18, 6, matrix011);
		/**********/
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 0.6f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 3.8f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 7.1f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		box3.render(18, 6, matrix011);
		box3.render(18, 6, matrix011);
		matrix011 = glm::translate(temp04, glm::vec3(0.0f, 9.9f, -2.75f));
		matrix011 = glm::scale(matrix011, glm::vec3(0.01f, 0.8f, 2.5f));
		box3.render(18, 6, matrix011);

		/*matrix013 = glm::scale(matrix013, glm::vec3(0.01f, 3.0f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix013);

		matrix014 = glm::scale(matrix014, glm::vec3(0.01f, 3.0f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix014);

		matrix015 = glm::scale(matrix015, glm::vec3(0.01f, 3.0f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix015);


		matrix016 = glm::scale(matrix016, glm::vec3(0.01f, 3.0f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix016);

		matrix017 = glm::scale(matrix017, glm::vec3(0.01f, 3.0f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix017);

		matrix018 = glm::scale(matrix018, glm::vec3(0.01f, 3.0f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix018);*/
		//edificio del medio
		//parte de fuera con luz direccional
		matrix021 = glm::scale(matrix021, glm::vec3(10.5f, 12.0f, 8.0f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setShader(&shaderDirectionLight);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		//box.render(0, 6, matrix021);
		box.render(12,12, matrix021);
		box.render(30, 6, matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID8);
		box.render(24, 6, matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID2);
		//Parte de adentro con sin tipo de luz
		matrix022 = glm::scale(matrix022, glm::vec3(10.4f, 11.9f, 7.9f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(12, 6, matrix022);
		box.render(24, 6, matrix022);
		glBindTexture(GL_TEXTURE_2D, textureID31);
		box.render(18,6, matrix022);
		glBindTexture(GL_TEXTURE_2D, textureID32);
		box.render(30,6, matrix022);
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrix021 = glm::translate(temp07, glm::vec3(0.0f, -4.5f, 2.45f));
		matrix021 = glm::scale(matrix021, glm::vec3(10.4f, 3.0f, 3.0f));
		box3.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(6, 6, matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrix021 = glm::translate(temp07, glm::vec3(0.0f, -4.5f, 0.0f));
		matrix021 = glm::scale(matrix021, glm::vec3(10.4f, 3.0f, 2.0f));
		box3.render(6, 6, matrix021);
		/*entrada interior*/
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrix021 = glm::translate(temp07, glm::vec3(3.25f, -4.5f, -2.3f));
		matrix021 = glm::scale(matrix021, glm::vec3(4.0f, 3.0f, 3.0f));
		box3.render(6,30,matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID33);
		box3.render(0,6, matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrix021 = glm::translate(temp07, glm::vec3(-3.5f, -4.5f, -2.3f));
		matrix021 = glm::scale(matrix021, glm::vec3(3.5f, 3.0f, 3.0f));
		box3.render(6, 30, matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID35);
		box3.render(0, 6, matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrix021 = glm::translate(temp07, glm::vec3(-1.0f, -4.5f, -2.3f));
		matrix021 = glm::scale(matrix021, glm::vec3(1.5f, 3.0f, 3.0f));
		box3.render(6, 30, matrix021);
		glBindTexture(GL_TEXTURE_2D, textureID34);
		box3.render(0, 6, matrix021);

		glBindTexture(GL_TEXTURE_2D, textureID32);
		matrix021 = glm::translate(temp07, glm::vec3(3.25f, -3.0f, 0.0f));
		matrix021 = glm::scale(matrix021, glm::vec3(4.0f, 0.01f, 8.0f));
		box.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrix021);
		matrix021 = glm::translate(temp07, glm::vec3(0.25f, -3.0f, 1.5f));
		matrix021 = glm::scale(matrix021, glm::vec3(2.5f, 0.01f, 5.0f));
		box.render(matrix021);
		matrix021 = glm::translate(temp07, glm::vec3(-2.875f, -3.0f, 0.0f));
		matrix021 = glm::scale(matrix021, glm::vec3(5.0f, 0.01f, 8.0f));
		box.render(matrix021);
		//Lado izquierdo del edificio
		//parte de fuera con luz direccional
		matrix023 = glm::scale(matrix023, glm::vec3(8.0f, 12.0f, 28.0f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setShader(&shaderDirectionLight);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(0, 24, matrix023);
		box.render(30, 6, matrix023);
		glBindTexture(GL_TEXTURE_2D, textureID8);
		box.render(24, 6, matrix023);
		//Parte de adentro con sin tipo de luz
		matrix024 = glm::scale(matrix024, glm::vec3(7.9f, 11.9f, 27.9f));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrix024);
		//Detalles edificio medio 

		matrix025 = glm::scale(matrix025, glm::vec3(0.25f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix025);

		matrix026 = glm::scale(matrix026, glm::vec3(0.25f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix026);

		matrix027 = glm::scale(matrix027, glm::vec3(0.25f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix027);

		matrix028 = glm::scale(matrix028, glm::vec3(0.25f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix028);
		//horizontal
		matrix029 = glm::scale(matrix029, glm::vec3(10.5f, 0.025f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix029);


		matrix030 = glm::scale(matrix030, glm::vec3(10.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix030);

		matrix031 = glm::scale(matrix031, glm::vec3(10.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix031);

		matrix032 = glm::scale(matrix032, glm::vec3(10.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix032);

		matrix033 = glm::scale(matrix033, glm::vec3(10.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix033);

		//detalles edificio izquierdo

		matrix034 = glm::scale(matrix034, glm::vec3(0.5f, 12.0f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix034);
		matrix034 = glm::translate(temp01, glm::vec3(0.0f, 0.0f, 4.5f));
		matrix034 = glm::scale(matrix034, glm::vec3(0.5f, 12.0f, 0.5f));
		box3.render(matrix034);
		matrix034 = glm::translate(temp01, glm::vec3(0.0f, 0.0f, 9.0f));
		matrix034 = glm::scale(matrix034, glm::vec3(0.5f, 12.0f, 0.5f));
		box3.render(matrix034);
		matrix034 = glm::translate(temp01, glm::vec3(0.0f, 0.0f, 13.5f));
		matrix034 = glm::scale(matrix034, glm::vec3(0.5f, 12.0f, 0.5f));
		box3.render(matrix034);
		matrix034 = glm::translate(temp01, glm::vec3(0.0f, 0.0f, 17.0f));
		matrix034 = glm::scale(matrix034, glm::vec3(0.5f, 12.0f, 0.5f));
		box3.render(matrix034);



		matrix035 = glm::scale(matrix035, glm::vec3(0.5f, 0.025f, 17.5f));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box3.setShader(&shaderDirectionLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.0f, 2.625f, 0.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(0.5f, 0.5f, 17.5f));
		box3.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.0f, 5.875f, 0.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(0.5f, 0.5f, 17.5f));
		box3.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.0f, 9.125f, 0.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(0.5f, 0.5f, 17.5f));
		box3.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.0f, 11.875f, 0.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(0.5f, 0.5f, 17.5f));
		box3.render(matrix035);

		// ventanas edicio izquierdo
		glBindTexture(GL_TEXTURE_2D, textureID17);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 1.2f, -1.5f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 2.25f, 4.0f));
		box5.setShader(&shaderDirectionLight);
		box5.setProjectionMatrix(projection);
		box5.setViewMatrix(view);
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 4.25f, -1.5f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 7.3f, -1.5f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 10.5f, -1.5f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 2.5f, 4.0f));
		box5.render(matrix035);

		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 1.2f, -6.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 4.25f, -6.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 7.3f, -6.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 10.5f, -6.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 2.5f, 4.0f));
		box5.render(matrix035);

		//matrix035 = glm::translate(temp02, glm::vec3(0.01f, 1.2f, 3.0f));
		//matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		//box5.render(matrix035);
		glBindTexture(GL_TEXTURE_2D, textureID16);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 1.2f, 2.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 2.25f, 2.0f));
		box3.render(matrix035);
		glBindTexture(GL_TEXTURE_2D, textureID17);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 1.2f, 4.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 2.25f, 2.0f));
		box3.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 4.25f, 3.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 7.3f, 3.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 4.0f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 10.5f, 3.0f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 2.5f, 4.0f));
		box5.render(matrix035);

		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 1.2f, 7.25f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 3.5f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 4.25f, 7.25f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 3.5f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 7.3f, 7.25f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 3.0f, 3.5f));
		box5.render(matrix035);
		matrix035 = glm::translate(temp02, glm::vec3(0.01f, 10.5f, 7.25f));
		matrix035 = glm::scale(matrix035, glm::vec3(-0.01f, 2.5f, 3.5f));
		box5.render(matrix035);
		//ventanas edificio medio
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box4.setShader(&shaderTexture);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.render(0, 6, matrix036);
		matrix036 = glm::translate(temp03, glm::vec3(-0.125f, 1.25f, 0.5f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 0.5f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID12);
		box3.render(0, 6, matrix036);

		matrix036 = glm::translate(temp03, glm::vec3(-0.125f, 3.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box4.render(0, 6, matrix036);

		matrix036 = glm::translate(temp03, glm::vec3(-0.125f, 6.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);

		matrix036 = glm::translate(temp03, glm::vec3(-0.125f, 9.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);

		matrix036 = glm::translate(temp03, glm::vec3(3.125f, 01.25f, 0.5f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 0.5f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		box3.render(0, 6, matrix036);

		matrix036 = glm::translate(temp03, glm::vec3(3.125f, 0.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box3.setShader(&shaderTexture);
		box3.render(0, 6, matrix036);

		matrix036 = glm::translate(temp03, glm::vec3(3.125f, 3.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box4.render(0, 6, matrix036);
		matrix036 = glm::translate(temp03, glm::vec3(3.125f, 6.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);
		matrix036 = glm::translate(temp03, glm::vec3(3.125f, 9.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);
		matrix036 = glm::translate(temp03, glm::vec3(6.375f, 0.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);
		matrix036 = glm::translate(temp03, glm::vec3(6.375f, 3.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);
		matrix036 = glm::translate(temp03, glm::vec3(6.375f, 6.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);
		matrix036 = glm::translate(temp03, glm::vec3(6.375f, 9.0f, 0.0f));
		matrix036 = glm::scale(matrix036, glm::vec3(3.0f, 3.0f, 0.01f));
		box4.render(0, 6, matrix036);

		//cuadro frente al edificio
		matrix037 = glm::translate(temp05, glm::vec3(0.0f, -5.5f, 9.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 1.0f, 0.25f));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix037);

		//estatua
		matrix037 = glm::translate(temp05, glm::vec3(0.0f, -4.5f, 9.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 0.8f, 0.25f));
		glBindTexture(GL_TEXTURE_2D, textureID18);
		box3.render(12, 6, matrix037);
		//bebedero
		matrix037 = glm::translate(temp05, glm::vec3(-1.5f, -5.5f, 5.15f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 1.5f, 0.25f));
		glBindTexture(GL_TEXTURE_2D, textureID27);
		box3.render(0, 6, matrix037);
		matrix037 = glm::translate(temp05, glm::vec3(-1.5f, -5.5f, 5.15f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.125f, 1.5f, 0.25f));
		glBindTexture(GL_TEXTURE_2D, textureID28);
		box3.render(6, 6, matrix037);
		//tierra media
		matrix037 = glm::translate(temp05, glm::vec3(0.0f, -5.75f, 11.75f));
		matrix037 = glm::scale(matrix037, glm::vec3(3.0f, 0.5f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		box3.render(matrix037);

		matrix037 = glm::translate(temp05, glm::vec3(0.0f, -5.75f, 10.125f));
		matrix037 = glm::scale(matrix037, glm::vec3(3.0f, 0.5f, 0.25f));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		box3.render(matrix037);
		matrix037 = glm::translate(temp05, glm::vec3(0.0f, -5.75f, 13.375f));
		matrix037 = glm::scale(matrix037, glm::vec3(3.0f, 0.5f, 0.25f));
		box3.render(matrix037);
		matrix037 = glm::translate(temp05, glm::vec3(-1.625f, -5.75f, 11.75f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 0.5f, 3.5f));
		box3.render(0, 24, matrix037);
		box6.setShader(&shaderTexture);
		box6.setProjectionMatrix(projection);
		box6.setViewMatrix(view);
		box6.render(24, 6, matrix037);

		matrix037 = glm::translate(temp05, glm::vec3(1.625f, -5.75f, 11.75f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 0.5f, 3.5f));
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(matrix037);

		//tierra frontal
		matrix037 = glm::translate(temp06, glm::vec3(-0.5f, -5.875f, 11.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(9.0f, 0.25f, 2.0f));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		box3.render(matrix037);
		//tierra lado derecho
		matrix037 = glm::translate(temp06, glm::vec3(-5.0f, -5.95f, 7.25f));
		matrix037 = glm::scale(matrix037, glm::vec3(1.0f, 0.05f, 6.5f));
		box3.render(matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-0.5f, -5.75f, 12.625f));
		matrix037 = glm::scale(matrix037, glm::vec3(9.0f, 0.5f, 0.25f));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		box3.render(matrix037);

		matrix037 = glm::translate(temp06, glm::vec3(-5.125f, -5.75f, 11.88f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 0.5f, 1.625f));
		box3.render(matrix037);

		matrix037 = glm::translate(temp06, glm::vec3(-5.25f, -5.75f, 10.8f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.5f, 0.5f, 0.55f));
		box3.render(matrix037);

		matrix037 = glm::translate(temp06, glm::vec3(-0.5f, -5.75f, 12.625f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.01f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		//box3.render(matrix037);


		matrix037 = glm::translate(temp06, glm::vec3(-6.0f, -5.0f, 6.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.05f, 2.0f, 0.8f));
		glBindTexture(GL_TEXTURE_2D, textureID21);
		box3.render(18, 6, matrix037);

		matrix037 = glm::translate(temp06, glm::vec3(-6.0f, -5.0f, 7.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.05f, 2.0f, 0.8f));
		glBindTexture(GL_TEXTURE_2D, textureID22);
		box3.render(18, 6, matrix037);
		//arbustos derecho
		matrix037 = glm::translate(temp06, glm::vec3(-5.0f, -5.375f, 7.375f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 1.0f, 6.0f));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		box7.setShader(&shaderTexture);
		box7.setProjectionMatrix(projection);
		box7.setViewMatrix(view);
		box7.render(18, 6, matrix037);
		box7.render(6, 6, matrix037);
		//arbusto derecho
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 5.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(18, 6, matrix037);

		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 5.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 6.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 6.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 7.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 7.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 8.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 8.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 9.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 9.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 10.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 10.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 11.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-5.5f, -5.0f, 11.5f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);

		//arbusto izquierdo
		matrix037 = glm::translate(temp06, glm::vec3(-13.0f, -5.0f, 5.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);
		matrix037 = glm::translate(temp06, glm::vec3(-13.0f, -5.0f, 6.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 2.0f, 0.45f));
		box3.render(18, 6, matrix037);

		//tierra lado izquierdo
		matrix037 = glm::translate(temp06, glm::vec3(-13.75f, -5.95f, 13.0f));
		matrix037 = glm::scale(matrix037, glm::vec3(1.0f, 0.05f, 17.5f));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		box3.render(matrix037);

		//arbusto izquierdo
		matrix037 = glm::translate(temp06, glm::vec3(-13.5f, -5.375f, 9.375f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 1.0f, 10.0f));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		box7.setShader(&shaderTexture);
		box7.setProjectionMatrix(projection);
		box7.setViewMatrix(view);
		box7.render(18, 6, matrix037);
		box7.render(6, 6, matrix037);

		matrix037 = glm::translate(temp06, glm::vec3(-13.5f, -5.375f, 20.125f));
		matrix037 = glm::scale(matrix037, glm::vec3(0.25f, 1.0f, 4.0f));
		box7.render(18, 6, matrix037);
		box7.render(6, 6, matrix037);

		matrix037 = glm::translate(temp06, glm::vec3(-13.5f, -5.8f, 15.875f));
		matrix037 = glm::scale(matrix037, glm::vec3(1.0f, 0.125f, 4.0f));
		glBindTexture(GL_TEXTURE_2D, textureID26);
		box3.setShader(&shaderTexture);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.render(24, 6, matrix037);
		glBindTexture(GL_TEXTURE_2D, textureID27);
		box3.render(0, 6, matrix037);


		arbol.setShader(&shaderTexture);
		arbol.setProjectionMatrix(projection);
		arbol.setViewMatrix(view);
		arbol.setPosition(glm::vec3(11.75, 0.5, -2.5));
		arbol.setScale(glm::vec3(0.9, 1.0, 0.9));
		arbol.render();

		glm::mat4 hojas = glm::translate(tempavion, glm::vec3(11.5f, 1.0f, -2.5f));
		hojas = glm::scale(hojas, glm::vec3(2.0f, 2.0f, 2.0f));
		glBindTexture(GL_TEXTURE_2D, texturehojas);
		boxWater.setShader(&shaderTexture);
		boxWater.setProjectionMatrix(projection);
		boxWater.setViewMatrix(view);
		boxWater.offsetUVS(glm::vec3(0.0, 0.01,0.0));
		boxWater.render(0,24, hojas);

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.001;

		persona.setShader(&shaderTexture);
		persona.setProjectionMatrix(projection);
		persona.setViewMatrix(view);
		persona.setPosition(glm::vec3(14.75, 0.125, -0.5));
		persona.setScale(glm::vec3(0.01, 0.02, 0.01));
		persona.render();

		arbol.setPosition(glm::vec3(15.8, 0.25, -8.7));
		arbol.setScale(glm::vec3(0.25, 0.5, 0.25));
		arbol.render();

		arbol.setPosition(glm::vec3(20.75, -0.25, -2.5));
		arbol.setScale(glm::vec3(0.25, 0.5, 0.25));
		arbol.render();

		arbol.setPosition(glm::vec3(21.75, -0.25, -2.5));
		arbol.setScale(glm::vec3(0.25, 0.5, 0.25));
		arbol.render();
		arbol2.setShader(&shaderTexture);
		arbol2.setProjectionMatrix(projection);
		arbol2.setViewMatrix(view);
		arbol2.setPosition(glm::vec3(7.75, 0.2, -7.0));
		arbol2.setScale(glm::vec3(0.5, 1.0, 0.5));
		arbol2.render();
		arbol2.setPosition(glm::vec3(7.75, 0.2, 4.0));
		arbol2.setScale(glm::vec3(0.5, 1.0, 0.5));
		arbol2.render();

		//glBindTexture(GL_TEXTURE_2D, textureID7119);
		//modelmueble.setShader(&shaderTexture);
		//modelmueble.setProjectionMatrix(projection);
		//modelmueble.setViewMatrix(view);
		//modelmueble.setPosition(glm::vec3(11.75, 3.5, 1.5));
		//modelmueble.setScale(glm::vec3(0.9, 1.0, 0.9));
		//modelmueble.render();


		//SALON *****************************************************
		//*************************************************************

		//glm::mat4 matrix81 = glm::mat4(1.0f);//matriz con diagonal unitaria
		//matrix81 = glm::translate(temp79, glm::vec3(0.0f, 0.0f, -10.0f));
		
	


	
		//glm::mat4 cpu = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//sala B


		//Creacion del salon
		//cara exterior

		//SALON *****************************************************
		//*************************************************************

		glm::mat4 matrix81 = glm::mat4(1.0f);//matriz con diagonal unitaria
		matrix81 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, -10.0f));
		glm::mat4 matrix71 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//edificio externo
		glm::mat4 matrix72 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//edificio interno
		glm::mat4 matrix73 = glm::translate(matrix81, glm::vec3(21.4f, 1.5f, -7.5f));//sala B
		glm::mat4 matrix74 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//pared
		glm::mat4 matrix75 = glm::translate(matrix81, glm::vec3(21.4f, 1.5f, -7.5f));//pared



		glm::mat4 temp2 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//sala B
		glm::mat4 temp3 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//mesas
		glm::mat4 temp4 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//computadoras
		glm::mat4 temp5 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//acero piso
		glm::mat4 temp6 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//CORTINAS
		glm::mat4 temp7 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 temp8 = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));

		//glm::mat4 cpu = glm::translate(matrix81, glm::vec3(0.0f, 0.0f, 0.0f));//sala B


		//Creacion del salon
		//cara exterior

		matrix73 = glm::translate(matrix73, glm::vec3(0.0f, 0.0f, -3.8f));
		matrix74 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(7.0f, 3.0f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box014.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box014.setProjectionMatrix(projection);
		box014.setViewMatrix(view);
		//box014.render(0, 24, matrix73);

		//techo
		glBindTexture(GL_TEXTURE_2D, textureID716);
		box014.setShader(&shaderTexture);
		//box014.render(24, 6, matrix73);

		//SUELO
		glBindTexture(GL_TEXTURE_2D, textureID715);
		box014.setShader(&shaderTexture);
		//box014.render(30, 6, matrix73);

		//acero suelo

		matrix73 = glm::translate(matrix73, glm::vec3(-0.8f, -1.499f, 0.0f));
		matrix73= glm::scale(matrix73, glm::vec3(0.2f, 0.1f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID719);
		//box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		//box012.setProjectionMatrix(projection);
		//box012.setViewMatrix(view);
		//box012.render(24, 6, matrix73);

		//proyector

		matrix73 = glm::translate(matrix74, glm::vec3(0.45f, 1.40f, -1.0f));
		matrix73 = glm::scale(matrix73, glm::vec3(0.04f, 0.17f, 0.1f));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

//		glm::mat4 matrixAirCraft7173 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
		matrixAirCraft7173 = glm::translate(matrix74, glm::vec3(0.0, 1.3, -1.0));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(90.0f), glm::vec3(0, 1, 0));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelproyector.setScale(glm::vec3(0.005, 0.005, 0.005));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelproyector.setShader(&shaderTexture);
		modelproyector.setProjectionMatrix(projection);
		modelproyector.setViewMatrix(view);
		//modelproyector.render(matrixAirCraft7173);



		//lamparas

		matrix73 = glm::translate(matrix74, glm::vec3(2.0f, 1.49f, 2.5f));
		matrix73 = glm::scale(matrix73, glm::vec3(0.5f, 0.01f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID7120);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		//reglamento

		matrix73 = glm::translate(matrix74, glm::vec3(1.97f, 0.4f, 3.49f));
		matrix73 = glm::scale(matrix73, glm::vec3(0.5f, 0.9f, 0.001f));
		glBindTexture(GL_TEXTURE_2D, textureID7121);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);


		//atras del pizarron
		matrix73 = glm::translate(matrix74, glm::vec3(0.0f, 0.0f, -3.5f));
		matrix74 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(7.0f, 3.0f, 0.001f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box014.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box014.setProjectionMatrix(projection);
		box014.setViewMatrix(view);
	//	box014.render(0, 24, matrix73);

		//castillo y botiquin
		matrix73 = glm::translate(matrix74, glm::vec3(3.45f, -0.25f, 2.5f));
		temp5 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(0.10f, 2.5f, 0.21f));
		glBindTexture(GL_TEXTURE_2D, textureID7110);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		matrix73 = glm::translate(temp5, glm::vec3(-0.08f, 0.55f, 0.0f));
		temp5 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(0.09f, 0.4f, 0.19f));
		glBindTexture(GL_TEXTURE_2D, textureID7111);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		matrix73 = glm::translate(temp5, glm::vec3(0.0268f, 0.52f, 0.0f));
		temp5 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(0.001f, 0.18f, 0.17f));
		glBindTexture(GL_TEXTURE_2D, textureID7112);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);


		matrix73 = glm::translate(temp5, glm::vec3(0.0f, 0.0f, 0.17f));
		temp5 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(0.001f, 0.18f, 0.17f));
		glBindTexture(GL_TEXTURE_2D, textureID7113);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);


		//glm::mat4 matrixAirCraft7173 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
		matrixAirCraft7173 = glm::translate(temp5, glm::vec3(0.0, 0.1, 0.4));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(90.0f), glm::vec3(0, 1, 0));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelac.setScale(glm::vec3(0.02, 0.009, 0.0014));
		glBindTexture(GL_TEXTURE_2D, textureID7118);
		modelac.setShader(&shaderTexture);
		modelac.setProjectionMatrix(projection);
		modelac.setViewMatrix(view);
		//modelac.render(matrixAirCraft7173);



		matrixAirCraft7173 = glm::translate(matrix74, glm::vec3(0.0, -1.4, 6.7));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(180.0f), glm::vec3(0, 1, 0));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmueble.setScale(glm::vec3(0.3, 0.5, 0.14));
		glBindTexture(GL_TEXTURE_2D, textureID7119);
		modelmueble.setShader(&shaderTexture);
		modelmueble.setProjectionMatrix(projection);
		modelmueble.setViewMatrix(view);
		//modelmueble.render(matrixAirCraft7173);

		//VENTANAS

		matrix73 = glm::translate(matrix74, glm::vec3(3.49f, 0.60f, 3.5f));
		temp6 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(0.01f, 1.8f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7114);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		matrix73 = glm::translate(temp6, glm::vec3(-0.01f, 0.0f, 2.5f));
		matrix73 = glm::scale(matrix73, glm::vec3(0.01f, 1.7f, 2.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7116);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		matrix73 = glm::translate(temp6, glm::vec3(-0.01f, 0.0f, 0.3f));
		matrix73 = glm::scale(matrix73, glm::vec3(0.01f, 1.7f, 2.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7116);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		matrix73 = glm::translate(temp6, glm::vec3(-0.01f, 0.0f, -2.2f));
		matrix73 = glm::scale(matrix73, glm::vec3(0.01f, 1.7f, 2.3f));
		glBindTexture(GL_TEXTURE_2D, textureID7116);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		//cemento


		matrix73 = glm::translate(matrix74, glm::vec3(3.49f, -0.877f, 3.5f));
		temp6 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(0.01f, 1.2f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7115);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);


		matrix73 = glm::translate(temp6, glm::vec3(-0.01f, 0.54f, 0.0f));
		temp6 = matrix73;
		matrix73 = glm::scale(matrix73, glm::vec3(0.1f, 0.3f, 7.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7115);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 36, matrix73);

		//PIZARRON

		matrix71 = glm::translate(matrix74, glm::vec3(0.0f, 0.5f, 0.02f));
		temp2 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(2.2f, 1.5f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID76);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		//box012.render(0, 24, matrix71);


		//mesas----------------------------------------------------------------------------------------

		matrix71 = glm::translate(matrix74, glm::vec3(1.4f, -0.8f, 1.5f));
		temp3 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(3.8f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID71);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//computadoras

		matrix71 = glm::translate(temp3, glm::vec3(1.77f, 0.27f, 0.0f));
		temp8 = temp4 = matrix71;
		matrix71 = matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box013.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box013.setProjectionMatrix(projection);
		box013.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);


		//monitor

		//modelmonitor.setScale(glm::vec3(1.20, 1.18, 1.35));
		glm::mat4 matrixAirCraft7172 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);


		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);


		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla

		glm::mat4 matrixAirCraft71 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);



		//2

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);


		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);


		//3

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);


		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);


		//4

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//patas
		//derecha

		matrix71 = glm::translate(temp3, glm::vec3(1.8f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//izq

		matrix71 = glm::translate(temp3, glm::vec3(-1.8f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);



		//mesa 2  

		matrix71 = glm::translate(matrix74, glm::vec3(-2.35f, -0.8f, 1.5f));
		temp3 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(2.3f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID71);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//computadoras

		matrix71 = glm::translate(temp3, glm::vec3(-1.025f, 0.31f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);


		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);






		//2

		matrix71 = glm::translate(temp7, glm::vec3(0.38f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.20f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);


		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//3

		matrix71 = glm::translate(temp7, glm::vec3(0.38f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);


		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);



		//patas
		//derecha

		matrix71 = glm::translate(temp3, glm::vec3(1.05f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//izq

		matrix71 = glm::translate(temp3, glm::vec3(-1.05f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);


		//mesa 3 

		matrix71 = glm::translate(matrix74, glm::vec3(-2.35f, -0.8f, 3.5f));
		temp3 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(2.3f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID71);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//computadoras

		matrix71 = glm::translate(temp3, glm::vec3(-1.025f, 0.31f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);


		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//2

		matrix71 = glm::translate(temp7, glm::vec3(0.38f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.20f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);


		//3

		matrix71 = glm::translate(temp7, glm::vec3(0.38f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);
		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);



		//patas
		//derecha

		matrix71 = glm::translate(temp3, glm::vec3(1.05f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//izq

		matrix71 = glm::translate(temp3, glm::vec3(-1.05f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);



		//mesa 4

		matrix71 = glm::translate(matrix74, glm::vec3(1.4f, -0.8f, 3.5f));
		temp3 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(3.8f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID71);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//computadoras

		matrix71 = glm::translate(temp3, glm::vec3(1.77f, 0.31f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);
		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);


		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);



		//2

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//3

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);
		//4

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//patas
		//derecha

		matrix71 = glm::translate(temp3, glm::vec3(1.8f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//izq

		matrix71 = glm::translate(temp3, glm::vec3(-1.8f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);


		//mesa 5

		matrix71 = glm::translate(matrix74, glm::vec3(1.4f, -0.8f, 5.5f));
		temp3 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(3.8f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID71);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);


		//computadoras

		matrix71 = glm::translate(temp3, glm::vec3(1.77f, 0.31f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);



		//2

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//3

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);
		//4

		matrix71 = glm::translate(temp7, glm::vec3(-0.55f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.38f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(-0.45f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(-0.45f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//patas
		//derecha

		matrix71 = glm::translate(temp3, glm::vec3(1.8f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//izq

		matrix71 = glm::translate(temp3, glm::vec3(-1.8f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);





		//mesa 6 

		matrix71 = glm::translate(matrix74, glm::vec3(-2.35f, -0.8f, 5.5f));
		temp3 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(2.3f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID71);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//computadoras

		matrix71 = glm::translate(temp3, glm::vec3(-1.025f, 0.31f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);


		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);


		//2

		matrix71 = glm::translate(temp7, glm::vec3(0.38f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.20f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);

		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);

		//3

		matrix71 = glm::translate(temp7, glm::vec3(0.38f, 0.0f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.25f, 0.47f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, textureID75);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box013.render(0, 6, matrix71);

		glBindTexture(GL_TEXTURE_2D, textureID717);
		box013.render(0, 36, matrix71);



		//monitor

		matrixAirCraft7172 = glm::translate(temp4, glm::vec3(0.38f, 0.0f, 0.0f));
		temp7 = matrixAirCraft7172;
		modelmonitor.setScale(glm::vec3(0.30, 0.45, 0.4));
		modelmonitor.setShader(&shaderTexture);
		modelmonitor.setProjectionMatrix(projection);
		modelmonitor.setViewMatrix(view);
		modelmonitor.render(matrixAirCraft7172);

		//teclado

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(-0.1, -0.21, 0.32));
		matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modeldesk.setScale(glm::vec3(0.007, 0.007, 0.01));
		modeldesk.setShader(&shaderTexture);
		modeldesk.setProjectionMatrix(projection);
		modeldesk.setViewMatrix(view);
		modeldesk.render(matrixAirCraft7173);

		//mouse

		matrixAirCraft7173 = glm::translate(temp7, glm::vec3(0.2, -0.21, 0.32));
		//matrixAirCraft7173 = glm::rotate(matrixAirCraft7173, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelmouse.setScale(glm::vec3(0.02, 0.02, 0.02));
		glBindTexture(GL_TEXTURE_2D, textureID7117);
		modelmouse.setShader(&shaderTexture);
		modelmouse.setProjectionMatrix(projection);
		modelmouse.setViewMatrix(view);
		modelmouse.render(matrixAirCraft7173);

		matrix71 = glm::translate(temp4, glm::vec3(0.38f, 0.06f, 0.04f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.45f, 0.30f, 0.01f));
		glBindTexture(GL_TEXTURE_2D, textureID718);
		box012.setShader(&shaderTexture);//Para luz con sombras se utiliza el shader direccional
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 6, matrix71);


		//modelo silla
		matrixAirCraft71 = glm::translate(temp4, glm::vec3(0.0, -1.0, 0.7));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(180.0f), glm::vec3(0, 1, 0));
		matrixAirCraft71 = glm::rotate(matrixAirCraft71, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelsilla.setScale(glm::vec3(0.028, 0.018, 0.035));
		modelsilla.setShader(&shaderTexture);
		modelsilla.setProjectionMatrix(projection);
		modelsilla.setViewMatrix(view);
		modelsilla.render(matrixAirCraft71);





		//patas
		//derecha

		matrix71 = glm::translate(temp3, glm::vec3(1.05f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		//izq

		matrix71 = glm::translate(temp3, glm::vec3(-1.05f, -0.1f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.1f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.24f, 0.0f));
		temp4 = matrix71;
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.55f, 0.15f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);

		matrix71 = glm::translate(temp4, glm::vec3(0.0f, -0.3f, 0.0f));
		matrix71 = glm::scale(matrix71, glm::vec3(0.1f, 0.05f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID74);
		box012.setShader(&shaderTexture);
		box012.setProjectionMatrix(projection);
		box012.setViewMatrix(view);
		box012.render(0, 36, matrix71);


		//_------------------------------------
		avion.setShader(&shaderTexture);
		avion.setProjectionMatrix(projection);
		avion.setViewMatrix(view);
		avion.setScale(glm::vec3(0.001, 0.001, 0.001));
		glm::mat4  matrixavion = glm::translate(tempavion, glm::vec3(movX, 0.0f, movZ));
		
		//matrixavion = glm::rotate(matrixavion, glm::radians(90.0f), glm::vec3(0, 0, 1));
		matrixavion = glm::translate(matrixavion, glm::vec3(10.0f, 20.0f, 0.0f));
		matrixavion = glm::rotate(matrixavion, rotavion, glm::vec3(0, 1, 0));
		matrixavion = glm::rotate(matrixavion, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		avion.render(matrixavion);
		/**************************ANIMACIONES**********************************/
		//Modelo de perro
		perro.setShader(&shaderLighting);
		perro.setProjectionMatrix(projection);
		perro.setViewMatrix(view);
		//perro.setPosition(glm::vec3(15.0, 0.0, 10.0));
		perro.setScale(glm::vec3(0.01, 0.01, 0.01));
		perro.setOrientation(glm::vec3(0.0, -90.0, -90.0));
		glm::mat4 matrixPerro = glm::translate(glm::mat4(1.0f), glm::vec3(perroZ, 0.0, -10.0));
		matrixPerro = glm::translate(matrixPerro, glm::vec3(13.0, 0.0, 10.0));//Cambiar el valor de glm::vec3 para la posicion
		matrixPerro = glm::rotate(matrixPerro, rotacionPerro, glm::vec3(0, 1, 0));
		perro.render(matrixPerro);

		//Modelo de avion2
		avion2.setShader(&shaderLighting);
		avion2.setProjectionMatrix(projection);
		avion2.setViewMatrix(view);
		avion2.setScale(glm::vec3(0.001, 0.001, 0.001));
		avion2.setOrientation(glm::vec3(-90.0, 0.0, 0.0));
		glm::mat4 matrixAvion2 = glm::translate(glm::mat4(1.0f), glm::vec3(avion2X, avion2Y, 0.0));
		matrixAvion2 = glm::translate(matrixAvion2, glm::vec3(-10.0, 25.0, 10.0));//Cambiar los valores de glm::vec3 para la posicion
		matrixAvion2 = glm::rotate(matrixAvion2, rotacionavion, glm::vec3(0, 1, 0));
		avion2.render(matrixAvion2);

		//Modelo de pajaro2
		pajaro2.setShader(&shaderLighting);
		pajaro2.setProjectionMatrix(projection);
		pajaro2.setViewMatrix(view);
		//pajaro2.setPosition(glm::vec3(5.0, 0.0, 10.0));
		pajaro2.setScale(glm::vec3(0.03, 0.03, 0.03));
		pajaro2.setOrientation(glm::vec3(0.0, -90.0, -90.0));
		glm::mat4 matrixPajaro = glm::translate(glm::mat4(1.0f), glm::vec3(5.0, pajaroY, pajaroZ-3));
		matrixPajaro = glm::translate(matrixPajaro, glm::vec3(5.0, 0.0, 10.0)); //CAmbiar los valores de glm::vec3 para poner el modelo en la posicion deseada
		matrixPajaro = glm::rotate(matrixPajaro, rotationPajaro, glm::vec3(0, 1, 0));
		pajaro2.render(matrixPajaro);

		//Modelo de pajaro1
		pajaro2.setShader(&shaderLighting);
		pajaro2.setProjectionMatrix(projection);
		pajaro2.setViewMatrix(view);
		//pajaro2.setPosition(glm::vec3(7.0, 0.0, 20.0));
		pajaro2.setScale(glm::vec3(0.03, 0.03, 0.03));
		pajaro2.setOrientation(glm::vec3(0.0, 90.0, 90.0));
		glm::mat4 matrixPajaro2 = glm::translate(glm::mat4(1.0f), glm::vec3(5.0, pajaroY, -pajaroZ-10));
		matrixPajaro2 = glm::translate(matrixPajaro2, glm::vec3(7.0, 0.0, 20.0)); //CAmbiar los valores de glm::vec3 para poner el modelo en la posicion deseada
		matrixPajaro2 = glm::rotate(matrixPajaro2, rotationPajaro, glm::vec3(0, 1, 0));
		pajaro2.render(matrixPajaro2);

		//Variables para hacer la animacion por keyframes
		glm::quat firstQuat;
		glm::quat secondQuat;
		glm::quat finalQuat;
		glm::mat4 interpoltaedMatrix;
		glm::vec4 transformComp1;
		glm::vec4 transformComp2;
		glm::vec4 finalTrans;

		//Modelo del  auto
		audi.setShader(&shaderLighting);
		audi.setProjectionMatrix(projection);
		audi.setViewMatrix(view);
		audi.setScale(glm::vec3(0.01, 0.01, 0.01));
		audi.setOrientation(glm::vec3(0.0, -90.0, -90.0));

		//Matriz de rotacion actual						//Frame		  /Matriz
		firstQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoCurr][0]);
		secondQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoNext][0]);
		//slerp hace la interpolacion del quaternion (Matriz de rotacion)
		finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
		//Se convierte el quaternion a una matriz de 4x4
		interpoltaedMatrix = glm::mat4_cast(finalQuat);
		//Se obtiene la translacion del frame i-1
		transformComp1 = keyFramesBrazo[indexKeyFrameBrazoCurr][0] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//Se obtiene la trnaslacion del frame i
		transformComp2 = keyFramesBrazo[indexKeyFrameBrazoNext][0] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//Se realiza la interpolacion entre el frame i-1 y el frame i
		//tranformComp1 es el frame i-1
		//tranformComp1 es el frame i
		finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
		//Unimos la matriz de interpolacion del quaternion y la iterpolacionde la traslacion
		interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;

		// Animacion KeyFrames
		glm::mat4 matrixGlobalAnimation = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
		// Se modela siempre con los ejes de giro en el eje z
		glm::mat4 keyFrameJoint = matrixGlobalAnimation * interpoltaedMatrix;
		glm::mat4 matrixAuto = glm::translate(keyFrameJoint, glm::vec3(0.0, 0.0, 0.0));
		matrixAuto = glm::translate(matrixAuto, glm::vec3(1.0, 0.0, 0.25));//Cambiar el valor de glm::vec3 para la posicion
		matrixAuto = glm::rotate(matrixAuto, 0.0f, glm::vec3(0, 1, 0));
		audi.render(matrixAuto);

		numPasosAnimBrazoCurr++;
		interpolation = numPasosAnimBrazoCurr / (float)numPasosAnimBrazo;

		if (interpolation >= 1.0) {
			interpolation = 0.0;
			numPasosAnimBrazoCurr = 0;
			indexKeyFrameBrazoCurr = indexKeyFrameBrazoNext;
			indexKeyFrameBrazoNext++;
		}

		if (indexKeyFrameBrazoNext > keyFramesBrazo.size() - 1) {
			interpolation = 0.0;
			indexKeyFrameBrazoCurr = 0;
			indexKeyFrameBrazoNext = 1;
		}


		//MARIPOSA

		temp272 = glm::translate(temp272, glm::vec3(xm+7.0f, ym, -5.0f));
	//	temp272 = glm::translate(temp272, glm::vec3(xm, 0.0f, 0.0f));
		modelmariposa.setScale(glm::vec3(0.01, 0.01, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		modelmariposa.setShader(&shaderTexture);
		modelmariposa.setProjectionMatrix(projection);
		modelmariposa.setViewMatrix(view);
		modelmariposa.render(temp272);
		/****************************************************************/


		// Se Dibuja el Skybox
		shaderCubeTexture.turnOn();
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
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


		if (a) {
			xm = 5 + 5 * sin(2 * tm) / 2;
			ym = 7 + 5 * sin(tm);
			tm += 0.015;
		}

		if (rotacionoff) {
			if (inicio) {
				movX += 0.1;
				movZ = sqrt(pow(radio, 2) - pow(movX, 2));
			}
			else {
				movX -= 0.1;
				movZ = -sqrt(pow(radio, 2) - pow(movX, 2));
			}
			if (inicio && movX >= 20.0) {
				inicio = false;
				rotacionoff =false;
				movX = 20.0;
				movZ = sqrt(pow(radio, 2) - pow(movX, 2));
			}
			if (!inicio && movX <= -20.0) {
				inicio = true;
				rotacionoff = false;
				movX = -20.0;
				movZ = -sqrt(pow(radio, 2) - pow(movX, 2));
			}
		}
		else {
			rotavion += 0.1;
			if (inicio) {
				if (rotavion > glm::radians(90.0f)) {
					rotacionoff = true;
					rotavion = glm::radians(90.0f);
				}
			}
			else {
				if (rotavion > glm::radians(270.0f)) {
					rotacionoff = true;
					rotavion = glm::radians(270.0f);
				}
			}
		}
		/********************MAQUINAS DE ESTADO ANIMACIONES****************************/
		//Animacion para el avion2

		if (avion2Y >= 0.0f && ang >= 0.1) {
			avion2X = Vini * t* cos(ang);
			if (bandera == 1) {
				avion2X = temp - avion2X;
			}
			avion2Y = Vini * t* sin(ang) - 0.5*g*t*t;
			t += 0.0005f;
		}


		else {
			rotacionavion += 0.1;
			if (bandera == 0) {
				if (rotacionavion > glm::radians(180.0f)) {
					rotacionavion = glm::radians(180.0f);
					t = 0.0;
					avion2Y = 0.0;
					bandera = 1;
					temp = avion2X;
				}

			}
			else {
				if (rotacionavion > glm::radians(360.0f)) {
					rotacionavion = glm::radians(0.0f);
					t = 0.0;
					temp = 0.0;
					avion2Y = 0.0;
					bandera = 0;
				}
			}

		}





		//Se crea el movimiento de las aves
		if (finishRotation2) {
			if (direcionPajaro) {
				pajaroY += 0.03;
				pajaroZ -= 0.03;
				if (pajaroY > 0.05)
					pajaroY = 0.0;

			}

			else {
				pajaroY += 0.03;
				pajaroZ += 0.03;
				if (pajaroY > 0.05)
					pajaroY = 0.0;

			}

			if (direcionPajaro && pajaroZ < -1.0) {
				direcionPajaro = false;
				finishRotation2 = false;
				pajaroZ = -1.0;
			}if (!direcionPajaro && pajaroZ > 1.0) {
				direcionPajaro = true;
				finishRotation2 = false;
				pajaroZ = 1.0;
			}
		}
		else {
			rotationPajaro += 0.2;
			if (!direcionPajaro) {
				if (rotationPajaro > glm::radians(180.0f)) {
					finishRotation2 = true;
					rotationPajaro = glm::radians(180.0f);
				}
			}
			else {
				if (rotationPajaro > glm::radians(360.0f)) {
					finishRotation2 = true;
					rotationPajaro = glm::radians(0.0f);
				}
			}
		}

		//Genera el movmiento del perro
		if (finishRotation3) {
			if (direcionPerro) {
				perroZ -= 0.1;

			}

			else {
				perroZ += 0.1;

			}

			if (direcionPerro && perroZ < -3.0) {
				direcionPerro = false;
				finishRotation3 = false;
				perroZ = -3.0;
			}if (!direcionPerro && perroZ > 3.0) {
				direcionPerro = true;
				finishRotation3 = false;
				perroZ = 3.0;
			}
		}
		else {
			rotacionPerro += 0.1;
			if (!direcionPerro) {
				if (rotacionPerro > glm::radians(180.0f)) {
					finishRotation3 = true;
					rotacionPerro = glm::radians(180.0f);
				}
			}
			else {
				if (rotacionPerro > glm::radians(360.0f)) {
					finishRotation3 = true;
					rotacionPerro = glm::radians(0.0f);
				}
			}
		}
			glfwSwapBuffers(window);
	}
}


int main(int argc, char ** argv) {
	init(1500, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	
	return 1;
}








