
#include "Area.h"

#include <fstream>


Area::Area()
{

}

Area::~Area()
{

}


bool Area::load(const std::string& filename)
{
	std::fstream f;

	this->m_width = 1024;

	f.open(filename, std::ios::binary | std::ios::in);

	if ( !f.is_open() )
	{
		return false;
	}

	uint8_t dmap[1024 * 1024];
	uint16_t dmap2[1024 * 1024];

	// -- Read Heightmap in
	f.read(dmap, 1024*1024);

	for ( size_t i = 0; i < 1024 * 1024; i++ )
	{
		this->m_heightmap.push_back()
	}

	f.close();

	return true;
}
