#pragma once



#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"


class Model
{
private:

	static GLint TextureID;

	Mesh* mesh = nullptr;
	Material * material = nullptr;
	Texture* diffuse = nullptr;
	Texture* Spculare = nullptr;

	

	void updateMeshPos()
	{
		mesh->setPosition(position);
		mesh->setRotation(rotation);
		mesh->setScale(scale);
	}
	

public:

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Model(Mesh* mesh , Texture* difTextureName ,  Texture* SpecTextureName, Material* material, 
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this-> rotation = rotation;
		this->scale = scale;
		this->mesh = mesh;
		this->diffuse = difTextureName;
		this->Spculare = SpecTextureName;
		this->material = material;
	}

	~Model()
	{
	}

	void render(Shader* shaders)
	{
		updateMeshPos();
		this->material->sendToShader(*shaders);		// Dend Matiral data to Shader
		shaders->use();								// Set Shader to be the active shader
		this->diffuse->bind();						// Bind Matirals texter
		this->Spculare->bind();						// Bind Matirals texter
		this->mesh->render(shaders);				//render GameObject mesh
	}

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
};