#include "Chunk.h"
#include <algorithm>


#define AIR 0
#define STONE 1
#define GRASS 2
#define SAND 3
#define WOOD 4
#define SNOW_GRASS 5
#define DIRT 6
#define LEAF 7



Chunk::Chunk(glm::vec3 pos) : pos(pos), mesh(NULL), scand(false)
{
	this->chunk = (uint8_t*)malloc(sizeof(uint8_t) * HIGHT_SIZE * SIZE * SIZE);
	this->fill(0);
}

Chunk::~Chunk()
{
	delete this->chunk;
}

uint32_t Chunk::Lehmer32(uint32_t nLehmer)
{
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

void Chunk::Set_mesh(Mesh* mesh) {
	if (this->mesh != NULL)
		delete this->mesh;
	this->mesh = mesh;
}

Mesh* Chunk::Get_mesh()
{
	return this->mesh;
}

glm::vec3 Chunk::getPosition() 
{
	return pos;
}

void Chunk::SetCubes(uint8_t type, int x, int y, int z)
{
	int X = std::min(std::max(0, x), SIZE - 1);
	int Y = std::min(std::max(0, y), HIGHT_SIZE - 1);
	int Z = std::min(std::max(0, z), SIZE - 1);
	chunk[Y *SIZE *SIZE + Z * SIZE + X] = type;
}

uint8_t Chunk::getCube(int x, int y, int z)
{
	int X = std::min(std::max(0, x), SIZE - 1);
	int Y = std::min(std::max(0, y), HIGHT_SIZE - 1);
	int Z = std::min(std::max(0, z), SIZE - 1);
	return chunk[Y *SIZE *SIZE + Z * SIZE + X];
}

void Chunk::setLayer(uint8_t type, int y)
{
	for (uint32_t i = 0; i < SIZE * SIZE ; i++)
	{
		chunk[y * SIZE * SIZE + i] = type;
	}
}

void Chunk::set_index(int i)
{
	Mesh_index = i;
}

void Chunk::fill(uint8_t type)
{
	for (uint32_t i = 0; i < HIGHT_SIZE * SIZE * SIZE; i++)
	{
		chunk[i] = type;
	}
}

void Chunk::add_tree(uint8_t type, int x, int y, int z) {
	int value3 = 0;
	for (int i = 0; i < 5; i++) {
		this->SetCubes(WOOD, x, y + i, z);
		for (int X = -2; X < 3; X++) {
			for (int Z = -2; Z < 3; Z++) {
				value3 = Lehmer32((z + Z)<< 24 | (x + X) << 8 | (y + i));
				value3 = rndInt(value3, 0, 30);
				if (x + X < SIZE && x + X > -1 && z + Z < SIZE && z + Z > -1) {
					if(value3 > 1)
						this->SetCubes(LEAF, x + X, y + 4 +i, z + Z);
				}
			}
		}
	}

}

void Chunk::assingByMoiseMap(utils::NoiseMap heightMap, utils::NoiseMap heightMap2, utils::NoiseMap buyomMap)
{
	uint16_t size_x = heightMap.GetWidth(); uint16_t size_z = heightMap.GetHeight();
	int value = 0;
	float value2 = 0;
	int value3 = 0;
	for(int16_t z = 0; z < size_z; z++)
		for (int16_t x = 0; x < size_x; x++) {
			
			//value = rndInt(value, 0, SIZE - 1);
			
			value2 = buyomMap.GetValue(x, z) ;
			value3 = Lehmer32((z + (uint16_t)pos.z) << 16 | (x + (uint16_t)pos.x));
			
			if (value2 > 0.4f) {
				value = SIZE * (heightMap2.GetValue(x, z) + 1.f) * 0.5f;
				value = std::min(std::max(0, value), HIGHT_SIZE - 1);
				this->SetCubes(5, x, value, z);
			}
			else if(value2 < -0.3f){
				value = SIZE * (heightMap2.GetValue(x, z) + 1.f) * 0.5f;
				value = std::min(std::max(0, value), HIGHT_SIZE - 1);
				this->SetCubes(3, x, value, z);
			}
			else {
				value = SIZE * (heightMap.GetValue(x, z) + 1.f) * 0.5f;
				value = std::min(std::max(0, value), HIGHT_SIZE - 1);
				this->SetCubes(2, x, value, z);

				value3 = rndInt(value3, 0, value + 10);
				if(value3 == 1)
					add_tree(4, x, value + 1, z);
			}
			
			for (int y = value -1; y >= 0; y--) {
				if(y > value - 3)
					this->SetCubes(6, x, y, z);
				else
					this->SetCubes(1, x, y, z);
			}
		}
}


