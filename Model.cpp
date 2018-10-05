#include "Model.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

enum D3DLineEnum
{
	D3D_PRIMITIVE_BEGIN		= 0,
	D3D_PRIMITIVE_END		= 1,
	D3D_VERTEX_XYZ,
	D3D_VERTEX_XYZRGBA,
	D3D_VERTEX_XYZST,
	D3D_VERTEX_XYZSTRGBA,
	D3D_VERTEX_XYZNXNYNZ,
	D3D_VERTEX_XYZNXNYNZRGBA,
	D3D_VERTEX_XYZNXNYNZST,
	D3D_VERTEX_XYZNXNYNZSTRGBA,

	D3D_BLOCK,
	D3D_CYLINDER,

	D3D_LAST_LINE
};

enum D3DPrimitiveEnum
{
	pr_pointlist = 1,
    pr_linelist,
    pr_linestrip,
    pr_trianglelist,
    pr_trianglestrip,
    pr_trianglefan
};


Model::Model()
{
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
}

Model::~Model()
{
	glDeleteBuffers(1, &this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}

void Model::update()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(), this->vertices.data(), GL_DYNAMIC_DRAW);
}

void Model::render()
{
	glBindVertexArray(this->vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
}

void Model::addVertex( float x, float y, float z )
{
	Vertex v;
	v.x = x;
	v.y = y;
	v.z = z;

	this->vertices.push_back(v);
}

size_t Model::getVertexCount() const
{
	return this->vertices.size();
}

size_t Model::getTriangleCount() const
{
	return this->triangles.size();
}

bool Model::loadFromGMMOD(std::string filename)
{
	std::fstream file;

	file.open( filename, std::ios::in );

	if ( !file.is_open() )
	{
		std::cout << "Unable to open the file \"" << filename << "\"!" << std::endl;
		return false;
	}

	// -- Read/skip the header
	std::string line = "";
	char cstr_line[256];

	file.getline(cstr_line, 127);
	file.getline(cstr_line, 127);
	file.getline(cstr_line, 127);

	while (!file.eof())
	{
		file.getline(cstr_line, 255);
		line = cstr_line;
		std::stringstream ss(line);

        int type = 0;
        ss >> type;

        // 0 == primitive begin
        // 1 == primitive end
        if (type >= 2 && type <= 9)
		{
			float x, y, z, nx, ny, nz, s, t;
			ss >> x;
			ss >> y;
			ss >> z;
			ss >> nx;
			ss >> ny;
			ss >> nz;
			ss >> s;
			ss >> t;
		}

	}

	file.close();

	return true;
}

bool Model::load(std::string filename)
{
	std::fstream file;

	file.open( filename, std::ios::binary | std::ios::in );

	if ( !file.is_open() )
	{
		std::cout << "Unable to open the file \"" << filename << "\"!" << std::endl;
		return false;
	}

	char c = 0;
	char header[3];

	// -- Version + chunk header
	file.read( &c, 1 );
	file.read( header, 3 );

	// -- Read vertices to the file
	size_t vertex_count = 0;
    file.read( (char*)&vertex_count, 4 );

    for (size_t i = 0; i<vertex_count;  i++)
	{
		Vertex v;
		file.read( (char*)&v.x, sizeof(float) );
		file.read( (char*)&v.y, sizeof(float) );
		file.read( (char*)&v.z, sizeof(float) );
		file.read( (char*)&v.nx, sizeof(float) );
		file.read( (char*)&v.ny, sizeof(float) );
		file.read( (char*)&v.nz, sizeof(float) );
		file.read( (char*)&v.s, sizeof(float) );
		file.read( (char*)&v.t, sizeof(float) );
		this->vertices.push_back(v);
	}

	// -- Read triangles to the file
    size_t tri_count = 0;
    file.read( (char*)&tri_count, 4 );

    for (size_t i = 0; i<tri_count;  i++)
	{
		std::array<uint32_t,3> t;

		file.read( (char*)&t[0], sizeof(uint32_t) );
		file.read( (char*)&t[1], sizeof(uint32_t) );
		file.read( (char*)&t[2], sizeof(uint32_t) );

		this->triangles.push_back(t);
	}

	// V-- Version + chunk header
	file.read( &c, 1 );
	file.read( header, 3 );

	// -- Write vertices to the file
	vertex_count = 0;
    file.read( (char*)&vertex_count, 4 );

    for (size_t i = 0; i<vertex_count;  i++)
	{
		std::array<float,3> v;

		file.read( (char*)&v[0], sizeof(float) );
		file.read( (char*)&v[1], sizeof(float) );
		file.read( (char*)&v[2], sizeof(float) );

		this->collision.vertices.push_back(v);
	}

	// -- Write triangles to the file
    tri_count = this->getTriangleCount();
    file.read( (char*)&tri_count, 4 );

	for (size_t i = 0; i<tri_count;  i++)
	{
		std::array<uint32_t,3> t;

		file.read( (char*)&t[0], sizeof(uint32_t) );
		file.read( (char*)&t[1], sizeof(uint32_t) );
		file.read( (char*)&t[2], sizeof(uint32_t) );

		this->collision.triangles.push_back(t);
	}

	file.close();

	return true;
}

bool Model::save(std::string filename)
{
	std::fstream file;

	file.open( filename, std::ios::binary | std::ios::out );

	if ( !file.is_open() )
	{
		std::cout << "Unable to open the file \"" << filename << "\"!" << std::endl;
		return false;
	}

	char c = 0;

	// -- Version + chunk header
	c = 1;
	file.write( &c, 1 );
	c = 'M'; file.write( &c, 1 );
	c = 'O'; file.write( &c, 1 );
	c = 'D'; file.write( &c, 1 );

	// -- Write vertices to the file
	size_t vertex_count = this->getVertexCount();
    file.write( (char*)&vertex_count, 4 );

    for (vertex_vec_t::iterator v = this->vertices.begin(); v != this->vertices.end(); v++)
	{
		file.write( (char*)&v->x, sizeof(float) );
		file.write( (char*)&v->y, sizeof(float) );
		file.write( (char*)&v->z, sizeof(float) );
		file.write( (char*)&v->nx, sizeof(float) );
		file.write( (char*)&v->ny, sizeof(float) );
		file.write( (char*)&v->nz, sizeof(float) );
		file.write( (char*)&v->s, sizeof(float) );
		file.write( (char*)&v->t, sizeof(float) );
	}

	// -- Write triangles to the file
    size_t tri_count = this->getTriangleCount();
    file.write( (char*)&tri_count, 4 );

    for (size_t ti = 0; ti < this->triangles.size(); ti++)
	{
	    std::array<uint32_t, 3> t = this->triangles[ti];
		uint32_t v1 = t[0];
		uint32_t v2 = t[1];
		uint32_t v3 = t[2];
		file.write( (char*)&v1, sizeof(float) );
		file.write( (char*)&v2, sizeof(float) );
		file.write( (char*)&v3, sizeof(float) );
	}

	// V-- Version + chunk header
	c = 1;
	file.write( &c, 1 );
	c = 'C'; file.write( &c, 1 );
	c = 'O'; file.write( &c, 1 );
	c = 'L'; file.write( &c, 1 );

	// -- Write vertices to the file
	vertex_count = this->getVertexCount();
    file.write( (char*)&vertex_count, 4 );

    for (size_t vi = 0; vi < this->collision.vertices.size(); vi++)
	{
	    std::array<float, 3> v = this->collision.vertices[vi];
		float x = v[0];
		float y = v[1];
		float z = v[2];
		file.write( (char*)&x, sizeof(float) );
		file.write( (char*)&y, sizeof(float) );
		file.write( (char*)&z, sizeof(float) );
	}

	// -- Write triangles to the file
    tri_count = this->getTriangleCount();
    file.write( (char*)&tri_count, 4 );

    for (size_t ti = 0; ti < this->collision.triangles.size(); ti++)
	{
	    std::array<uint32_t, 3> t = this->collision.triangles[ti];
		uint32_t v1 = t[0];
		uint32_t v2 = t[1];
		uint32_t v3 = t[2];
		file.write( (char*)&v1, sizeof(uint32_t) );
		file.write( (char*)&v2, sizeof(uint32_t) );
		file.write( (char*)&v3, sizeof(uint32_t) );
	}

	file.close();

	return true;
}
