#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader("6.multiple_lights.vs", "6.multiple_lights.fs");
	Shader lampShader("lamp.vs", "lamp.fs");
	Shader modelShader("model.vs", "model.fs");
	Shader transShader("transparent.vs", "transparent.fs");

	// Model ourModel("resources/objects/nanosuit/nanosuit.obj");
	// Model ourModel("resources/objects/Blue_Arisis/Blue_Arisis.pmx");
	// Model ourModel("resources/objects/meme_shi_skyfire_Obsidian_ver/meme_shi_Skyfire_Obsidian_ver.pmx");
	// Model ourModel("resources/objects/liliyalin/liliyalinloli.pmx");
	Model ourModel("resources/objects/losliya/losliya.pmx");
	//Model ourModel("resources/objects/zhezhi/fanzhuanzhezhi.pmx");



	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	 float vertices[] = {
		 // positions          // normals           // texture coords
		 // positions          // normals           // texture coords
		 -0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		  0.5f,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		  0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		  0.5f,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 -0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 -0.5f,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

		 -0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		  0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		  0.5f,   0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		  0.5f,   0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 -0.5f,   0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		 -0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

		 -0.5f,   0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 -0.5f,   0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 -0.5f,  -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 -0.5f,  -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 -0.5f,  -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 -0.5f,   0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		  0.5f,   0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		  0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		  0.5f,   0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		  0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		  0.5f,   0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		  0.5f,  -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		 -0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		  0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		  0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		  0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 -0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		 -0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		 -0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		  0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		  0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		  0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 -0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 -0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
	 };
	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f, 0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, 2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, 3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, 2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, 1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	vector<glm::vec3> vegetation
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// transparent VAO
	unsigned int transparentVAO, transparentVBO;
	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);
	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap = loadTexture("resources/textures/container2.png");
	unsigned int specularMap = loadTexture("resources/textures/container2_specular.png");
	unsigned int transparentTexture = loadTexture("resources/textures/blending_transparent_window.png");

	// glBindTexture(GL_TEXTURE_2D, transparentTexture);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glBindTexture(GL_TEXTURE_2D, 0);

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);

	transShader.use();
	transShader.setInt("texture3", 2);

	// be sure to activate shader when setting uniforms/drawing objects
	lightingShader.use();

	// lightingShader.setVec3("dirLight.direction", glm::vec3(0.0f, 00.0f, -1.0f));
	// lightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	// lightingShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
	// lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

	// lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	// lightingShader.setVec3("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
	// lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	// lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	// lightingShader.setFloat("pointLights[0].constant", 1.0f);
	// lightingShader.setFloat("pointLights[0].linear", 0.09f);
	// lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

	// lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	// lightingShader.setVec3("pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
	// lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	// lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	// lightingShader.setFloat("pointLights[1].constant", 1.0f);
	// lightingShader.setFloat("pointLights[1].linear", 0.09f);
	// lightingShader.setFloat("pointLights[1].quadratic", 0.032f);

	// lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	// lightingShader.setVec3("pointLights[2].ambient", 0.2f, 0.2f, 0.2f);
	// lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	// lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	// lightingShader.setFloat("pointLights[2].constant", 1.0f);
	// lightingShader.setFloat("pointLights[2].linear", 0.09f);
	// lightingShader.setFloat("pointLights[2].quadratic", 0.032f);

	// lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	// lightingShader.setVec3("pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
	// lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	// lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	// lightingShader.setFloat("pointLights[3].constant", 1.0f);
	// lightingShader.setFloat("pointLights[3].linear", 0.09f);
	// lightingShader.setFloat("pointLights[3].quadratic", 0.032f);

	lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.1f, 0.05f);
	lightingShader.setVec3("pointLights[0].diffuse", 0.1f, 0.8f, 0.2f);
	lightingShader.setVec3("pointLights[0].specular", 0.2f, 1.0f, 0.3f);
	lightingShader.setFloat("pointLights[0].constant", 1.0f);
	lightingShader.setFloat("pointLights[0].linear", 0.09);
	lightingShader.setFloat("pointLights[0].quadratic", 0.032);
	// point light 2
	lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.1f, 0.05f);
	lightingShader.setVec3("pointLights[1].diffuse", 0.1f, 0.8f, 0.2f);
	lightingShader.setVec3("pointLights[1].specular", 0.2f, 1.0f, 0.3f);
	lightingShader.setFloat("pointLights[1].constant", 1.0f);
	lightingShader.setFloat("pointLights[1].linear", 0.09);
	lightingShader.setFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.1f, 0.05f);
	lightingShader.setVec3("pointLights[2].diffuse", 0.1f, 0.8f, 0.2f);
	lightingShader.setVec3("pointLights[2].specular", 0.2f, 1.0f, 0.3f);
	lightingShader.setFloat("pointLights[2].constant", 1.0f);
	lightingShader.setFloat("pointLights[2].linear", 0.09);
	lightingShader.setFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.1f, 0.05f);
	lightingShader.setVec3("pointLights[3].diffuse", 0.1f, 0.8f, 0.2f);
	lightingShader.setVec3("pointLights[3].specular", 0.2f, 1.0f, 0.3f);
	lightingShader.setFloat("pointLights[3].constant", 1.0f);
	lightingShader.setFloat("pointLights[3].linear", 0.09);
	lightingShader.setFloat("pointLights[3].quadratic", 0.032);


	// framebuffer configuration
		// -------------------------
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.5f,  1.5f,  0.0f, 1.0f,
		-1.5f, -1.5f,  0.0f, 0.0f,
		 1.5f, -1.5f,  1.0f, 0.0f,

		-1.5f,  1.5f,  0.0f, 1.0f,
		 1.5f, -1.5f,  1.0f, 0.0f,
		 1.5f,  1.5f,  1.0f, 1.0f
	};

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xff);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	
	Shader screenShader("screenShader.vs", "screenShader.fs");
	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	//GLdouble eqn[] = { 0.0f, 0.5f,0.0f,0.0f };
	glEnable(GL_CLIP_DISTANCE0);
	lightingShader.use();
	lightingShader.setVec4("clipPlaneCoeffs", glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	lightingShader.setBool("isClipPlane", false);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		// render
		// ------
		glClearColor(0.3f, 0.5f, 0.3f, 1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 mirrorView = camera.GetMirrorViewMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
		//mirrorView = camera.GetViewMatrix();

		
		//glDisable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
			screenShader.use();
			screenShader.setMat4("projection", projection);
			screenShader.setMat4("view", mirrorView);
			glBindVertexArray(quadVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, transparentTexture);	// use the color attachment texture as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);
		//glCullFace(GL_BACK);
		//glEnable(GL_CULL_FACE);
		
		//glDepthFunc(GL_GREATER);
							// glDepthFunc(GL_ALWAYS);
							// be sure to activate shader when setting uniforms/drawing objects
							lightingShader.use();
							lightingShader.setBool("isClipPlane", true);
							lightingShader.setVec3("spotLight.position", camera.Position);
							lightingShader.setVec3("spotLight.direction", camera.Front);
							lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
							lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
							lightingShader.setVec3("viewPos", camera.Position);

							// light properties
							lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
							// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
							// each environment and lighting type requires some tweaking to get the best out of your environment.
							lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
							lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
							lightingShader.setFloat("spotLight.constant", 1.0f);
							lightingShader.setFloat("spotLight.linear", 0.09f);
							lightingShader.setFloat("spotLight.quadratic", 0.032f);

							// material properties
							lightingShader.setFloat("material.shininess", 32.0f);


							lightingShader.setMat4("projection", projection);
							lightingShader.setMat4("view", mirrorView);

							// world transformation
							glm::mat4 model = glm::mat4(1.0f);
							lightingShader.setMat4("model", model);

							// bind diffuse map
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, diffuseMap);
							// bind specular map
							glActiveTexture(GL_TEXTURE1);
							glBindTexture(GL_TEXTURE_2D, specularMap);






							// render containers
							glBindVertexArray(cubeVAO);
							lightingShader.setFloat("textCoordBaseLine", (glfwGetTime() - (int)(glfwGetTime() / 1.0f)));
							//glDepthFunc(GL_LESS);
							//glStencilFunc(GL_ALWAYS, 1, 0xff);
							//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
							//glFrontFace(GL_CW);
							for (unsigned int i = 0; i < 10; i++)
							{
								// calculate the model matrix for each object and pass it to shader before drawing
								glm::mat4 model = glm::mat4(1.0f);
								model = glm::translate(model, cubePositions[i]);
								float angle = 20.0f * i;
								model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
								lightingShader.setMat4("model", model);

								glDrawArrays(GL_TRIANGLES, 0, 36);
							}
							// model = glm::mat4(1.0f);
							// model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.2f));
							// model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
							// lightingShader.setMat4("model", model);
							// ourModel.Draw(lightingShader);

							//glDepthFunc(GL_ALWAYS);
							//glStencilFunc(GL_EQUAL, 0, 0xff);
							//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
							// ÕâÊÇ»­Ãè±ßµÄ
							//lampShader.use();
							//lampShader.setMat4("projection", projection);
							//lampShader.setMat4("view", mirrorView);
							//for (unsigned int i = 0; i < 10; i++)
							//{
							//	// calculate the model matrix for each object and pass it to shader before drawing
							//	glm::mat4 model = glm::mat4(1.0f);
							//	model = glm::translate(model, cubePositions[i]);
							//	float angle = 20.0f * i;
							//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
							//	model = glm::scale(model, glm::vec3(1.03f, 1.03f, 1.03f));
							//	lampShader.setMat4("model", model);

							//	glDrawArrays(GL_TRIANGLES, 0, 36);
							//}
							// model = glm::mat4(1.0f);
							// model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.2f));
							// model = glm::scale(model, glm::vec3(0.21f, 0.21f, 0.21f));
							// lampShader.setMat4("model", model);
							// ourModel.Draw(lampShader);
							//glFrontFace(GL_CCW);
							//glDepthFunc(GL_LESS);
							//glStencilFunc(GL_ALWAYS, 1, 0xff);
							//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

							//glDepthFunc(GL_LESS);
							lightingShader.use();
							//glFrontFace(GL_CW);
							model = glm::mat4(1.0f);
							model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.2f));
							model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
							lightingShader.setMat4("view", mirrorView);
							lightingShader.setMat4("model", model);
							lightingShader.setMat4("projection", projection);
							ourModel.Draw(lightingShader);
							// also draw the lamp object(s)
							lampShader.use();
							lampShader.setMat4("projection", projection);
							lampShader.setMat4("view", mirrorView);

							// we now draw as many light bulbs as we have point lights.
							glBindVertexArray(lightVAO);
							for (unsigned int i = 0; i < 4; i++)
							{
								model = glm::mat4(1.0f);
								model = glm::translate(model, pointLightPositions[i]);
								model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
								lampShader.setMat4("model", model);
								glDrawArrays(GL_TRIANGLES, 0, 36);
							}

							// // don't forget to enable shader before setting uniforms
							// modelShader.use();
							// 
							// // view/projection transformations
							// 
							// modelShader.setMat4("projection", projection);
							// modelShader.setMat4("view", view);
							// 
							// 
							// modelShader.setMat4("model", model);

							glActiveTexture(GL_TEXTURE2);
							glBindTexture(GL_TEXTURE_2D, transparentTexture);
							// vegetation
							transShader.use();
							transShader.setMat4("projection", projection);
							transShader.setMat4("view", mirrorView);
							glBindVertexArray(transparentVAO);
		
							// std::map<float, glm::vec3> sorted;
							// for (unsigned int i = 0; i < vegetation.size(); i++)
							// {
							// 	float distance = glm::length(camera.Position - vegetation[i]);
							// 	sorted[distance] = vegetation[i];
							// }
							// for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
							// {
							// 	model = glm::mat4();
							// 	model = glm::translate(model, it->second);
							// 	transShader.setMat4("model", model);
							// 	glDrawArrays(GL_TRIANGLES, 0, 6);
							// }
							//glDepthMask(false);
							 for (GLuint i = 0; i < vegetation.size(); i++)
							 {
		 						model = glm::mat4(1.0f);
		 						model = glm::translate(model, vegetation[i]);
		 						transShader.setMat4("model", model);
		 						glDrawArrays(GL_TRIANGLES, 0, 6);
							 }
							//glDepthMask(true);
							 lightingShader.use();
							 lightingShader.setBool("isClipPlane", false);
		glDepthFunc(GL_LESS);
		// again, a lamp object is weird when we only have a spot light, don't render the light object
		// lampShader.use();
		// lampShader.setMat4("projection", projection);
		// lampShader.setMat4("view", view);
		// model = glm::mat4(1.0f);
		// model = glm::translate(model, lightPos);
		// model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		// lampShader.setMat4("model", model);

		// glBindVertexArray(lightVAO);
		// glDrawArrays(GL_TRIANGLES, 0, 36);

				// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);


















								glEnable(GL_DEPTH_TEST);
								// render
								// ------
								glClearColor(0.3f, 0.8f, 0.3f, 1.0f);
								glClearStencil(0);
								glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

								// glDepthFunc(GL_ALWAYS);
								// be sure to activate shader when setting uniforms/drawing objects
								lightingShader.use();
								lightingShader.setVec3("spotLight.position", camera.Position);
								lightingShader.setVec3("spotLight.direction", camera.Front);
								lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
								lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
								lightingShader.setVec3("viewPos", camera.Position);

								// light properties
								lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
								// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
								// each environment and lighting type requires some tweaking to get the best out of your environment.
								lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
								lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
								lightingShader.setFloat("spotLight.constant", 1.0f);
								lightingShader.setFloat("spotLight.linear", 0.09f);
								lightingShader.setFloat("spotLight.quadratic", 0.032f);

								// material properties
								lightingShader.setFloat("material.shininess", 32.0f);

								// view/projection transformations
								projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
								view = camera.GetViewMatrix();
								mirrorView = camera.GetMirrorViewMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
								mirrorView = camera.GetViewMatrix();
								lightingShader.setMat4("projection", projection);
								lightingShader.setMat4("view", mirrorView);

								// world transformation
								model = glm::mat4(1.0f);
								lightingShader.setMat4("model", model);

								// bind diffuse map
								glActiveTexture(GL_TEXTURE0);
								glBindTexture(GL_TEXTURE_2D, diffuseMap);
								// bind specular map
								glActiveTexture(GL_TEXTURE1);
								glBindTexture(GL_TEXTURE_2D, specularMap);



								// render containers
								glBindVertexArray(cubeVAO);

								glDepthFunc(GL_LESS);
								glStencilFunc(GL_ALWAYS, 1, 0xff);
								glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
								//glFrontFace(GL_CW);
								for (unsigned int i = 0; i < 10; i++)
								{
									// calculate the model matrix for each object and pass it to shader before drawing
									glm::mat4 model = glm::mat4(1.0f);
									model = glm::translate(model, cubePositions[i]);
									float angle = 20.0f * i;
									model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
									lightingShader.setMat4("model", model);

									glDrawArrays(GL_TRIANGLES, 0, 36);
								}
								// model = glm::mat4(1.0f);
								// model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.2f));
								// model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
								// lightingShader.setMat4("model", model);
								// ourModel.Draw(lightingShader);

								glDepthFunc(GL_ALWAYS);
								glStencilFunc(GL_EQUAL, 0, 0xff);
								glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

								lampShader.use();
								lampShader.setMat4("projection", projection);
								lampShader.setMat4("view", mirrorView);
								for (unsigned int i = 0; i < 10; i++)
								{
									// calculate the model matrix for each object and pass it to shader before drawing
									glm::mat4 model = glm::mat4(1.0f);
									model = glm::translate(model, cubePositions[i]);
									float angle = 20.0f * i;
									model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
									model = glm::scale(model, glm::vec3(1.03f, 1.03f, 1.03f));
									lampShader.setMat4("model", model);

									glDrawArrays(GL_TRIANGLES, 0, 36);
								}
								// model = glm::mat4(1.0f);
								// model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.2f));
								// model = glm::scale(model, glm::vec3(0.21f, 0.21f, 0.21f));
								// lampShader.setMat4("model", model);
								// ourModel.Draw(lampShader);
								//glFrontFace(GL_CCW);
								glDepthFunc(GL_LESS);
								glStencilFunc(GL_ALWAYS, 1, 0xff);
								glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

								//glDepthFunc(GL_LESS);
								lightingShader.use();
								model = glm::mat4(1.0f);
								model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.2f));
								model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
								lightingShader.setMat4("model", model);
								ourModel.Draw(lightingShader);
								// also draw the lamp object(s)
								lampShader.use();
								lampShader.setMat4("projection", projection);
								lampShader.setMat4("view", view);

								// we now draw as many light bulbs as we have point lights.
								glBindVertexArray(lightVAO);
								for (unsigned int i = 0; i < 4; i++)
								{
									model = glm::mat4(1.0f);
									model = glm::translate(model, pointLightPositions[i]);
									model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
									lampShader.setMat4("model", model);
									glDrawArrays(GL_TRIANGLES, 0, 36);
								}

								// // don't forget to enable shader before setting uniforms
								// modelShader.use();
								// 
								// // view/projection transformations
								// 
								// modelShader.setMat4("projection", projection);
								// modelShader.setMat4("view", view);
								// 
								// 
								// modelShader.setMat4("model", model);

								glActiveTexture(GL_TEXTURE2);
								glBindTexture(GL_TEXTURE_2D, transparentTexture);
								// vegetation
								transShader.use();
								transShader.setMat4("projection", projection);
								transShader.setMat4("view", mirrorView);
								glBindVertexArray(transparentVAO);

								// std::map<float, glm::vec3> sorted;
								// for (unsigned int i = 0; i < vegetation.size(); i++)
								// {
								// 	float distance = glm::length(camera.Position - vegetation[i]);
								// 	sorted[distance] = vegetation[i];
								// }
								// for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
								// {
								// 	model = glm::mat4();
								// 	model = glm::translate(model, it->second);
								// 	transShader.setMat4("model", model);
								// 	glDrawArrays(GL_TRIANGLES, 0, 6);
								// }
								glDepthMask(false);
								for (GLuint i = 0; i < vegetation.size(); i++)
								{
									model = glm::mat4(1.0f);
									model = glm::translate(model, vegetation[i]);
									transShader.setMat4("model", model);
									glDrawArrays(GL_TRIANGLES, 0, 6);
								}
								glDepthMask(true);

								// again, a lamp object is weird when we only have a spot light, don't render the light object
								// lampShader.use();
								// lampShader.setMat4("projection", projection);
								// lampShader.setMat4("view", view);
								// model = glm::mat4(1.0f);
								// model = glm::translate(model, lightPos);
								// model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
								// lampShader.setMat4("model", model);

								// glBindVertexArray(lightVAO);
								// glDrawArrays(GL_TRIANGLES, 0, 36);
















		screenShader.use();

		screenShader.use();
		screenShader.setMat4("projection", projection);
		screenShader.setMat4("view", view);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &transparentVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteFramebuffers(1, &quadVBO);
	glDeleteBuffers(1, &transparentVBO);
	glDeleteBuffers(1, &VBO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}