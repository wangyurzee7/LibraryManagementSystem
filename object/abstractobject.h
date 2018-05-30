#pragma once
#include <bits/stdc++.h>

#include "field.h"

using namespace std;



template<typename Container>
class AbstractObject{
protected:
	Container attribute;
public:
	AbstractObject();
	AbstractObject(const Container&);
	string operator [](const string&) const; // Type of return value is string(NOT string&!! ) !!
	void update(const string&,const string&); // If key exists, overwrite it. Or create it.
	void update(const Field&); // The same use as the function above.
	virtual string typeName() const=0;
};

class FormatInfo:public AbstractObject<set<Field>>{
public:
	using AbstractObject::AbstractObject;
	virtual string typeName() const override;
	virtual bool empty() const;
};