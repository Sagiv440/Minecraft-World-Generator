#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#define SIZE 60
#define HIGHT_SIZE 120

#define UP		0
#define DOWN	1
#define FRONT	2
#define BACK	3
#define LEFT	4
#define RIGHT	5

#include"Mesh.h"
#include "Array2D.h"
#include "noiseutils.h"
#include <map>
class Chunk
{
private:
	glm::vec3 pos;
	int Mesh_index;
	Mesh* mesh;
	uint8_t* chunk;
	bool scand;
	Chunk* near_chunk[6] = { NULL,NULL,NULL,NULL,NULL,NULL };

	uint32_t Lehmer32(uint32_t nLehmer);

	int rndInt(int value, int min, int max) {
		return (value % (max - min) + min);
	}

public:
	bool scane(std::map<int32,Chunk*> &world) {

		scand = true;
		int32_t key = 0;
		key = ((int16_t)pos.z) << 16 | (1 + (int16_t)pos.x);
		if (world.find(key) != world.end()) {
			near_chunk[LEFT] = world[key];
			scand = false;
		}

		key = ((int16_t)pos.z) << 16 | (-1 + (int16_t)pos.x);
		if (world.find(key) != world.end()) {
			near_chunk[RIGHT] = world[key];
			scand = false;
		}

		key = (1 + (int16_t)pos.z) << 16 | ((int16_t)pos.x);
		if (world.find(key) != world.end()) {
			near_chunk[FRONT] = world[key];
			scand = false;
		}

		key = (-1 + (int16_t)pos.z) << 16 | ((int16_t)pos.x);
		if (world.find(key) != world.end()) {
			near_chunk[BACK] = world[key];
			scand = false;
		}
		return scand;
	}

	void set_Near(int x, int y, int z, Chunk* chunk)
	{
		if( y < 0)
			near_chunk[DOWN] = chunk;
		else if (y > 0)
			near_chunk[UP] = chunk;

		if (z < 0)
			near_chunk[BACK] = chunk;
		else if (z > 0)
			near_chunk[FRONT] = chunk;

		if (z < 0)
			near_chunk[LEFT] = chunk;
		else if (z > 0)
			near_chunk[RIGHT] = chunk;
	}

	bool Get_scand() {
		return scand;
	}

	Chunk**Get_near_list() {
		return this->near_chunk;
	}

	Chunk(glm::vec3 pos);

	~Chunk();

	glm::vec3 getPosition();

	void Set_mesh(Mesh* mesh);

	Mesh* Get_mesh();

	void SetCubes(uint8_t type, int x, int y, int z);

	uint8_t getCube(int x, int y, int z);

	void fill(uint8_t type);

	void assingByMoiseMap(utils::NoiseMap heightMap, utils::NoiseMap heightMap2, utils::NoiseMap buyomMap);

	void setLayer(uint8_t type, int y);

	void set_index(int i);

	void add_tree(uint8_t type, int x, int y, int z);

};
#endif

