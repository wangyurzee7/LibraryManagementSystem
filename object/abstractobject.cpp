#include <bits/stdc++.h>

#include "field.h"

using namespace std;

#include "abstractobject.h"
/*
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
*/

string FormatInfo::typeName() const{
	return "FormatInfo";
}

bool FormatInfo::empty() const{
	return int(attribute.size())==0;
}