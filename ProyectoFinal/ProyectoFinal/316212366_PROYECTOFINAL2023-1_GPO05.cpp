#include <iostream>
#include <cmath>
#include <random>
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"
#include "irrKlang.h"

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

ISoundEngine* SoundEngine = createIrrKlangDevice();



// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.5f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

//Animación Puerta
float AnimPuerta = 0.0f;
bool AbrirPuerta = false;
bool CerrarPuerta = false;
bool Dopen = false;

//Animación Biombo
float rotar = 0.0f; //Desplazamiento horizontal de los extremos = 0.9f, desplazamiento de partes centrales = 0.27f
float trasExt = 0.0f;
float trasCent = 0.0f;
bool plegar = false;
bool desplegar = false;
bool plegada = false;

//Animacion petalos
float tiempo = 1;
float speed = 4;
float desp = 0;
float desp2 = 0;

//Animacón Paimon
bool baile = false;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-9.305f,0.1313f, -0.49f),
	glm::vec3(-8.42f,0.1313f, -0.49f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);

glm::vec3 spotlightP = glm::vec3(2.0, 1.0, -2.0);
glm::vec3 spotlightD = glm::vec3(1.0, 0.0, 0.0);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "316212366_PROYECTOFINAL2023-1_GPO05", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader Animacion("Shaders/Anima.vs", "Shaders/Anima.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");

	Model Piso((char*)"Models/PisoExt/PisoExt.obj");
	Model Fachada((char*)"Models/Fachada/Fachada.obj");
	Model Interior((char*)"Models/Interior.obj");

	//Biombo
	Model BiomIzq((char*)"Models/Biombo/Left.obj");
	Model BiomIzqM((char*)"Models/Biombo/LeftMid.obj");
	Model BiomDerM((char*)"Models/Biombo/RightMid.obj");
	Model BiomDer((char*)"Models/Biombo/Right.obj");

	//Objetos
	Model Cama((char*)"Models/Cama.obj");
	Model Closet((char*)"Models/Closet.obj");
	Model Cojin((char*)"Models/Cojin.obj");
	Model Coral((char*)"Models/Coral.obj");
	Model Florero((char*)"Models/Florero.obj");
	Model Kotatsu((char*)"Models/Kotatsu.obj");

	Model Biombo2((char*)"Models/Biombo2/Biombo2.obj");
	Model Mesa((char*)"Models/Mesa/Mesa.obj");
	Model Vela((char*)"Models/Vela/Vela.obj");

	Model Puerta((char*)"Models/Puerta.obj");

	Model Flor((char*)"Models/Flor.obj");

	//Animación personaje
	ModelAnim animacionPersonaje("Models/PaimonBaile/Flair.dae");
	animacionPersonaje.initShaders(animShader.Program);
	ModelAnim animacionPersonaje2("Models/Paimon/Standing Greeting.dae");
	animacionPersonaje2.initShaders(animShader.Program);

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;

	faces.push_back("SkyBox/right.png");
	faces.push_back("SkyBox/left.png");
	faces.push_back("SkyBox/top.png");
	faces.push_back("SkyBox/bottom.png");
	faces.push_back("SkyBox/back.png");
	faces.push_back("SkyBox/front.png");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	SoundEngine->play2D("KitsunesMask.mp3", true);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{


		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.7f, 0.58f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.7f, 0.58f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x) * 2);
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		if (lightColor.y < 0.5f && active) {
			lightColor.y = 0.5f;
			lightColor.x = 1.0f;
		}
		lightColor.z = Light1.z;


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.95f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 3.75f);



		// Point light 2
		glm::vec3 lightColor2;
		lightColor2.x = abs(sin(glfwGetTime() * Light2.x) * 2);
		lightColor2.y = abs(sin(glfwGetTime() * Light2.y));
		if (lightColor2.y < 0.5f && active) {
			lightColor2.y = 0.5f;
			lightColor2.x = 1.0f;
		}

		lightColor2.z = Light2.z;

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.95f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 3.75f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), spotlightP.x, spotlightP.y, spotlightP.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotlightD.x, spotlightD.y, spotlightD.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activTransparencia"), 1.0f);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		Piso.Draw(lightingShader);

		//Box.Draw(lightingShader);
		//Foco.Draw(lightingShader);

		//Fachada
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activTransparencia"), 0.0f);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Fachada.Draw(lightingShader);

		//Interior
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activTransparencia"), 0.0f);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Interior.Draw(lightingShader);

		Cama.Draw(lightingShader);
		Closet.Draw(lightingShader);
		Cojin.Draw(lightingShader);
		Coral.Draw(lightingShader);
		Florero.Draw(lightingShader);
		Kotatsu.Draw(lightingShader);

		Biombo2.Draw(lightingShader);
		Mesa.Draw(lightingShader);
		Vela.Draw(lightingShader);

		/* Biombo 1 */
		//Parte izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.412f, 1.518f, 0.889f - trasExt));
		model = glm::rotate(model, glm::radians(-rotar), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BiomIzq.Draw(lightingShader);



		//Parte medio-izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.413f, 1.581f, 0.1477f - trasCent));
		model = glm::rotate(model, glm::radians(rotar), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BiomIzqM.Draw(lightingShader);


		//Parte medio-derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.413f, 1.691f, -0.533f + trasCent));
		model = glm::rotate(model, glm::radians(-rotar), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BiomDerM.Draw(lightingShader);


		//Parte derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.412f, 1.46f, -1.277f + trasExt));
		model = glm::rotate(model, glm::radians(rotar), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BiomDer.Draw(lightingShader);




		/* Puertas Cuarto1*/
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.15f - AnimPuerta, 1.89f, 3.82f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.135f + AnimPuerta, 1.89f, 3.82f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		/* Puertas Cuarto2*/
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.735f - AnimPuerta, 1.89f, -5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.72f + AnimPuerta, 1.89f, -5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		glBindVertexArray(0);


		glBindVertexArray(0);
		Animacion.Use();
		tiempo = glfwGetTime();

		modelLoc = glGetUniformLocation(Animacion.Program, "model");
		viewLoc = glGetUniformLocation(Animacion.Program, "view");
		projLoc = glGetUniformLocation(Animacion.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Flor 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.48f - desp2, 1.26f - desp, -11.3523f ));
		glUniform1f(glGetUniformLocation(Animacion.Program, "time"), tiempo);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Flor.Draw(Animacion);
		//Flor 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.48f + desp2, 1.21f - desp - desp2, -11.394f ));
		glUniform1f(glGetUniformLocation(Animacion.Program, "time"), tiempo);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Flor.Draw(Animacion);
		//Flor 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.48f - desp2, 1.173f - desp - desp2, -11.461f ));
		glUniform1f(glGetUniformLocation(Animacion.Program, "time"), tiempo);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Flor.Draw(Animacion);
		//Flor 4
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.48f + desp2, 1.188f - desp, -11.524f ));
		glUniform1f(glGetUniformLocation(Animacion.Program, "time"), tiempo);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Flor.Draw(Animacion);
		glBindVertexArray(0);

		/*_______________________________Personaje Animado___________________________*/
		animShader.Use();
		modelLoc = glGetUniformLocation(animShader.Program, "model");
		viewLoc = glGetUniformLocation(animShader.Program, "view");
		projLoc = glGetUniformLocation(animShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(animShader.Program, "material.specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(animShader.Program, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.specular"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.direction"), 0.0f, -1.0f, -1.0f);
		view = camera.GetViewMatrix();

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.5f, 0.66f, -3.7f));
		model = glm::scale(model, glm::vec3(0.14f));	// it's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (baile) {
			animacionPersonaje.Draw(animShader);
		}
		else {
			animacionPersonaje2.Draw(animShader);
			
		}
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 2; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glBindVertexArray(0);

		// Draw skybox as last
			glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    // Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content 
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    // Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content 
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);

		
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (AbrirPuerta) {

		if (AnimPuerta < 1.0) {
			AnimPuerta += 0.01f;
		}
		else {
			AbrirPuerta = false;
			Dopen = true;
		}

	}
	if (CerrarPuerta) {
		if (AnimPuerta > 0.0f) {
			AnimPuerta -= 0.01f;
		}
		if (AnimPuerta < 0.0f) {
			CerrarPuerta = false;
			Dopen = false;
		}

	}
	if (plegar) { //Desplazamiento horizontal de los extremos = 0.9f, desplazamiento de partes centrales = 0.27f

		if (rotar < 90.0f) {
			rotar += 1.6f;
			if (trasExt  < 0.9f) {
				trasExt += 0.015f;
			}
			if (trasCent < 0.27f) {
				trasCent += 0.005f;
			}			
			
		}
		if(rotar > 90.0f) {
			plegar = false;
			plegada = true;
		}

	}
	if (desplegar) {
		if ( rotar > 0.0f) {
			rotar -= 1.7f;
			if (trasExt > 0.0f) {
				trasExt -= 0.02f;
				if (trasCent > 0.0f) {
					trasCent -= 0.05f;
				}
			}
			
		}
		if (rotar < 0.0f) {
			desplegar = false;
			plegada = false;
		}

	}
	
	if (desp<0.54) {
		desp += 0.0008;
		desp2 += 0.00075;
		
	}
	if (desp > 0.54) {
		desp = 0.0;
		desp2 = 0.0;
		
	}
	
	

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
			Light2 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			Light2 = glm::vec3(0);
		}

		

	}

	if (keys[GLFW_KEY_O]) {
		AbrirPuerta = true;
		CerrarPuerta = false;
	}

	if (keys[GLFW_KEY_O] && Dopen == true)
	{
		CerrarPuerta = true;
		AbrirPuerta = false;
	}

	if (keys[GLFW_KEY_P]) {
		plegar = true;
		desplegar = false;
	}

	if (keys[GLFW_KEY_P] && plegada == true)
	{
		desplegar = true;
		plegar = false;
	}

	if (keys[GLFW_KEY_B])
	{
		baile = !baile;
	}


}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}