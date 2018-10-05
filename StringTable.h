#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include <map>
#include <string>


class StringTable
{
public:
	StringTable( const std::string &filename );
	StringTable( const StringTable &string_table);
	~StringTable();

	// TODO: const StringTable & operator = (const StringTable &rhs);
	// TODO: const StringTable & operator + (const StringTable &rhs);

	void load( const std::string &filename );

	std::string getString( const std::string &handle ) const;
	void setString( const std::string &handle, const std::string &entry );


protected:

private:
	typedef std::map<std::string, std::string> stringmap_t;
	stringmap_t m_strings;
};

#endif // STRINGTABLE_H
