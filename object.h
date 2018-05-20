#pragma once
#include <bits/stdc++.h>

#include "field.h"

using namespace std;


class Object{
private:
	set<Field> attribute;
public:
	Object();
	string operator [](const string&);
	void update(const string&,const string&);
	void update(const Field&);
};