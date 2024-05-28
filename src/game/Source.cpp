 #include "libs.h"



GLuint loadShaders(const char* vertex_shader, const char* fragment_shader) {
	GLuint program;
	GLint success;
	GLuint status = 1;
	char infoLog[512];
	std::string tmp = "";
	std::string src = "";

	std::ifstream in_file;

	in_file.open(vertex_shader);

	if (in_file.is_open()) {
		while (std::getline(in_file, tmp)) {
			src += tmp + "\n";
		}
	}
	else {
		std::cout << "ERROR::LOAD_SADERS:: COULD_NOT_LOAD_VERTEX_SHADER" << "\n";	
		status = 0;
	}
	in_file.close();
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOAD_SHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
		std::cout << infoLog << "\n";
		status = 0;
	}
	tmp = "";
	src = "";

	in_file.open(fragment_shader);
	if (in_file.is_open()) {
		while (std::getline(in_file, tmp)) {
			src += tmp + "\n";
		}
	}
	else {
		std::cout << "ERROR::LOAD_SHADERS::COULD_NOT_LOAD_FRAGMENT_SHADER" << "\n";
		status = 0;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOAD_SHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
		std::cout << src << "\n";
		status = 0;
	}
	
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::LOAD_SHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
		std::cout << infoLog << "\n";
		status = 0;
	}

	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (status == 0) {
		std::cout << "ERROR::LOAD_SHADERS::FAILD_TO_LOAD_SHADERS" << "\n";
		return 0;
	}else
		return program;
}


int main() {

	Vertex vertices[] =
	{
		//Position						//Color						//Texcoord
	glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.f, 0.0f, 0.f),	glm::vec2(0.0f, 1.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.f, 1.0f, 0.f),	glm::vec2(0.0f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(0.f, 0.0f, 1.f),	glm::vec2(1.0f, 0.0f)
	};

	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] =
	{
		0,1,2
	};

	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	glfwInit();

	const int	WINDOW_WIDTH = 640;
	const int	WINDOW_HEIGHT = 480;
	int		framebufferWidht = 0;
	int		framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Testing", NULL, NULL);

	glfwGetFramebufferSize(window, &framebufferWidht, &framebufferHeight);
	glViewport(0, 0, framebufferWidht, framebufferHeight);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	glfwMakeContextCurrent(window); //Initialize GLEW
	glewExperimental = true; //Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background


	if (glewInit() != GLEW_OK) {
		std::cout << "Error::main.cpp::glew_init_failde\n";
		glfwTerminate();
	}

	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLuint core_program = loadShaders("vertex_core.glsl", "fragment_core.glsl");
	if (core_program == 0) {
		glfwTerminate();
	}

	//VAO, VBO, EAO
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//GEN VERTIX BUFFER
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//GEN EBO AND BIDN AND SEND DATA
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//SET VERTEX ATTRIB POINTERS AND ENABEL (INPUT ASSEMBLY)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	//BIDE VAO 0
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {

		//UPDATE INPUT
		glfwPollEvents(); 
		//UPDATE

		//DRAW

		//Clear_st
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use a Program
		glUseProgram(core_program);

		//Bind vertex array object
		glBindVertexArray(VAO);

		//DRAW
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		//END_DRAW
		glfwSwapBuffers(window);
		glFlush();

	}

	//Delete program
	glDeleteProgram(core_program);
	glfwTerminate();
	return 0;

}


