#pragma once
#include <bits/stdc++.h>

#include "field.h"

using namespace std;


class Object{
private:
	set<Field> attribute;
public:
	Object();
	string operator [](const string&); // Type of return value is string(NOT string&!! ) !!
	void update(const string&,const string&); // If key exists, overwrite it. Or create it.
	void update(const Field&); // The same use as the function above.
};