
#include "Display.h"
#include "Input.h"
#include "ScriptEngine.h"
#include "StringTable.h"
#include "Shader.h"
#include "Model.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <exception>
#include <iostream>
#include <iostream>
#include <sstream>
#include <chrono>


GLFWwindow	*main_window = nullptr;
GLFWmonitor	*main_monitor = nullptr;

enum GameStateEnum {
	GS_INTRO	= 0,
	GS_MENU,
	GS_LOADING,
	GS_WORLD,
	GS_MAPEDITOR,
	GS_QUIT,
	GS_POSTQUIT
} game_state;


int main(int argc, char **argv)
{
    int     width, height;
    int     frame = 0;
    bool    running = true;
    auto	timeStart = std::chrono::high_resolution_clock::now();
    auto	timePrev = timeStart;
    auto	timePrev2 = timeStart;
    int16_t	frames = 0;
    int64_t	ticks = 0;

    try
    {
		#ifdef _DEBUG
		Display display("user.ltx", true);
		#else
		Display display("user.ltx", false);
		#endif

		game_state = GS_INTRO;

		if( display.getWindow() )
		{
			display.makeCurrent();

			Input input(display.getWindow());
			ScriptEngine script_engine;
			script_engine.runScript("gamedata/scripts/test.lua");

			StringTable string_table("gamedata/localisation/en.xml");

			Shader sh_default;
			sh_default.addVertexShader("gamedata/shaders/default.vs");
			sh_default.addFragmentShader("gamedata/shaders/default.fs");
			sh_default.compileShaders();
			sh_default.bind();
			sh_default.addUniform("mvp");
			sh_default.addAttrib("position");
			//sh_default.addAttrib("texcoord");

			Model triangle;
			triangle.addVertex(0, 0, 0);
			triangle.addVertex(250, 0, 0);
			triangle.addVertex(0, 250, 0);
			triangle.update();

			while (!glfwWindowShouldClose(display.getWindow()))
			{
				if ( glfwGetKey( display.getWindow(), GLFW_KEY_F9 ) == GLFW_PRESS )
				{
					sh_default.reset();
					sh_default.addVertexShader("gamedata/shaders/default.vs");
					sh_default.addFragmentShader("gamedata/shaders/default.fs");
					sh_default.compileShaders();
					sh_default.bind();
					sh_default.addUniform("mvp");
					sh_default.addAttrib("position");
					//sh_default.addAttrib("texcoord");
				}

				glfwGetWindowSize( display.getWindow(), &width, &height );
				height = height > 0 ? height : 1;

				glClearColor( 0.0f, 0.0f, 0.1f, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				glm::mat4 projection(1.0f);
				glm::mat4 modelview(1.0f);

				switch (game_state)
				{
					case GS_INTRO:
					case GS_MENU:
					case GS_LOADING:
						projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -100.0f, 100.0f);
						break;

					case GS_WORLD:
						projection = glm::perspective(65.0f, (float)width/(float)height, 1.0f, 100.0f);
						modelview = glm::lookAt(glm::vec3(0.0f, -10.0f, 0.0f),
							glm::vec3(0.0f, 0.0f, 0.0f),
							glm::vec3(0.0f, 0.0f, 1.0f) );
						break;

					case GS_MAPEDITOR:
						break;
					case GS_QUIT:
					case GS_POSTQUIT:
						projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -100.0f, 100.0f);
						break;
				}

				modelview = glm::translate( glm::mat4(), glm::vec3(0.5f, 0.0f, 0.0f) );

				glm::mat4 mvp = modelview * projection;
				sh_default.setUniformMatrix("mvp", glm::value_ptr(mvp[0]));

				triangle.render();

				display.swapBuffers();
				frames++;
				input.poll();

				auto	timeCurrent = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> timeDelta(timeCurrent-timePrev);

				if ( timeDelta.count() >= (1.0 / 64.0) )
				{
					ticks++;
					timePrev = timeCurrent;
				}

				std::chrono::duration<double> timeDelta2(timeCurrent-timePrev2);

				if ( timeDelta2.count() >= (1.0) )
				{
					std::stringstream title;
					title << "Hunt Engine | FPS:" << frames;
					glfwSetWindowTitle(display.getWindow(), title.str().c_str());
					frames = 0;
					timePrev2 = timeCurrent;
				}
			}
		}
    }
    catch (std::exception &e)
    {
    	std::cerr << "======================================================\n";
    	std::cerr << "An exception was caught!\n";
    	std::cerr << e.what() << "\n";
    	std::cerr << "======================================================" << std::endl;
    }
    catch (...)
    {
    	std::cerr << "Unknown exception thrown!" << std::endl;
    }

	return 0;
}
