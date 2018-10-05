#include "StringTable.h"

#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <iostream>

using namespace std;


StringTable::StringTable( const string &filename )
{
	this->load( filename );
}

StringTable::~StringTable()
{
	//dtor
}

string StringTable::getString( const string &handle ) const
{
	stringmap_t::const_iterator it = m_strings.find( handle );

	if ( it == m_strings.end() )
	{
		return "";
	}

	return it->second;
}

void StringTable::setString( const string &handle, const string &entry )
{
	stringmap_t::iterator it = this->m_strings.find(handle);

	if ( it == this->m_strings.end() )
	{
		this->m_strings.insert( make_pair(handle, entry) );
	}
	else
	{
		it->second = entry;
	}
}


void StringTable::load( const string &filename )
{
	// TODO: Use libxml or support it alternatively
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(filename.c_str());

	if ( doc == nullptr )
	{
		cerr << "\nStringTable Error\nDocument not parsed successfully\n" << endl;
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if ( cur == nullptr )
	{
		cerr << "\nStringTable Error\nEmpty XML document\n" << endl;
		xmlFreeDoc(doc);
		return;
	}

	if ( xmlStrcmp(cur->name, (const xmlChar *) "stringtable") )
	{
		cerr << "\nStringTable Error\nDocument of the wrong type\n" << endl;
		xmlFreeDoc(doc);
		return;
	}

	cur = cur->xmlChildrenNode;
    while ( cur != nullptr )
    {
        if ( !xmlStrcmp(cur->name, (const xmlChar *)"string") )
        {
			char* name = (char*)xmlGetProp(cur, (const xmlChar *)"name");
            char* key = (char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

			this->setString(name, key);

			free(name);
			free(key);

			//cur = cur->xmlChildrenNode;
        }
        cur = cur->next;
    }

	xmlFreeDoc(doc);
}
