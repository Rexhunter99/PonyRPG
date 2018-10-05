#include "Input.h"

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

static int last_kb_scancode = -1;


void keyboard_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if ( action == GLFW_PRESS )
	{

	}
	else if ( action == GLFW_RELEASE )
	{
		last_kb_scancode = scancode;
		std::cout << "Key released, code: " << scancode << std::endl;
	}
}

void joystick_callback( int js, int event )
{
    if (event == GLFW_CONNECTED)
    {
        // -- The joystick was connected
        std::string name = glfwGetJoystickName(js);
		std::cout << "Gamepad[" << js << "] connected: " << name << std::endl;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        // -- The joystick was disconnected
		std::cout << "Gamepad[" << js << "] disconnected" << std::endl;
    }
}

Input::Input( GLFWwindow *window )
{
	std::cout << "Input Engine Initialising..." << std::endl;

	// -- Keyboard/Mouse setup
	glfwSetKeyCallback( window, keyboard_callback );

	// -- Joystick/Gamepad setup
	glfwSetJoystickCallback( joystick_callback );

	for (auto js = GLFW_JOYSTICK_1; js < GLFW_JOYSTICK_LAST; js++)
	{
		if ( glfwJoystickPresent( js ) )
		{
			std::string name = glfwGetJoystickName( js );
			std::cout << "Gamepad[" << js << "] connected: " << name << std::endl;
		}
	}

	std::cout << "Status: OK" << std::endl;
}

Input::~Input()
{
	glfwSetJoystickCallback( nullptr );
}


void Input::poll() const
{
	glfwPollEvents( );
}
