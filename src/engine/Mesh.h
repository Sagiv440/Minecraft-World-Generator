#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

class Mesh
{
private:
	unsigned nrOfVertices;
	unsigned nrOfIndicies;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO(Vertex* VertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndicies)
	{
		//Set variables
		this->nrOfVertices = nrOfVertices;
		this->nrOfIndicies = nrOfIndicies;


		//VAO, VBO, EBO
	//GEN VAO AND BIND
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VERTIX BUFFER
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), VertexArray, GL_STATIC_DRAW);

		//GEN EBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndicies* sizeof(GLuint), indexArray, GL_STATIC_DRAW);

		//SET VERTEX ATTRIB POINTERS NAD ENABEL (INPUT ASSEBLY)
		//GLuint attribLoc = glGetAttribLocation(core_program, "vertex_position");
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		//attribLoc = glGetAttribLocation(core_program, "vertex_color");
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		//attribLoc = glGetAttribLocation(core_program, "vertex_texcoord");
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		//attribLoc = glGetAttribLocation(core_program, "vertex_texcoord");
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIDE VAO 0
		glBindVertexArray(0);
	}

	void updateUniforms(Shader* shader)
	{
		shader->setMatrix4f(this->ModelMatrix, "ModelMatrix");
	}

	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

public:
	Mesh(Vertex* VertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndicies,
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f) )
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(VertexArray, nrOfVertices, indexArray, nrOfIndicies);
		this->updateModelMatrix();
	}
	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);

	}
	//Acessors

	//Modifiers
	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}

	void setRotation(const glm::vec3 rotation)
	{
		this->rotation = rotation;
	}

	void setScale(const glm::vec3 scale)
	{
		this->scale = scale;
	}

	//Functions

	void move(const glm::vec3 position)
	{
		this->position += position;
	}

	void rotate(const glm::vec3 rotation)
	{
		this->rotation += rotation;
	}

	void scale_UP(const glm::vec3 scale)
	{
		this->scale += scale;
	}

	void update() 
	{

	}

	void render(Shader* shader) {
		//
		this->updateModelMatrix();
		this->updateUniforms(shader);
	
		shader->use();
		//Bind vertex array object
		glBindVertexArray(this->VAO);

		//RENDER
		glDrawElements(GL_TRIANGLES, this->nrOfIndicies, GL_UNSIGNED_INT, 0);
	}
};