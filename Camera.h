#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\matrix_transform.hpp>
#include "Shader.h"

class Camera
{
private:
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ModelMatrix;

	float fov;
	float nearPlane;
	float farPlane;
	float aspectRatio;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	void updateProjection(float aspectRatio, float fov, float nearPlane, float farPlane)
	{
		this->ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}

public:
	Camera( glm::vec3 position = glm::vec3(0.f) , glm::vec3 rotation = glm::vec3(0.f), float fov = 90.f, float nearPlane = 0.1f, float farPlane = 1000.f, float aspectRatio = 1.f, glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f))
	{
		this->camPosition = position;
		this->worldUp = worldUp;
		this->fov = fov;
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
		this->ViewMatrix = glm::mat4(1.f);
		this->ProjectionMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::mat4(1.f);

		setRotation(rotation);

		this->ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}
	//Acessors

	//Modifiers
	void setRotation(glm::vec3 rotation)
	{
		this->ModelMatrix = glm::mat4(1.f);
		Rotate(rotation);
	}

	void setPosition(glm::vec3 position)
	{
		this->camPosition = position;
		this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	}

	glm::vec3 getPosition()
	{
		return this->camPosition;
	}

	void Rotate(glm::vec3 rotation)
	{
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

		this->camFront = glm::vec3(0.f, 0.f, -1.f);
		this->camFront = (glm::vec4(this->camFront, 1.f) * ModelMatrix);

		this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	}

	void LRotate(glm::vec3 rotation)
	{

		rotation = glm::vec4(rotation, 1.f) * this->ModelMatrix;

		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

		this->camFront = glm::vec3(0.f, 0.f, -1.f);
		this->camFront = (glm::vec4(this->camFront, 1.f) * ModelMatrix);

		this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	}
	
	void move(glm::vec3 move)
	{
		this->camPosition += move;
		this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	}

	void Lmove(glm::vec3 move)
	{
		move = glm::vec4(move, 1.f) * this->ModelMatrix;

		this->camPosition += move;
		this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	}

	void setAspectRatio(float ar)
	{
		this->aspectRatio = ar;
		updateProjection(this->aspectRatio, this->fov, this->nearPlane, this->farPlane);
	}

	void setFOV(float fov)
	{
		this->fov = fov;
		updateProjection(this->aspectRatio, this->fov, this->nearPlane, this->farPlane);
	}

	void setNearPlane(float near)
	{
		this->nearPlane = near;
		updateProjection(this->aspectRatio, this->fov, this->nearPlane, this->farPlane);
	}

	void setFarPlane(float far)
	{
		this->farPlane = far;
		updateProjection(this->aspectRatio, this->fov, this->nearPlane, this->farPlane);
	}

	//Functions
	void updateUniforms(Shader &shader)
	{
		shader.setVec3f(this->camPosition, "cameraPos");
		shader.setMatrix4f(this->ViewMatrix, "ViewMatrix");
		shader.setMatrix4f(this->ProjectionMatrix, "ProjectionMatrix");
	}

};