#include "ClassMap.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>

using namespace std;


#define PRINTERROR(tx) std::cerr << "File: " << __FILE__ << "\nLine: " << __LINE__ << "\n" << tx << std::endl


/*******************************************************************
** ClassMap implementation
**/

ClassMap::ClassMap()
{
	//ctor
}

ClassMap::ClassMap(const std::string &file)
{
	this->load(file);
}

ClassMap::~ClassMap()
{
	m_classes.clear();
}

void ClassMap::load(const std::string &file)
{
	std::stack<std::fstream> file_stack;
	std::fstream fs;

	fs.open( file, std::ios::in );

	if ( !fs.is_open() )
	{
		std::cerr << "Failed to open LTX file: \"" << file << "\"!" << std::endl;
	}

	if ( fs.is_open() )
	{
		int line_num = 0;
		std::string line = "";

		bool class_def = false;
		bool class_def_closed = false;
		bool postclass_def = false;
		std::string class_name = "";
		Class *class_ptr = nullptr;

        while (!fs.eof())
		{
			line_num++;
			getline(fs, line, '\n');

			//bool member_def = false;
			bool member_value_def = false;
			bool include_def = false;
			std::string include_file = "";
			std::string member_name = "";
			std::string member_value = "";
			std::string parent_class_name = "";
			int char_num = 0;

            for (string::const_iterator i=line.begin(); i!=line.end(); i++)
			{
				char c = *i;
				if ( !include_def )
				{
					if ( c == '[' )
					{
						class_def = true;
						class_def_closed = false;
						class_name = "";
						continue;
					}

					if ( class_def && c == ']' )
					{
						class_ptr = this->internalCreateClass( class_name );
						class_def = false;
						class_def_closed = true;
						continue;
					}

					if ( class_def_closed && c == ':' )
					{
						postclass_def = true;
						continue;
					}

					// -- Comment
					if ( c == ';' )
					{
						break;
					}

					if ( c == '=' )
					{
						member_value_def = true;
						continue;
					}

					if ( postclass_def && c == ',' )
					{
						class_ptr->addParent( parent_class_name );
					}
				}

				if ( c == '#' && !member_value_def )
				{
					if ( *(i+1) == 'i' && *(i+2) == 'n' && *(i+3) == 'c' && *(i+4) == 'l' && *(i+5) == 'u' && *(i+6) == 'd' && *(i+7) == 'e' )
					{
						i += 8;
						include_def = true;
						continue;
					}
				}

				if ( class_def )
				{
					if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '1' && c<= '9') || (c == '0') || (c == '_') )
					{
						if ( postclass_def )
						{
							parent_class_name.push_back(c);
						}
						else
						{
							class_name.push_back(c);
						}
					}
				}
				else if ( member_value_def )
				{
					if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '1' && c<= '9') || (c == '0') || (c == '_') )
					{
						member_value.push_back(c);
					}
				}
				else if ( include_def )
				{
					if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '1' && c<= '9') || (c == '0') || (c == '_') || ( c == '-' ) )
					{
						include_file.push_back(c);
					}
				}
				else if ( class_ptr )
				{
					if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '1' && c<= '9') || (c == '0') || (c == '_') )
					{
						member_name.push_back(c);
					}
				}

				char_num ++;
			}

			class_def_closed = false;

			if ( class_def )
			{
				// Oops the class wasn't 'closed'
				std::cerr << "File: " << file << "\n";
				std::cerr << "Line: " << line_num << "\n";
				std::cerr << "Index: " << char_num << "\n";
				std::cerr << "class definition does not have closing \']\'!" << std::endl;
				class_def = false;
			}

			if ( member_value_def )
			{
				this->setString( class_name, member_name, member_value );
			}
		}

		fs.close();
	}
}

void ClassMap::createClass( const std::string &name )
{
	// TODO: check if it exists first, if it does, throw an exception or create a warning
	m_classes.insert( std::pair<std::string, Class>(name, Class(name)) );
}

ClassMap::Class * ClassMap::internalCreateClass( const std::string &name )
{
	// TODO: check if it exists first, if it does, throw an exception or create a warning
	class_map_t::iterator it = m_classes.insert( std::pair<std::string, Class>(name, Class(name)) ).first;
	return &it->second;
}

std::string ClassMap::getString( std::string class_name, std::string member_name )
{
	class_map_t::iterator cmi = m_classes.find( class_name );

	if ( cmi != m_classes.end() )
	{
		Class *cls = &(cmi->second);
		Member *mem = cls->getMember(member_name);

		return mem->getString();
	}
	else
	{
		PRINTERROR( "class ClassMap | There is no class by this name!" );
		return "";
	}
}

bool ClassMap::getBool( std::string class_name, std::string member_name )
{
	std::string result = this->getString( class_name, member_name );

	if ( result == "True" || result == "true" || result == "1" )
	{
		return true;
	}
	else if ( result == "False" || result == "false" || result == "0" )
	{
		return false;
	}
	else
	{
		//std::cerr << "File: " << __FILE__ << "\nLine: " << __LINE__ << "\n" << "class ClassMap | Invalid resulting string!" << std::endl;
		PRINTERROR( "class ClassMap | Invalid resulting string!" );
		return false;
	}
}

int ClassMap::getInt( std::string class_name, std::string member_name )
{
	std::string result = this->getString( class_name, member_name );
	int result_i = 0;

	std::stringstream ss( result );
	ss >> result_i;

	if ( !ss.fail() )
	{
		return result_i;
	}
	else
	{
		std::cerr << "File: " << __FILE__ << "\nLine: " << __LINE__ << "\n" << "class ClassMap | Invalid resulting string!" << std::endl;
		return 0;
	}
}

float ClassMap::getFloat( std::string class_name, std::string member_name )
{
	std::string result = this->getString( class_name, member_name );
	float result_f = 0;

	std::stringstream ss( result );
	ss >> result_f;

	if ( ss.goodbit )
	{
		return result_f;
	}
	else
	{
		std::cerr << "File: " << __FILE__ << "\nLine: " << __LINE__ << "\n" << "class ClassMap | Invalid resulting string!" << std::endl;
		return 0.0f;
	}
}


void ClassMap::setBool( std::string class_name, std::string member_name, bool value )
{
	if ( value )
	{
		this->setString(class_name, member_name, "true");
	}
	else
	{
		this->setString(class_name, member_name, "false");
	}
}

void ClassMap::setInt( std::string class_name, std::string member_name, int value )
{
	std::stringstream ss;
	ss << value;
	std::string s_value = ss.str();
	this->setString(class_name, member_name, s_value);
}

void ClassMap::setFloat( std::string class_name, std::string member_name, float value )
{
	std::stringstream ss;
	ss << value;
	std::string s_value = ss.str();
	this->setString(class_name, member_name, s_value);
}

void ClassMap::setString( std::string class_name, std::string member_name, std::string value )
{
	class_map_t::iterator cmi = m_classes.find( class_name );

	if ( cmi == m_classes.end() )
	{
		std::pair<class_map_t::iterator, bool> res = m_classes.insert( std::make_pair(class_name, Class(class_name)) );
		cmi = res.first;
	}

	Class *cls = &(cmi->second);
	Member *mem = cls->getMember(member_name);

	if ( mem == nullptr )
	{
		if ( !cls->addMember(member_name, value) )
		{
			std::cerr << "LTX :: failed to add member!" << std::endl;
		}
	}
	else
	{
		mem->setString(value);
	}
}

void ClassMap::dbgPrint()
{
#ifdef _DEBUG

	for (auto it = this->m_classes.begin(); it != this->m_classes.end(); it++)
	{
		Class &cls = it->second;
		std::cout << "Class: " << cls.getName() << std::endl;
		cls.dbgPrint();
	}

#endif // _DEBUG
}


/*******************************************************************
** Class implementation
**/
ClassMap::Class::Class()
{
	this->m_name = "<nil>";
}

ClassMap::Class::Class( const std::string &name )
{
	this->m_name = name;
}

ClassMap::Class::~Class()
{
	this->m_members.clear();
}

void ClassMap::Class::addParent( const std::string &parent )
{
	//TODO
}

bool ClassMap::Class::addMember( const std::string &member, const std::string &value )
{
	if ( m_members.find( member ) == m_members.end() )
	{
		m_members.insert(make_pair(member, ClassMap::Member(member, value)));
		return true;
	}
	else
	{
		return false;
	}
}

std::string ClassMap::Class::getName() const
{
	return this->m_name;
}

ClassMap::Member* ClassMap::Class::getMember( const std::string &member )
{
	std::unordered_map<std::string, Member>::iterator it = m_members.find( member );

	if ( it != m_members.end() )
	{
		return &it->second;
	}

	return nullptr;
}

void ClassMap::Class::dbgPrint()
{
#ifdef _DEBUG

	for (auto it = this->m_members.begin(); it != this->m_members.end(); it++)
	{
		Member &mem = it->second;
		std::cout << "\t" << mem.getName() << " = " << mem.getString() << std::endl;
	}

#endif // _DEBUG
}


/*******************************************************************
** Member implementation
**/
ClassMap::Member::Member()
{
	this->m_name = "";
	this->m_value = "";
}

ClassMap::Member::Member(const std::string &name, const std::string &value)
{
	this->m_name = name;
	this->m_value = value;
}

ClassMap::Member::Member( const Member& member )
{
	this->m_name = member.m_name;
	this->m_value = member.m_value;
}

ClassMap::Member::~Member()
{

}

std::string ClassMap::Member::getName() const
{
	return this->m_name;
}

std::string ClassMap::Member::getString() const
{
	return this->m_value;
}

float ClassMap::Member::getFloat() const
{
	std::stringstream ss(this->m_value);
	float f;
	ss >> f;
	return f;//this->m_value;
}

int32_t ClassMap::Member::getInteger() const
{
	std::stringstream ss(this->m_value);
	int32_t i;
	ss >> i;
	return i;
}

template <typename T> std::vector<T> ClassMap::Member::getArray() const
{
	std::vector<T> v;
	std::string r = "";
	stringstream ss(this->m_value);

	while (std::getline(ss, r, ','))
	{
		T i;
		ss >> i;
		v.push_back(i);
	}

	return v;
}

void ClassMap::Member::setString( const std::string &value )
{
	this->m_value = value;
}
