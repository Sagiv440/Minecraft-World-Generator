#include "World.h"

using namespace noise;

World::World() : PlayerPos(glm::i16vec3(0)), Ux(0), Uy(0), Uz(0), Lx(0), Ly(0), Lz(0)
{
	world_ratio = 1.f / (float)SIZE;

	long x = (float)WORLD_X_CHUNK;
	//long y = pos.y * world_ratio;
	long z = (float)WORLD_Z_CHUNK;
	offset = glm::vec3(x, 0.f, z);

	
}

World::~World()
{
	for (auto item : world)
		delete item.second;

	for (auto &e1 : meshes)
		delete e1;
}

void World::GetPlayrPos(glm::vec3 pos) {
	PlayerPos.x = pos.x * world_ratio;
	//long y = pos.y * world_ratio;
	PlayerPos.z = pos.z * world_ratio;
	
}

void World::updateWorld() {

	int32_t key = 0;
	int32_t Lkey = 0;
	key = (Uz + PlayerPos.z) << 16 | (Ux + PlayerPos.x);
	Lkey = (Lz + PlayerPos.z) << 16 | (Lx + PlayerPos.x);
	if (world.find(key) == world.end()) {
		world[key] = new Chunk(glm::i16vec3(Ux, Uy, Uz) + PlayerPos);
		//world[key]->set_Near(Ux - Lx, Uy - Ly, Uz - Lz, world[Lkey]);
		//world[Lkey]->set_Near(Lx - Ux, Ly - Uy, Lz - Uz, world[key]);
		world[key]->assingByMoiseMap(Generate_tarraing(heightMap,glm::vec3(Ux, Uy, Uz) + (glm::vec3)PlayerPos, 1, 0.4), Generate_tarraing(heightMap2, glm::vec3(Ux, Uy, Uz) + (glm::vec3)PlayerPos, 56, 0.1), Generate_tarraing(biomMap,glm::vec3(Ux, Uy, Uz) + (glm::vec3)PlayerPos, 43, 0.07f));
		//world[key]->Set_mesh(Genurate_Chunk(*world[key]));
	}
	else {
		if (world[key]->scane(world) == false)
			if (world[key]->Get_mesh() == NULL) {
				world[key]->Set_mesh(Genurate_Chunk(*world[key]));
			}
	}

	this->WorldCounter();
}

void World::renderWorld(Shader &shaeder, Material &material, Texture &diff, Texture &Spec)
{
	material.sendToShader(shaeder);		// Dend Matiral data to Shader
	shaeder.use();						// Set Shader to be the active shader
	diff.bind();						// Bind Matirals texter
	Spec.bind();						// Bind Matirals texter
	/*for (auto &e1 : world)
	{
		if(e1.second->Get_mesh() != NULL)
			e1.second->Get_mesh()->render(&shaeder);
	}*/
	int32_t key = 0;
	
	for(int16_t z = 0; z < WORLD_Z_CHUNK; z++)
		for (int16_t x = 0; x < WORLD_X_CHUNK; x++) {
			key = (z + (int16_t)PlayerPos.z) << 16 | (x + (int16_t)PlayerPos.x);
			if (world.find(key) != world.end())
				if (world[key]->Get_mesh() != NULL)
					world[key]->Get_mesh()->render(&shaeder);
		}
	
}

glm::vec2 World::texCord(uint8_t type, uint8_t angle, int index)
{
	float px = (1 / (float)WIDTH) * 16.f;
	float py = (1 / (float)HEIGHT) * 16.f;
	static glm::vec2 txcord[4] = { glm::vec2(0.f,py) ,glm::vec2(0.f,0.f), glm::vec2(px,0.f) ,glm::vec2(px,py) };
	glm::vec2 current;
	switch (type)
	{
	case 0:
		break;
	case 1:// Stown
		current = txcord[index];
		current += glm::vec2(0.f, 15 * py);
		return current;
		break;
	case 2: //Grass
		if (angle == UP) {
			current = txcord[index];
			current += glm::vec2(2 * px, 15 * py);
			return current;
		}
		if (angle == DOWN) {
			current = txcord[index];
			current += glm::vec2(11 * px, 14 * py);
			return current;
		}
		current = txcord[index];
		current += glm::vec2(3 * px, 15 * py);
		return current;
		break;
	case 3: //Sand
		current = txcord[index];
		current += glm::vec2(3 * px, 14 * py);
		return current;
		break;

	case 4: //Wood
		if (angle == UP) {
			current = txcord[index];
			current += glm::vec2(29 * px, 13 * py);
			return current;
		}
		if (angle == DOWN) {
			current = txcord[index];
			current += glm::vec2(29 * px, 13 * py);
			return current;
		}
		current = txcord[index];
		current += glm::vec2(28 * px, 13 * py);
		return current;
		break;
	
	case 5: //Sonw Grass
		if (angle == UP) {
			current = txcord[index];
			current += glm::vec2(5 * px, 10 * py);
			return current;
		}
		if (angle == DOWN) {
			current = txcord[index];
			current += glm::vec2(11 * px, 14 * py);
			return current;
		}
		current = txcord[index];
		current += glm::vec2(1 * px, 15 * py);
		return current;
		break;
	case 6:// Dart
		current = txcord[index];
		current += glm::vec2(11 * px, 14 * py);
		return current;
	case 7:// Leafs
		current = txcord[index];
		current += glm::vec2(25 * px, 11 * py);
		return current;
	}
	
}

void World::QuadGenerate(uint8_t type, uint8_t angle, int16_t x, int16_t y, int16_t z, std::vector<Vertex> &verticie, std::vector<GLuint> &indicise)
{
	int i = verticie.size();
	switch (angle) {
	case 0: // UP
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, 1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, 1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, 1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, 1.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 3);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 1);
		indicise.push_back(i + 0);
		break;
	case 1: // Down
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, -1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, -1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, -1.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, -1.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 3);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 1);
		indicise.push_back(i + 0);
		break;
	case 2: // Front
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, 0.f, 1.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y			,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, 0.f, 1.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y			,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, 0.f, 1.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, 0.f, 1.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 3);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 1);
		indicise.push_back(i + 0);
		break;
	case 3: // Back
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f	,(float)z),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(0.f, 0.f, -1.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(0.f, 0.f, -1.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(0.f, 0.f, -1.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f	,(float)z),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(0.f, 0.f, -1.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 3);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 1);
		indicise.push_back(i + 0);
		break;
	case 4: // Left
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f	,(float)z),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(-1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(-1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(-1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x + 1.f	,(float)y + 1.f	,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(-1.f, 0.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 3);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 1);
		indicise.push_back(i + 0);
		break;
	case 5: // Right
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f	,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 0) ,glm::vec3(1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z + 1.f),glm::vec3(1.f) ,texCord(type, angle, 1) ,glm::vec3(1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y		,(float)z),glm::vec3(1.f) ,texCord(type, angle, 2) ,glm::vec3(1.f, 0.f, 0.f) });
		verticie.push_back({ glm::vec3((float)x			,(float)y + 1.f	,(float)z),glm::vec3(1.f) ,texCord(type, angle, 3) ,glm::vec3(1.f, 0.f, 0.f) });

		indicise.push_back(i + 0);
		indicise.push_back(i + 3);
		indicise.push_back(i + 2);
		indicise.push_back(i + 2);
		indicise.push_back(i + 1);
		indicise.push_back(i + 0);
		break;
	}
}

bool World::face_is_clear(Chunk &chunk, int angle, Chunk** near_chank, int X, int Y, int Z)
{
	uint8_t exist = near_chank[angle] != NULL;
	uint8_t age;  /*x , y, z == size - 1 or 0*/
	uint8_t other_block; /* cheak if age is coverd by block*/
	int ax; int ay; int az;

	switch (angle)
	{
	case UP:
		ax = X;		ay = Y + 1;		az = Z;
		age = Y == (HIGHT_SIZE - 1);
		if (exist == 1)
			other_block = near_chank[angle]->getCube(X, 0, Z) != 0;
		break;
	case DOWN:
		ax = X;		ay = Y - 1;		az = Z;
		age = Y == (0);
		if (exist == 1)
			other_block = near_chank[angle]->getCube(X, HIGHT_SIZE - 1, Z) != 0;
		break;
	case FRONT:
		ax = X;		ay = Y;		az = Z + 1;
		age = Z == (SIZE - 1);
		if (exist == 1)
			other_block = near_chank[angle]->getCube(X, Y, 0) != 0;
		break;	  
	case BACK:	  
		ax = X;		ay = Y;		az = Z - 1;
		age = Z == (0);
		if (exist == 1)
			other_block = near_chank[angle]->getCube(X, Y, SIZE - 1) != 0;
		break;	   
	case LEFT:	
		ax = X + 1;		ay = Y;		az = Z;
		age = X == (SIZE - 1);
		//exist = near_chank[RIGHT] == NULL;
		if (exist == 1)
			other_block = near_chank[angle]->getCube(0, Y, Z) != 0;
		break;	   
	case RIGHT:	 
		ax = X - 1;		ay = Y;		az = Z;
		age = X == (0);
		if (exist == 1)
			other_block = near_chank[angle]->getCube(SIZE - 1, Y, Z) != 0;
		break;
	}

	/*if (age == 1 && exist == 0 || age == 1 && other_block == 0) {
		return true;
	}*/
	if ( age == 1 && exist == 1 && other_block == 0) {
		return true;
	}
	if (age == 1 && exist == 0) {
		return false;
	}
	else if (chunk.getCube(ax, ay, az) == 0) {
		return true;
	}
	else
		return false;
		
}

Mesh* World::Genurate_Chunk(Chunk &chunk)
{
	std::vector<Vertex> verticies;
	std::vector<GLuint> indicies;

	uint8_t type;
	int16_t Y = 0;
	for (int16_t Y = 0; Y < HIGHT_SIZE; Y++) {
		for (int16_t Z = 0; Z < SIZE; Z++) {
			for (int16_t X = 0; X < SIZE; X++) {
				type = chunk.getCube(X, Y, Z);



				if (type != 0) {
					if (face_is_clear(chunk, UP, chunk.Get_near_list(), X, Y, Z) == true) //UP
						QuadGenerate(type, UP, X, Y, Z, verticies, indicies);

					if (face_is_clear(chunk, DOWN, chunk.Get_near_list(), X, Y, Z) == true) //UP
						QuadGenerate(type, DOWN, X, Y, Z, verticies, indicies);

					if (face_is_clear(chunk, FRONT, chunk.Get_near_list(), X, Y, Z) == true) //UP
						QuadGenerate(type, FRONT, X, Y, Z, verticies, indicies);

					if (face_is_clear(chunk, BACK, chunk.Get_near_list(), X, Y, Z) == true) //UP
						QuadGenerate(type, BACK, X, Y, Z, verticies, indicies);

					if (face_is_clear(chunk, LEFT, chunk.Get_near_list(), X, Y, Z) == true) //UP
						QuadGenerate(type, LEFT, X, Y, Z, verticies, indicies);

					if (face_is_clear(chunk, RIGHT, chunk.Get_near_list(), X, Y, Z) == true) //UP
						QuadGenerate(type, RIGHT, X, Y, Z, verticies, indicies);
				}
			}
		}
	}
	glm::vec3 start(0.f, 0.f, 0.f);
	start.x = 0.5f * (float)WORLD_X_CHUNK;
	start.y = (float)WORLD_Y_CHUNK ;
	start.z = 0.5f * (float)WORLD_Z_CHUNK;

	Mesh* mesh = new Mesh(verticies.data(), verticies.size(), indicies.data(), indicies.size());
	mesh->setPosition((chunk.getPosition() - start)* glm::vec3((float)SIZE, (float)HIGHT_SIZE, (float)SIZE));
	return mesh;
}

utils::NoiseMap World::Generate_tarraing(utils::NoiseMap &heightMap, glm::vec3 positon, int seed, float Retio)
{
	module::Perlin myModule;

	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(SIZE, SIZE);
	heightMapBuilder.SetBounds(Retio *positon.x, Retio * (positon.x+1), Retio * positon.z, Retio * (positon.z + 1));
	heightMapBuilder.Build();

	return heightMap;
}