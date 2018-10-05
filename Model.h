#pragma once

#include "CollisionModel.h"

#include <cstdint>
#include <array>
#include <vector>

class Animation;
class Model;


class Model
{
public:

	size_t getVertexCount() const;
	size_t getTriangleCount() const;
	bool loadFromGMMOD(std::string filename);
	bool load(std::string filename);
	bool save(std::string filename);
	void update();
	void render();

	void addVertex( float x, float y, float z );

	Model();
	~Model();

	//Model& operator + (const Model& m);

protected:
	friend class Animation;

	class Vertex
	{
	public:
		float x,y,z;
		float nx,ny,nz;
		float s,t;
	};

	typedef std::vector<Vertex> vertex_vec_t;
	typedef std::vector<std::array<uint32_t,3>> triangle_vec_t;

	std::string							name;
	std::vector<Vertex>					vertices;
	std::vector<std::array<uint32_t,3>> triangles;
	CollisionModel						collision;

private:
	uint32_t	vao, vbo;
};
