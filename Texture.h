#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Texture
{
protected:
	uint32_t	m_texture;
	uint32_t	m_target;
	bool		m_mipmaps;
	int32_t		m_internal_format;
	uint32_t	m_format;

public:

	// Construct Null Texture (for texture reset bind)
	Texture();
	~Texture();

	virtual void bind() {}
};

class Texture2D : public Texture
{
private:
	uint32_t	m_width,
				m_height;

public:

    Texture2D( bool mipmaps, const std::string &png_file );
    Texture2D( uint32_t width, uint32_t height, uint32_t components, uint32_t format, bool mipmaps, void* data );
    ~Texture2D();

    void bind();
};

class Texture3D : public Texture2D
{
private:
	uint32_t	m_width,
				m_height,
				m_depth;

public:

    Texture3D( bool mipmaps, const std::string &png_file );
    Texture3D( uint32_t width, uint32_t height, uint32_t depth, uint32_t components, uint32_t format, bool mipmaps, void* data );
    ~Texture3D();

    void bind();
};

class TextureArray : public Texture
{
private:
	uint32_t	m_width,
				m_height,
				m_layers;
public:

	TextureArray( bool mipmaps, const std::vector<std::string>& png_files );
	TextureArray( bool mipmaps, const std::string& png_file );
	~TextureArray();

	void bind();
};
