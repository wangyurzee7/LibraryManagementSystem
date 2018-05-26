#include <bits/stdc++.h>

#include "field.h"

using namespace std;

#include "object.h"

/*
(const string& _typeName,const vector<string>& _uniqueKey,const vector<string>& _implicitKey,const vector<string>& _explicitKey):typeName(_typeName),uniqueKey(_uniqueKey),implicitKey(_implicitKey),explicitKey(_explicitKey)
*/

Object::Object(){
	attribute.clear();
}

string Object::operator [](const string& key){
	auto it=attribute.find(Field(key));
	return it!=attribute.end()?it->value:"";
}

// If key exists, overwrite it. Or create it.
void Object::update(const Field& field){
	auto it=attribute.find(field);
	if (it!=attribute.end()){
		attribute.erase(it); // If key exists,remove the field firstly.
	}
	attribute.insert(field); // Create it.
}

// The same use as the function above.
void Object::update(const string& key,const string& value){
	this->update(Field(key,value));
}