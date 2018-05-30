#pragma once
#include <bits/stdc++.h>

#include "field.h"

using namespace std;


class Object{
protected:
	set<Field> attribute;
public:
	Object();
	string operator [](const string&) const; // Type of return value is string(NOT string&!! ) !!
	void update(const string&,const string&); // If key exists, overwrite it. Or create it.
	void update(const Field&); // The same use as the function above.
	virtual string typeName() const=0;
	virtual vector<string> uniqueKey() const=0;
	virtual vector<string> implicitKey() const=0;
	virtual vector<string> explicitKey() const=0;
};