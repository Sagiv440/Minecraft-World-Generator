#pragma once
#include "libs.h";
namespace sagiv {
	float Vector_DotProduct(glm::vec3 &v1, glm::vec3 &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	float Vector_Length(glm::vec3 &v)
	{
		return sqrtf(Vector_DotProduct(v, v));
	}

	glm::vec3 Vector_Normalise(glm::vec3 &v)
	{
		float l = Vector_Length(v);
		//return { v.x / l, v.y / l, v.z / l };
		return glm::vec3(v.x / l, v.y / l, v.z / l);
	}

	glm::vec3 Vector_CrossProduct(glm::vec3 &v1, glm::vec3 &v2)
	{
		glm::vec3 v(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
		return v;
	}

	bool get_obj_indicies(char* s, float &v1, float &v2, float &v3, float &t1, float &t2, float &t3)
	{
		char junk;
		bool texcoord_ON = false;
		unsigned int i = 0;
		while (s[i] != '\0') {
			if (s[i] == '/') {
				s[i] = ' ';
				texcoord_ON = true;
			}
			i++;
		}
		std::strstream sl;
		sl << s;
		if (texcoord_ON)
			sl >> junk >> v1 >> t1 >> v2 >> t2 >> v3 >> t3;
		else {
			sl >> junk >> v1 >> v2 >> v3;
			t1 = 0; t2 = 0; t3 = 0;
		}
		return texcoord_ON;


	}

	bool LoadFromObjectFile(std::string sFilename, Vertex *&vertex, GLuint *&indices, unsigned int &vs, unsigned int &is)
	{
		std::fstream in_file;
		in_file.open(sFilename);
		if (!in_file.is_open()) {
			return false;
		}
		unsigned int vertex_size = 0;
		unsigned int index_size = 0;
		unsigned int texcoord_size = 0;
		float x, y, z, x1, y1, z1;
		std::string junk;
		std::vector<glm::vec2> texcoord;

		while (!in_file.eof())
		{
			char line[128];
			in_file.getline(line, 128);

			std::strstream s;
			s << line;

			if (line[0] == 'v' && line[1] != 't') {
				vertex_size++;
			}
			if (line[0] == 'v' && line[1] == 't') {
				s >> junk >> x >> y;
				texcoord.push_back(glm::vec2(x, y));
			}
			if (line[0] == 'f') {
				index_size++;
			}
		}
		in_file.close();
		in_file.open(sFilename);
		if (!in_file.is_open()) {
			return false;
		}
		vertex = (Vertex*)malloc(vertex_size * sizeof(Vertex));
		indices = (GLuint*)malloc(index_size * 3 * sizeof(GLuint));
		unsigned int v_s = 0;
		unsigned int t_s = 0;
		unsigned int i_s = 0;
		bool texcoord_ON;

		glm::vec3 line1, line2, normal, a;
		while (!in_file.eof())
		{
			char line[128];
			in_file.getline(line, 128);

			std::strstream s;
			s << line;

			if (line[0] == 'v' && line[1] != 't') {

				s >> junk >> x >> y >> z;
				vertex[v_s].position = glm::vec3(x, y, z);
				vertex[v_s].color = glm::vec3(1.f, 0.0f, 0.f);
				vertex[v_s].texcoord = glm::vec2(0.0f, 0.0f);
				vertex[v_s].normal = glm::vec3(0.f);
				v_s++;
			}
			if (line[0] == 'f') {

				texcoord_ON = get_obj_indicies(line, x, y, z, x1, y1, z1);


				indices[i_s] = x - 1;
				indices[i_s + 1] = y - 1;
				indices[i_s + 2] = z - 1;


				line1 = vertex[(int)x - 1].position - vertex[(int)y - 1].position;
				line2 = vertex[(int)x - 1].position - vertex[(int)z - 1].position;
				normal = Vector_CrossProduct(line1, line2);
				normal = Vector_Normalise(normal);

				if (vertex[(int)x - 1].normal == glm::vec3(0.f)) {
					vertex[(int)x - 1].normal = normal;
				}
				else {
					a = normal + vertex[(int)x - 1].normal;
					vertex[(int)x - 1].normal = Vector_Normalise(a);
				}

				if (vertex[(int)y - 1].normal == glm::vec3(0.f)) {
					vertex[(int)y - 1].normal = normal;
				}
				else {
					a = normal + vertex[(int)y - 1].normal;
					vertex[(int)y - 1].normal = Vector_Normalise(a);
				}

				if (vertex[(int)z - 1].normal == glm::vec3(0.f)) {
					vertex[(int)z - 1].normal = normal;
				}
				else {
					a = normal + vertex[(int)z - 1].normal;
					vertex[(int)z - 1].normal = Vector_Normalise(a);
				}
				if (texcoord_ON) {
					vertex[(int)x - 1].texcoord = texcoord[(int)x1 - 1];
					vertex[(int)y - 1].texcoord = texcoord[(int)y1 - 1];
					vertex[(int)z - 1].texcoord = texcoord[(int)z1 - 1];
				}

				i_s += 3;




			}
		}

		vs = vertex_size;
		is = index_size * 3;
		in_file.close();
		return true;
	}
}



