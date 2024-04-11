#pragma once
#include "Shader.h"

class Material 
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint specularTex;

public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material() {}

	//Function
	void sendToShader(Shader & core_program)
	{
		core_program.setVec3f(this->ambient, "material.ambient");
		core_program.setVec3f(this->diffuse, "material.diffuse");
		core_program.setVec3f(this->specular, "material.specular");
		core_program.set1i(this->diffuseTex, "material.diffuseTex");
		core_program.set1i(this->specularTex, "material.specularTex");
	}
};