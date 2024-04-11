#include "Game.h"



//Private functions
void Game::initGLFW()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILD" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(const char* titale, bool resizeable)
{

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GLVerMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GLVerMinor);
	glfwWindowHint(GLFW_RESIZABLE, resizeable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, titale, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILD" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callbalck);

	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); //IMPORTANT
	
}

void Game::initGLEW()
{
	if (glewInit() != GLEW_OK) {
		std::cout << "Error:: main.cpp::glew_init_failde\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwGetCursorPos(window, &xpos, &ypos);
	old_xpos = xpos;
	old_ypos = ypos;
}


void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->framebufferWidth) / this->framebufferHeight, this->nearPlane, this->farPlane);

}

void Game::initCamera()
{
	this->camera = Camera(glm::vec3(0.f, 2.f, 7.f));
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader("vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTexturs()
{
	this->textures.push_back(new Texture("Images/Mincraft_Block_texture.png", GL_TEXTURE_2D, TextureUnitIndex));
	TextureUnitIndex++;
	this->textures.push_back(new Texture("Images/awp_india_texture_4.png" ,GL_TEXTURE_2D, TextureUnitIndex));
	TextureUnitIndex++;
	//this->textures.push_back(new Texture("Images/SpaceMarin.png", GL_TEXTURE_2D, TextureUnitIndex));
	//TextureUnitIndex++;
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(0.f), this->textures[0]->getTextureUnit(), this->textures[1]->getTextureUnit()));
	//this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), this->textures[2]->getTextureUnit(), this->textures[1]->getTextureUnit()));

}

void Game::initLight()
{
	this->lights.push_back(new glm::vec3(-10.f, 7.f, 7.f));
}

void Game::initUniforms()
{
	/*this->shaders[0]->setMatrix4f(this->ViewMatrix, "ViewMatrix");
	this->shaders[0]->setMatrix4f(this->ProjectionMatrix, "ProjectionMatrix");
	this->shaders[0]->setVec3f(this->camPosition, "cameraPos");*/
	camera.updateUniforms(*shaders[0]);
	this->shaders[0]->setVec3f(*this->lights[0], "lightPos0");
}

void Game::initMesh()
{
	
	
	//LoadFromObjectFile("SpaceMarin.obj");
}

void Game::initModel()
{
	//this->Models.push_back(new Model(meshes[0], textures[0], textures[1], materials[0], glm::vec3(-20.f, -42.f, -20.f)));
	//this->Models.push_back(new Model(meshes[1], textures[2], textures[1], materials[1], glm::vec3(4.f, -4.f, 0.f)));
}

void Game::world_Generator()
{

}
//Static variables


//Constructors / Distractors
Game::Game(const char* titale, 
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	int GLVerMajor, int GLVerMinor,

	bool resizeable) 
: 
	WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT),
	GLVerMajor(GLVerMajor), GLVerMinor(GLVerMinor)
{	
	//Init variables
	this->window = nullptr;
	this->framebufferWidth = WINDOW_WIDTH;
	this->framebufferHeight = WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 7.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.0f;

	this->initGLFW();
	this->initWindow(titale, resizeable);
	this->initGLEW();
	this->initOpenGLOptions();
	//this->initMatrices();
	this->initCamera();
	this->initShaders();
	this->initTexturs();
	this->initMaterials();
	this->initMesh();
	this->initModel();
	this->initLight();
	this->initUniforms();
}
Game::~Game()
{
	this->shaders[0]->unuse();
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < shaders.size(); i++)
	{
		delete this->shaders[i];
	}
	for (size_t i = 0; i < textures.size(); i++)
	{
		delete this->textures[i];
	}
	for (size_t i = 0; i < this->meshes.size(); i++)
	{
		delete this->meshes[i];
	}
	for (size_t i = 0; i < this->Models.size(); i++)
	{
		delete this->Models[i];
	}
	for (size_t i = 0; i < this->lights.size(); i++)
	{
		delete this->lights[i];
	}
}
//Accessors
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}
//Modifiers
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}
//Functions

GLint Game::TextureUnitIndex{ 0 };

int Game::loadTexture(const char* fileName, GLenum type)
{
	Texture* text = new Texture(fileName, type, TextureUnitIndex);
	TextureUnitIndex++;
	this->textures.push_back(text);
	return textures.size() - 1;
}



void Game::update()
{
	//UPDATE INPUT ---
	glfwPollEvents();

	inputUpdate();
	/*glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwGetCursorPos(window, &xpos, &ypos);*/
	
	world.GetPlayrPos(camera.getPosition());
	world.updateWorld();

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	//this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->framebufferWidth) / this->framebufferHeight, this->nearPlane, this->farPlane);
	//this->shaders[0]->setMatrix4f(ProjectionMatrix, "ProjectionMatrix");
	camera.setAspectRatio(static_cast<float>(this->framebufferWidth) / this->framebufferHeight);
	camera.updateUniforms(*shaders[0]);

}

void Game::render()
{
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//this->materials[0]->sendToShader(*this->shaders[0]);	// Dend Matiral data to Shader
	//this->shaders[0]->use();								// Set Shader to be the active shader
	//this->texturs[0]->bind();								// Bind Matirals texter
	//this->texturs[1]->bind();								// Bind Matirals texter
	//this->meshes[0]->render(this->shaders[0]);				//render GameObject mesh

	world.renderWorld(*this->shaders[0], *materials[0], *textures[0], *textures[0]);

	for (size_t i = 0; i < Models.size(); i++)
	{
		Models[i]->render(this->shaders[0]);
	}

	glfwSwapBuffers(this->window);

	glFlush();
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::inputUpdate()
{
	glfwGetCursorPos(window, &xpos, &ypos);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		std::cout << "BUTTON_PRESSET" << "\n";
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GL_TRUE)
	{
		camera.Lmove(glm::vec3(0.f, 0.f, -1.f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GL_TRUE)
	{
		camera.Lmove(glm::vec3(0.f, 0.f, 1.f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GL_TRUE)
	{
		camera.Lmove(glm::vec3(-1.f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GL_TRUE)
	{
		camera.Lmove(glm::vec3(1.f, 0.f, 0.f));
	}

	camera.Rotate(glm::vec3(0.f,float(xpos - old_xpos)*0.1f, 0.f));
	camera.LRotate(glm::vec3(float(ypos - old_ypos)*0.1f, 0.f, 0.f));

	old_xpos = xpos;
	old_ypos = ypos;
}

//static functions
void Game::framebuffer_resize_callbalck(GLFWwindow* window, int fbW, int fbH) 
{
	glViewport(0, 0, fbW, fbH);
}