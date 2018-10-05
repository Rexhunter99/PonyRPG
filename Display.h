#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>

#include <string>
#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>

class Display
{
public:
	Display();
	Display( const std::string &ltx_file, bool debug = false );
	~Display();

	void makeCurrent() const;
	void swapBuffers() const;

	GLFWwindow* getWindow() const;

protected:

private:
	Display( const Display & display ) {};

	GLFWwindow			*m_window;
	GLFWmonitor			*m_monitor;
	const GLFWvidmode	*m_vidmode;
};

#endif // DISPLAY_H
