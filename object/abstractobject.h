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

template<class Container>
AbstractObject<Container>::AbstractObject():attribute(){}

template<class Container>
AbstractObject<Container>::AbstractObject(const Container& _attribute):attribute(_attribute){}

template<class Container>
string AbstractObject<Container>::operator [](const string& key) const{
	auto it=attribute.find(Field(key));
	return it!=attribute.end()?it->value:"";
}

// If key exists, overwrite it. Or create it.
template<class Container>
void AbstractObject<Container>::update(const Field& field){
	auto it=attribute.find(field);
	if (it!=attribute.end()){
		attribute.erase(it); // If key exists,remove the field firstly.
	}
	attribute.insert(field); // Create it.
}

// The same use as the function above.
template<class Container>
void AbstractObject<Container>::update(const string& key,const string& value){
	this->update(Field(key,value));
}

class FormatInfo:public AbstractObject<set<Field>>{
public:
	using AbstractObject::AbstractObject;
	virtual string typeName() const override;
	virtual bool empty() const;
};