
#include "Texture.h"

#include <GL/glew.h>
#include <png.h>
#include <cstdlib>
#include <iostream>
#include <vector>


bool loadPNG( const std::string& filename, uint32_t& width, uint32_t& height, int32_t& internalFormat, uint32_t& format, std::vector<uint8_t>& data )
{
	// -- Header for testing if it is a png
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen( filename.c_str(), "rb");
	if (!fp)
	{
        std::cerr << "loadPNG()\nError opening picture file: \"" << filename << "\"" << std::endl;
		return false;
	}

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	if ( !(!png_sig_cmp(header, 0, 8)) )
	{
		fclose(fp);
		std::cerr << "loadPNG()\nThe specified file is not a valid PNG." << std::endl;
		return false;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr)
	{
		fclose(fp);
		return false;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return false;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return false;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int			bit_depth,
				color_type;
	png_uint_32	twidth,
				theight;

	//int channels = png_get_channels( png_ptr, info_ptr );

	// -- Get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, nullptr, nullptr, nullptr);
	width = twidth;
	height = theight;

	// -- Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// -- Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// -- Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = (png_byte*)malloc( rowbytes * theight );
	if (!image_data)
	{
		// -- Clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		std::cerr << "loadPNG() :: Failed to allocate the image data for the PNG." << std::endl;

		return false;
	}

	// -- Row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[ theight ];
	if (!row_pointers)
	{
		// -- Clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free( image_data );
		fclose(fp);
		std::cerr << "loadTexture() :: Failed to allocate the row pointers for the PNG." << std::endl;
		return false;
	}

	// -- Set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned int i = 0; i < theight; ++i)
	{
		// -- Row_pointers[pic.m_height - 1 - i] = image_data + i * rowbytes;
		row_pointers[i] = image_data + i * rowbytes;
	}

	// -- Read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	internalFormat = GL_RGB8;
	format = GL_RGB;

	// -- Upload the raster data to the graphics hardware
	if ( color_type == PNG_COLOR_TYPE_RGB )
	{
		if ( bit_depth == 8 )
			internalFormat = GL_RGB8;
		if ( bit_depth == 16 )
			internalFormat = GL_RGB16;
		format = GL_RGB;
	}
	else if ( color_type == PNG_COLOR_TYPE_RGBA )
	{
		if ( bit_depth == 8 )
			internalFormat = GL_RGBA8;
		if ( bit_depth == 16 )
			internalFormat = GL_RGBA16;
		format = GL_RGBA;
	}

	// -- Copy the data to the STL vector
	data.insert( data.end(), &image_data[0], &image_data[ rowbytes * theight ] );

	//-- Clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;
	free( image_data );
	fclose( fp );

	return true;
}


Texture::Texture()
{
	this->m_texture = 0;
}

Texture::~Texture()
{
	if ( this->m_texture != 0 )
	glDeleteTextures( 1, &this->m_texture );
}


Texture2D::Texture2D(bool mipmaps, const std::string& png_file)
{
	std::vector<uint8_t> image_data;


	glGenTextures(1, &this->m_texture);

	loadPNG( png_file, this->m_width, this->m_height, this->m_internal_format, this->m_format, image_data );

	// -- Apply the texture as current
	glBindTexture( GL_TEXTURE_2D, this->m_texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexImage2D( GL_TEXTURE_2D, 0, this->m_internal_format, this->m_width, this->m_height, 0, this->m_format, GL_UNSIGNED_BYTE, image_data.data() );

	if ( mipmaps )
	{
		glGenerateMipmap( GL_TEXTURE_2D );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	}
}

void Texture2D::bind()
{
	glBindTexture( GL_TEXTURE_2D, this->m_texture );
}

void Texture3D::bind()
{
	glBindTexture( GL_TEXTURE_3D, this->m_texture );
}



TextureArray::TextureArray( bool mipmaps, const std::vector<std::string>& png_files )
{
	std::vector<uint8_t> image_data;

	this->m_mipmaps = mipmaps;

	// TODO: Figure out how to FS:store/load the array
	// NOTE: Texture layers are stored as a column, not a row (vertical)
	for ( size_t i = 0; i < png_files.size(); i++ )
	{
		loadPNG( png_files[i], this->m_width, this->m_height, this->m_internal_format, this->m_format, image_data );
	}

	glGenTextures(1, &this->m_texture);
	glBindTexture( GL_TEXTURE_2D_ARRAY, this->m_texture );

	glTexStorage3D( GL_TEXTURE_2D_ARRAY, 1, this->m_internal_format, this->m_width, this->m_height, this->m_layers );

	glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, this->m_width, this->m_height, this->m_layers, this->m_format, GL_UNSIGNED_BYTE, image_data.data() );

	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	if ( this->m_mipmaps )
	{
		glGenerateMipmap( GL_TEXTURE_2D_ARRAY );
		glTexParameterf( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f );
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	}
}

void TextureArray::bind()
{
	glBindTexture( GL_TEXTURE_2D_ARRAY, this->m_texture );
}
