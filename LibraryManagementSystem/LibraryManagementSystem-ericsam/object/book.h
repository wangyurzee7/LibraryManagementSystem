#pragma once
#include <bits/stdc++.h>

#include "object.h"

using namespace std;


class Book:public Object{
public:
	using Object::Object;
	Book();
	Book(const string&);
	
	virtual string typeName() const override;
	virtual vector<string> uniqueKey() const override;
	virtual vector<string> implicitKey() const override;
	virtual vector<string> explicitKey() const override;
	
	
	
};