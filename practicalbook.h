#pragma once
#include <bits/stdc++.h>

#include "object.h"
#include "password.h"

using namespace std;


class PracticalBook:public Object{
public:
	using Object::Object;
	PracticalBook(const string&);
	PracticalBook(const string&,const string&);
	
	virtual string typeName() const override;
	virtual vector<string> uniqueKey() const override;
	virtual vector<string> implicitKey() const override;
	virtual vector<string> explicitKey() const override;
	
	
	
};