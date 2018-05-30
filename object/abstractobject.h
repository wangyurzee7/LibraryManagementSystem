#pragma once
#include <bits/stdc++.h>

#include "field.h"

using namespace std;


#include <bsoncxx/builder/stream/document.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


template<typename Container>
class AbstractObject{
protected:
	Container attribute;
public:
	AbstractObject();
	string operator [](const string&) const; // Type of return value is string(NOT string&!! ) !!
	void update(const string&,const string&); // If key exists, overwrite it. Or create it.
	void update(const Field&); // The same use as the function above.
	virtual string typeName() const=0;
	virtual Container invalidFields() const=0;
	virtual bsoncxx::builder::stream::document toDocument() const; // This function is for database
};