#pragma once

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <SOIL2.h>

class Shader
{
private:
	GLuint id;
	
	//Private function
	std::string loadShaderSource(const char* fileName) 
	{
		char infoLog[512];
		GLint success;

		std::string temp = "";
		std::string src = "";

		std::ifstream in_file;

		//Vertex
		in_file.open(fileName);

		if (in_file.is_open()) {
			while (std::getline(in_file, temp)) {
				src += temp + "\n";
			}
		}
		else {
			std::cout << "ERROR::SHAERS::COULD_NOT_OPEN_FILE: " << fileName << "\n";
		}
		in_file.close();

		return src;
	}

	GLuint loadShader(GLenum type, const char* fileName)
	{
		char infoLog[512];
		GLint success;

		GLuint Shader = glCreateShader(type);
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str();
		glShaderSource(Shader, 1, &src, NULL);
		glCompileShader(Shader);

		glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(Shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADERS::COULD_NOT_COMPILE_SHADER_FILE: "<< fileName << "\n";
			std::cout << infoLog << "\n";
		}
		return Shader;
	}

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) 
	{
		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);
		if (geometryShader != 0) 
			glAttachShader(this->id, geometryShader);
		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}

		//END
		glUseProgram(0);
	}

public: 

	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = "")
	{
		GLuint vertexShader = 0, fragmentShader = 0, geometryShader = 0;
		if (geometryFile != "") {
			geometryShader = this->loadShader(GL_GEOMETRY_SHADER ,geometryFile);
		}
		vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

		this->linkProgram(vertexShader, geometryShader, fragmentShader);

		glDeleteProgram(vertexShader);
		glDeleteProgram(fragmentShader);
		glDeleteProgram(geometryShader);
	}

	~Shader()
	{
		glDeleteProgram(this->id);
	}
	void use() 
	{
		glUseProgram(this->id);
	}

	void unuse() 
	{
		glUseProgram(0);
	}

	void set1i(GLint value, const GLchar* name)
	{
		this->use();
		glUniform1i(glGetUniformLocation(this->id, name), value);
		this->unuse();
	}

	void set1f(GLfloat value, const GLchar* name)
	{
		this->use();
		glUniform1f(glGetUniformLocation(this->id, name), value);
		this->unuse();
	}

	void setVec2f(glm::fvec2 value, const GLchar* name)
	{
		this->use();
		glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec3f(glm::fvec3 value, const GLchar* name) 
	{
		this->use();
		glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec4f(glm::fvec4 value, const GLchar* name)
	{
		this->use();
		glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setMatrix3f(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}

	void setMatrix4f(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}
	//Set uniform functions
};