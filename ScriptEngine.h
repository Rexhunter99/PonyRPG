#pragma once

#include <cstdint>
#include <string>

class ScriptEngine
{
public:

	ScriptEngine();
	~ScriptEngine();

	bool runScript(const std::string& file);

private:

	intptr_t	m_state;

};
