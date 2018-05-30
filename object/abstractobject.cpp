#include <bits/stdc++.h>

#include "field.h"

using namespace std;

#include "abstractobject.h"


AbstractObject::AbstractObject(){
	attribute.clear();
}

string AbstractObject::operator [](const string& key) const{
	auto it=attribute.find(Field(key));
	return it!=attribute.end()?it->value:"";
}

// If key exists, overwrite it. Or create it.
void AbstractObject::update(const Field& field){
	auto it=attribute.find(field);
	if (it!=attribute.end()){
		attribute.erase(it); // If key exists,remove the field firstly.
	}
	attribute.insert(field); // Create it.
}

// The same use as the function above.
void AbstractObject::update(const string& key,const string& value){
	this->update(Field(key,value));
}