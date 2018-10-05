#pragma once

#include <GLFW/glfw3.h>
#include <string>


class Input
{
public:
	Input(GLFWwindow* window);
	~Input();

	void		poll() const;
	std::string	getName() const;

private:

};
