#pragma once
#ifndef CLASS_H
#define CLASS_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>


class ClassMap
{
private:

public:
	ClassMap();
	ClassMap(const std::string &file);
	~ClassMap();

	void createClass( const std::string &name );

	void load( const std::string &file );
	void save( const std::string &file );

	bool	getBool( std::string class_name, std::string member_name );
	int		getInt( std::string class_name, std::string member_name );
	float	getFloat( std::string class_name, std::string member_name );
	std::string	getString( std::string class_name, std::string member_name );
	template <typename T> std::vector<T> getArray( std::string class_name, std::string member_name );

	void	setBool( std::string class_name, std::string member_name, bool value );
	void	setInt( std::string class_name, std::string member_name, int value );
	void	setFloat( std::string class_name, std::string member_name, float value );
	void	setString( std::string class_name, std::string member_name, std::string value );

	void	dbgPrint();

private:

	class Class;
	class Member;
	typedef std::unordered_map<std::string, Class> class_map_t;

	Class* getClass( const std::string &class_name );

	class Member
	{
	public:
		Member();
		Member( const Member& member );
		Member( const std::string &name, const std::string &value );
		~Member();

		bool		isUndefined() const; //TODO: Implement
		std::string	getName() const;
		int32_t		getInteger() const;
		float		getFloat() const;
		std::string	getString() const;
		template <typename T> std::vector<T> getArray() const;

		void setString( const std::string &value );

	protected:
		friend class Class;

		std::string 	m_name;
		std::string		m_value;
	};

	class Class
	{
	private:

		std::string m_name;
		std::list<Class*> m_inheritance;

		std::unordered_map<std::string, Member> m_members;

	public:

		Member* getMember( const std::string &member );
		std::string getName() const;
		bool addMember( const std::string &member, const std::string &value );
		void addParent( const std::string &parent );

		void dbgPrint();

		Class();
		Class( const std::string &name );
		~Class();
	};

	class_map_t m_classes;

	Class* internalCreateClass( const std::string &name );

};

#endif // CONFIG_H
