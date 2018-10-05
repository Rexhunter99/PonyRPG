
#include "Shader.h"
#include "Exceptions.h"

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>



/*********************************************************************/
/*** Shader Implementation */

Shader::Shader()
{
	this->program = glCreateProgram();
}

Shader::~Shader()
{
	glDeleteProgram( this->program );
}

void Shader::bind()
{
	glUseProgram( this->program );
}

void Shader::reset()
{
	glDeleteProgram( this->program );
	this->program = glCreateProgram();
}

void Shader::addProgram( std::string &text, uint32_t type )
{
	std::string src_str	= "";
	const char* src = nullptr;
	int len			= 0;
	int status		= 0;
	FILE* fp		= nullptr;
	uint32_t shader = glCreateShader( type );

	// -- Validate that the shader was created
	if ( shader == 0 )
	{
		std::cerr << "[class:Shader] Failed to create the shader!" << std::endl;
		return;
	}

	if ( (fp = fopen( text.c_str(), "rb" )) != nullptr )
	{
		fseek( fp, 0, SEEK_END );
		len = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		char* fragShader = new char [len];

		fread( fragShader, len, 1, fp );
		src_str = fragShader;

		fclose( fp );
		delete fragShader;
	}
	else
	{
		std::cerr << "[class:Shader] Failed to open the shader source \"" << text << "\" for reading!" << std::endl;
		glDeleteShader( shader );
		return;
	}
	src = src_str.c_str();

	glShaderSource( shader, 1, &src, &len );
	glCompileShader( shader );

	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	if ( status == 0 )
	{
		int info_log_len = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &info_log_len );

		char *info_log = new char [ info_log_len ];
		glGetShaderInfoLog( shader, info_log_len, nullptr, info_log );

		std::cerr << "[class:Shader] Failed to compile shader source:\n" << text << "\n===========================================\n" << info_log << "\n===========================================" << std::endl;

		delete [] info_log;
		glDeleteShader( shader );

		#ifdef THROW_EXCEPTIONS
		throw custom_exception( "Failed to compile the shader!" );
		#endif
		return;
	}

    glAttachShader( this->program, shader );

	// -- Mark this shader for deletion, it will happen when glDeleteProgram occurs, which detaches all shaders
    glDeleteShader( shader );
}

void Shader::compileShaders()
{
	int status = 0;

	glLinkProgram( this->program );
	glGetProgramiv( this->program, GL_LINK_STATUS, &status );

    if ( status == 0 )
	{
		char infolog[1024];
		glGetProgramInfoLog( this->program, 1024, nullptr, infolog );
        std::cerr << "[OPENGL ERROR] Failed to link shader program\n===========================================\n" << infolog << "\n===========================================" << std::endl;
        return;
    }
}

void Shader::addVertexShader( std::string filename )
{
	this->addProgram( filename, GL_VERTEX_SHADER );
}

void Shader::addGeometryShader( std::string filename )
{
	this->addProgram( filename, GL_GEOMETRY_SHADER );
}

void Shader::addFragmentShader( std::string filename )
{
	this->addProgram( filename, GL_FRAGMENT_SHADER );
}

void Shader::bindFragData( uint32_t color, std::string name )
{
	glBindFragDataLocation( this->program, color, name.c_str() );
}

void Shader::addUniform( std::string name )
{
	int uniformLoc = glGetUniformLocation( this->program, name.c_str() );

	if ( uniformLoc == -1 )
	{
		std::cerr << "OpenGL :: Failed to get the Uniform " << name << " location in the shader program!" << std::endl;
		return;
	}

	this->uniforms.insert( std::unordered_map<std::string,int>::value_type( name, uniformLoc ) );
}

void Shader::setUniform1f( std::string name, float value )
{
	glUniform1f( this->uniforms.find( name )->second, value );
}

void Shader::setUniform4f( std::string name, float value[4] )
{
	glUniform4f( this->uniforms.find( name )->second, value[0], value[1], value[2], value[3] );
}

void Shader::setUniform1i( std::string name, int value )
{
	glUniform1i( this->uniforms.find( name )->second, value );
}

void Shader::setUniformMatrix( std::string name, float value[16] )
{
	glUniformMatrix4fv( this->uniforms.find( name)->second, 1, GL_FALSE, value );
}


void Shader::addAttrib( std::string name )
{
	glGetError();
	int attribLoc = glGetAttribLocation( this->program, name.c_str() );

	if ( attribLoc == -1 )
	{
		std::cerr << "OpenGL :: Failed to get the Attribute " << name << " location in the shader program!" << std::cerr;
		return;
	}

	this->attribs.insert( std::unordered_map<std::string,int>::value_type( name, attribLoc ) );
}

uint32_t Shader::getAttrib( std::string name )
{
	return glGetAttribLocation( this->program, name.c_str() );
}

void Shader::setAttrib1f( std::string name, float value )
{
	glVertexAttrib1f( this->attribs.find( name )->second, value );
}

void Shader::setAttrib2f( std::string name, float value[2] )
{
	glVertexAttrib2fv( this->attribs.find( name )->second, value );
}

void Shader::setAttrib3f( std::string name, float value[3] )
{
	glVertexAttrib3fv( this->attribs.find( name )->second, value );
}

void Shader::setAttrib4f( std::string name, float value[4] )
{
	glVertexAttrib4fv( this->attribs.find( name )->second, value );
}

void Shader::setDefaults()
{
    this->setUniform1i( "texture", 0 );
	this->setUniform4f( "g_SunLightSource.position", (float*)glm::value_ptr( glm::vec4( 0, -1, 0, 0 ) ) );
	this->setUniform4f( "g_SunLightSource.diffuse", (float*)glm::value_ptr( glm::vec4( 1, 1, 1, 1 ) ) );
	this->setUniform4f( "g_SunLightSource.ambient", (float*)glm::value_ptr( glm::vec4( 0.4, 0.4, 0.4, 1 ) ) );
	this->setUniform1f( "g_SunLightSource.specular", 1.0f );
}
