#include "ScriptEngine.h"


#include <iostream>
#include <string>
#include <vector>
#include <lua.hpp>


static int result = -1;


void print_lua_error( lua_State* state )
{
	std::string message = lua_tostring(state, -1);
	std::cout << "\nLUA Error:\n" << message << std::endl;
	lua_pop(state, 1);
}

ScriptEngine::ScriptEngine()
{
	lua_State* state;

	std::cout << "Script Engine Initialising..." << std::endl;

	this->m_state = (intptr_t)luaL_newstate();

	if ( !this->m_state )
	{
		std::cout << "\nFailed to create the LUA state!" << std::endl;
		std::cout << "Status: FAIL" << std::endl;
		return;
	}

	state = (lua_State*)this->m_state;

	luaL_openlibs(state);

	std::cout << "Status: OK" << std::endl;
}

ScriptEngine::~ScriptEngine()
{
	if ( this->m_state )
	{
		lua_close((lua_State*)this->m_state);
	}
}


bool ScriptEngine::runScript(const std::string& file)
{
	lua_State* state = (lua_State*)this->m_state;

	result = luaL_loadfile(state, file.c_str());

	if ( result != LUA_OK )
	{
		print_lua_error(state);
		return false;
	}

	result = lua_pcall(state, 0, LUA_MULTRET, 0);

	if ( result != LUA_OK )
	{
		print_lua_error(state);
		return false;
	}

	return true;
}
