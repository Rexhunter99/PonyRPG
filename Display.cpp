#include "Display.h"
#include "ClassMap.h"

//#include <GL/glext.h>
#include <iostream>

static bool is_glfw_init = false;


void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport( 0, 0, width, height );
	// TODO: Resize framebuffer here
}

void error_callback( int code, const char* description )
{
    std::cerr << std::endl;
    std::cerr << "\nGLFW Error[" << code << "]: " << description << std::endl;
}


Display::Display( const std::string &ltx_file, bool debug )
{
	std::cout << "Display Engine Initialising..." << std::endl;

	ClassMap ltx_config(ltx_file);
	//ltx_config.dbgPrint();

	if ( !is_glfw_init )
    {
    	glfwSetErrorCallback( error_callback );

		if ( glfwInit() == GLFW_TRUE )
		{
			is_glfw_init = true;
		}
		else
		{
			is_glfw_init = false;
			std::cout << "Status: FAIL" << std::endl;
			return;
		}
    }

    glfwDefaultWindowHints();

    // -- Retrieve the primary monitor and its video mode
    int i_vidmode = 0;
    GLFWmonitor* current_monitor = glfwGetPrimaryMonitor();

    m_vidmode = glfwGetVideoMode(current_monitor);
    const GLFWvidmode* vidmodes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &i_vidmode);

    for (auto i=0; i < i_vidmode; i++)
	{
		if ( vidmodes[i].redBits != 8 || vidmodes[i].greenBits != 8 || vidmodes[i].blueBits != 8 || vidmodes[i].refreshRate != m_vidmode->refreshRate )
		{
			continue;
		}

		//std::cout << "- Display CFG | " << vidmodes[i].width << "x" << vidmodes[i].height << " 32-bits" << std::endl;
	}

    m_monitor = current_monitor;
    int width = 960;
    int height = 540;

    glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, debug );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    switch (ltx_config.getInt("display", "d_window_mode"))
	{
		case 0: {
			width = ltx_config.getInt("display", "d_width");
			height = ltx_config.getInt("display", "d_height");
			m_monitor = nullptr;
			glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
			glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );
		}
		break;
		case 1: {
			width = m_vidmode->width;
			height = m_vidmode->height;
			glfwWindowHint( GLFW_RED_BITS,   m_vidmode->redBits );
			glfwWindowHint( GLFW_GREEN_BITS, m_vidmode->greenBits );
			glfwWindowHint( GLFW_BLUE_BITS,  m_vidmode->blueBits );
			glfwWindowHint( GLFW_REFRESH_RATE, m_vidmode->refreshRate );
		}
		break;
		case 2: {
			width = m_vidmode->width;
			height = m_vidmode->height;
			m_monitor = nullptr;
			glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
			glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
		}
		break;
	}

    glfwWindowHint( GLFW_DEPTH_BITS, 32 );

    m_window = glfwCreateWindow( width, height, "Breeze Engine", m_monitor, nullptr );

    if ( m_window == nullptr )
	{
		//throw exception here;
		std::cerr << "\nFailed to create the GLFW 3 window\n" << std::endl;;
		std::cerr << "Status: FAIL" << std::endl;;
		return;
	}

	this->makeCurrent();

	int glew_status = glewInit();
	if ( glew_status != GLEW_OK )
	{
		std::cerr << "GLEW: Failed to initialise!" << std::endl;
	}

	glfwSetFramebufferSizeCallback(this->m_window, resize_callback);

	std::cout << "OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Shader Version:  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "Status: OK" << std::endl;
}

Display::~Display()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}

	if (is_glfw_init)
	{
		glfwTerminate();
	}
}

GLFWwindow* Display::getWindow() const
{
	return this->m_window;
}

void Display::makeCurrent() const
{
	glfwMakeContextCurrent(this->m_window);
}

void Display::swapBuffers() const
{
	glfwSwapBuffers(this->m_window);
}
