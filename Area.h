#pragma once

#ifndef AREA_H
#define AREA_H

#include <cstdint>
#include "Texture.h"


class Area
{
public:

	Area();
	~Area();
	Area( const Area &a );

	uint32_t getWidth() const;
	uint32_t getLength() const;

	float getHeight( uint32_t x, uint32_t y ) const;

	bool load( const std::string &filename );

	// -- Save the map data in the new format
	bool save( const std::string &filename );

private:

	class Cell
	{
	public:
		uint32_t x;
		uint32_t y;
		uint32_t width;
		uint32_t length;
	};

	float	m_scale_x,
			m_scale_y,
			m_scale_z;

	std::vector<float>	m_heightmap;
	Texture2D			m_diffusemap,
						m_normalmap;

	std::vector<Texture2D*> m_textures;
};

#endif // AREA_H
