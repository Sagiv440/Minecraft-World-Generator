#ifndef WORLD_H
#define	WORLD_H

#include "Chunk.h"
#include <vector>
#include <map>
#include <glew.h>
#include <glfw3.h>
#include <noise/noise.h>
#include "noiseutils.h"

#include "Vertex.h"
#include "Mesh.h"

#define SIZE 60
#define HIGHT_SIZE 120

#define WORLD_X_CHUNK 18
#define WORLD_Y_CHUNK 1
#define WORLD_Z_CHUNK 18

#define UP		0
#define DOWN	1
#define FRONT	2
#define BACK	3
#define LEFT	4
#define RIGHT	5

#define WIDTH 512
#define HEIGHT 256



class World
{
private:
	//std::vector<Chunk*> world;
	std::map<int32_t, Chunk*> world;
	std::vector<Mesh*> meshes;
	int16_t Lx, Ly, Lz;
	int16_t Ux, Uy, Uz;

	utils::NoiseMap heightMap;
	utils::NoiseMap heightMap2;
	utils::NoiseMap biomMap;

	glm::i16vec3 PlayerPos;
	glm::vec3 offset;

	float world_ratio;

	bool face_is_clear(Chunk &chunk, int angle, Chunk** near_chank, int X, int Y, int Z);

	void WorldCounter()
	{	
		Lx = Ux;
		Ly = Uy;
		Lz = Uz;

		if (Ux < WORLD_X_CHUNK - 1) {
			Ux++;
		}
		else {
			Ux = 0;
			if (Uz < WORLD_Z_CHUNK - 1) {
				Uz++;
			}
			else { Uz = 0; }
		}
	}

public:
	World();
	~World();

//Functions

	void GetPlayrPos(glm::vec3 pos);

	void updateWorld();

	void renderWorld(Shader &shaeder, Material &material, Texture &diff, Texture &Spec);

	glm::vec2 texCord(uint8_t type, uint8_t angle, int index);

	void QuadGenerate(uint8_t type, uint8_t angle, int16_t x, int16_t y, int16_t z, std::vector<Vertex> &verticie, std::vector<GLuint> &indicise);
	
	Mesh* Genurate_Chunk(Chunk &chunk);

	utils::NoiseMap Generate_tarraing(utils::NoiseMap &heightMap, glm::vec3 positon, int seed, float Retio);

};
#endif

