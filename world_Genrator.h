#pragma once
#include "Chunk.h"
#include <iostream>

#define SIZE 40

#define UP		0
#define DOWN	1
#define FRONT	2
#define BACK	3
#define LEFT	4
#define RIGHT	5

#define WIDTH 512
#define HEIGHT 256

glm::vec2 texCord(uint8_t type, uint8_t angle, int index)
{
	float px = (1 / (float)WIDTH ) * 16.f;
	float py = (1 / (float)HEIGHT ) * 16.f;
	static glm::vec2 txcord[4] = { glm::vec2(0.f,0.f), glm::vec2(px,0.f) ,glm::vec2(px,py) , glm::vec2(0.f,py) };
	glm::vec2 current;
	switch (type)
	{
	case 0:
		break;
	case 1:
		current = txcord[index];
		current += glm::vec2(0.f, 15 * py);
		return current;
		break;
	case 2:
		if (angle == UP) {
			current = txcord[index];
			current += glm::vec2(3 * px, 15 * py);
			return current;
		}
		if (angle == DOWN) {
			current = txcord[index];
			current += glm::vec2(0.f, 15 * py);
			return current;
		}
		current = txcord[index];
		current += glm::vec2(11 * px, 14 * py);
		return current;
		break;
	case 3:
		break;
	}
}

void QuadGenerate(uint8_t type, uint8_t angle, uint16_t x, uint16_t y, uint16_t z, std::vector<Vertex> &verticie, std::vector<GLuint> &indicise)
{
	int i = verticie.size();
	switch (angle) {
	case 0: // UP
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f		,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, 1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f		,(float)z		),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, 1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f		,(float)z		),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, 1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x	+ 1.f	,(float)y + 1.f		,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, 1.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 1);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 3);
		indicise.push_back(i + 0);
		break;
	case 1: // Down
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z		),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, -1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, -1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, -1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x	+ 1.f	,(float)y		,(float)z		),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, -1.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 1);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 3);
		indicise.push_back(i + 0);
		break;
	case 2: // Front
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f		,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, 0.f, 1.f) });
		verticie.push_back({ glm::vec3((float)x	+ 1.f	,(float)y			,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, 0.f, 1.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y			,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, 0.f, 1.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f		,(float)z + 1.f	),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, 0.f, 1.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 1);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 3);
		indicise.push_back(i + 0);
		break;
	case 3: // Back
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f	,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, 0.f, -1.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, 0.f, -1.f) });
		verticie.push_back({ glm::vec3((float)x	+ 1.f	,(float)y		,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, 0.f, -1.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f	,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, 0.f, -1.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 1);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 3);
		indicise.push_back(i + 0);
		break;
	case 4: // Left
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f	,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(-1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(-1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z + 1.f		),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(-1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f	,(float)z + 1.f		),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(-1.f, 0.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 1);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 3);
		indicise.push_back(i + 0);
		break;
	case 5: // Right
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f	,(float)z + 1.f		),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z + 1.f		),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f	,(float)z			),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(1.f, 0.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 1);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 3);
		indicise.push_back(i + 0);
		break;
	}
}

Mesh* Genurate_Chunk(Chunk &chunk)
{
	std::vector<Vertex> verticies;
	std::vector<GLuint> indicies;
	
	uint8_t type;

	for (uint16_t Y = 0; Y < SIZE; Y++) {
		for (uint16_t Z = 0; Z < SIZE; Z++) {
			for (uint16_t X = 0; X < SIZE; X++) {
				type = chunk.getCube(X, Y, Z);

				if (Y == SIZE - 1) //UP
					QuadGenerate(type, UP, X, Y, Z, verticies, indicies);
				else if (chunk.getCube(X, Y + 1, Z) == 0) {
					QuadGenerate(type, UP, X, Y, Z, verticies, indicies);
				}
				if (Y == 0) //DOWN
					QuadGenerate(type, DOWN, X, Y, Z, verticies, indicies);
				else if (chunk.getCube(X, Y - 1, Z) == 0) {
					QuadGenerate(type, DOWN, X, Y, Z, verticies, indicies);
				}
				if (Z == SIZE - 1) //FRONT
					QuadGenerate(type, FRONT, X, Y, Z, verticies, indicies);
				else if (chunk.getCube(X, Y, Z + 1) == 0) {
					QuadGenerate(type, FRONT, X, Y, Z, verticies, indicies);
				}
				if (Z == 0) //BACK
					QuadGenerate(type, BACK, X, Y, Z, verticies, indicies);
				else if (chunk.getCube(X, Y, Z - 1) == 0) {
					QuadGenerate(type, BACK, X, Y, Z, verticies, indicies);
				}
				if (X == SIZE - 1) //LEFT
					QuadGenerate(type, LEFT, X, Y, Z, verticies, indicies);
				else if (chunk.getCube(X + 1, Y, Z) == 0) {
					QuadGenerate(type, LEFT, X, Y, Z, verticies, indicies);
				}
				if (X == 0) //RIGHT
					QuadGenerate(type, RIGHT, X, Y, Z, verticies, indicies);
				else if (chunk.getCube(X - 1, Y, Z) == 0) {
					QuadGenerate(type, RIGHT, X, Y, Z, verticies, indicies);
				}
			}
		}
	}
	Mesh* mesh = new Mesh(verticies.data(), verticies.size(), indicies.data(), indicies.size());
	return mesh;
}