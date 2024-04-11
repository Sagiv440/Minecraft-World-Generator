#pragma once

#include<glew.h>
#include<glfw3.h>

#include<vector>
#include "Vertex.h"

class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indicies;

public:
	Primitive() {};

	virtual ~Primitive() {};

	//Functions
	void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndicies)
	{
		for (size_t i = 0; i < nrOfVertices; i++) 
		{
			this->vertices.push_back(vertices[i]);
		}
		for (size_t i = 0; i < nrOfIndicies; i++)
		{
			this->indicies.push_back(indicies[i]);
		}
	}

	inline Vertex* getVertices() { return this->vertices.data(); };
	inline GLuint* getIndicies() { return this->indicies.data(); };
	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndicies() { return this->indicies.size(); }


};

class Quad : public Primitive
{
private:
	Quad() : Primitive()
	{
			Vertex vertices[] = 
		{
				//Position						//Color						//Texcoord
			glm::vec3(-0.5f, 0.5f, 0.0f),	glm::vec3(1.f, 0.0f, 0.f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.f, 0.0f, -1.f),
			glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.f, 1.0f, 0.f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.f, 0.0f, -1.f),
			glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(0.f, 0.0f, 1.f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.f, 0.0f, -1.f),
			glm::vec3(0.5f, 0.5f, 0.0f),	glm::vec3(1.f, 1.0f, 0.f),	glm::vec2(1.0f, 1.0f),	glm::vec3(0.f, 0.0f, -1.f)
		};

		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0,1,2, 0, 2, 3
		};

		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}

public:

};
