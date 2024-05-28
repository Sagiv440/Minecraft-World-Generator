#pragma once
#include "libs.h"






class Game
{
private:
//Variables
	GLFWwindow* window;
	static GLint TextureUnitIndex;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	double xpos, ypos, old_xpos, old_ypos;

	//OpenGL Contex
	const int GLVerMajor;
	const int GLVerMinor;

	//Camera
	Camera camera;

	//World
	World world;

	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ProjectionMatrix;

	float fov;
	float nearPlane;
	float farPlane;

	//Shaders
	std::vector<Shader*> shaders;

	//Textures
	std::vector<Texture*> textures;

	//Meshes
	std::vector<Mesh*> meshes;

	//Matirials
	std::vector<Material*> materials;

	//Lights
	std::vector<glm::vec3*> lights;

	//Models
	std::vector<Model*> Models;

//Private functions

	void initGLFW();
	void initWindow(const char* titale, bool resizeable);
	void initGLEW(); //AFTER CONTEXT CREATION!!!
	void initOpenGLOptions();
	void initMatrices();
	
	void initShaders();
	void initTexturs();
	void initMaterials();
	void initCamera();
	void initMesh();
	void initLight();
	void initUniforms();
	void initModel();

	float Vector_DotProduct(glm::vec3 &v1, glm::vec3 &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	float Vector_Length(glm::vec3 &v)
	{
		return sqrtf(Vector_DotProduct(v, v));
	}

	glm::vec3 Vector_Normalise(glm::vec3 &v)
	{
		float l = Vector_Length(v);
		//return { v.x / l, v.y / l, v.z / l };
		return glm::vec3(v.x / l, v.y / l, v.z / l);
	}

	glm::vec3 Vector_CrossProduct(glm::vec3 &v1, glm::vec3 &v2)
	{
		glm::vec3 v(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
		return v;
	}

	bool get_obj_indicies(char* s, float &v1, float &v2, float &v3, float &t1, float &t2, float &t3)
	{
		char junk;
		bool texcoord_ON = false;
		unsigned int i = 0;
		while (s[i] != '\0') {
			if (s[i] == '/') {
				s[i] = ' ';
				texcoord_ON = true;
			}
			i++;
		}
		std::strstream sl;
		sl << s;
		if (texcoord_ON)
			sl >> junk >> v1 >> t1 >> v2 >> t2 >> v3 >> t3;
		else {
			sl >> junk >> v1 >> v2 >> v3;
			t1 = 0; t2 = 0; t3 = 0;
		}
		return texcoord_ON;


	}

//Static variables

public:
//Constructors / Distractors
	Game(const char* titale,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		int GLVerMajor, int GLVerMinor,
		bool resizeable);

	virtual ~Game();
//Accessors
	int getWindowShouldClose();

//Modifiers
	void setWindowShouldClose();

//Functions
	void update();

	void render();

	void inputUpdate();

	int loadTexture(const char* fileName, GLenum type);

	//int loadModel(const char* fileName, int difTexIndex, int SepcTexIndex, glm::vec3 position = glm::vec3(0.f, 0.f, 0.f));

	void world_Generator();

	bool LoadFromObjectFile(std::string sFilename)
	{
		std::fstream in_file;
		in_file.open(sFilename);
		if (!in_file.is_open()) {
			return false;
		}
		unsigned int vertex_size = 0;
		unsigned int index_size = 0;
		unsigned int texcoord_size = 0;
		bool texcoord_ON;
		glm::vec3 line1, line2, normal, a;
		float x, y, z, x1, y1, z1;
		std::string junk;
		std::vector<glm::vec2> texcoord;
		std::vector<Vertex> verticies;
		std::vector<GLuint> indicies;

		while (!in_file.eof())
		{
			char line[128];
			in_file.getline(line, 128);

			std::strstream s;
			s << line;

			if (line[0] == 'v' && line[1] != 't') {
				s >> junk >> x >> y >> z;
				Vertex vertex;
				vertex.position = glm::vec3(x, y, z);
				vertex.color = glm::vec3(1.f, 0.0f, 0.f);
				vertex.texcoord = glm::vec2(0.0f, 0.0f);
				vertex.normal = glm::vec3(0.f);

				verticies.push_back(vertex);
			}
			if (line[0] == 'v' && line[1] == 't') {
				s >> junk >> x >> y;
				texcoord.push_back(glm::vec2(x, y));
			}
			if (line[0] == 'f') {

				texcoord_ON = get_obj_indicies(line, x, y, z, x1, y1, z1);


				indicies.push_back(x - 1);
				indicies.push_back(y - 1);
				indicies.push_back(z - 1);


				line1 = verticies[(int)x - 1].position - verticies[(int)y - 1].position;
				line2 = verticies[(int)x - 1].position - verticies[(int)z - 1].position;
				normal = Vector_CrossProduct(line1, line2);
				normal = Vector_Normalise(normal);

				if (verticies[(int)x - 1].normal == glm::vec3(0.f)) {
					verticies[(int)x - 1].normal = normal;
				}
				else {
					a = normal + verticies[(int)x - 1].normal;
					verticies[(int)x - 1].normal = Vector_Normalise(a);
				}

				if (verticies[(int)y - 1].normal == glm::vec3(0.f)) {
					verticies[(int)y - 1].normal = normal;
				}
				else {
					a = normal + verticies[(int)y - 1].normal;
					verticies[(int)y - 1].normal = Vector_Normalise(a);
				}

				if (verticies[(int)z - 1].normal == glm::vec3(0.f)) {
					verticies[(int)z - 1].normal = normal;
				}
				else {
					a = normal + verticies[(int)z - 1].normal;
					verticies[(int)z - 1].normal = Vector_Normalise(a);
				}
				if (texcoord_ON) {
					verticies[(int)x - 1].texcoord = texcoord[(int)x1 - 1];
					verticies[(int)y - 1].texcoord = texcoord[(int)y1 - 1];
					verticies[(int)z - 1].texcoord = texcoord[(int)z1 - 1];
				}

			}
		}
		in_file.close();

		this->meshes.push_back(new Mesh(verticies.data(), verticies.size(), indicies.data(), indicies.size()));
		return true;
	}

	

//static functions
	static void framebuffer_resize_callbalck(GLFWwindow* window, int fbW, int fbH);

	GLFWwindow* get_window()
	{
		return this->window;
	}

	Model* Get_Model(int index)
	{
		return Models[index];
	}	
};