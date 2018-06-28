#pragma once
#include <bits/stdc++.h>

#include "object.h"
#include "password/password.h"

using namespace std;

class User:public Object{
public:
	Password password;

	using Object::Object;
	User();
	User(const string&);
	User(const string&,const string&);
	void enterPassword(const string&);
	
	virtual string typeName() const override;
	virtual vector<string> uniqueKey() const override;
	virtual vector<string> implicitKey() const override;
	virtual vector<string> explicitKey() const override;
	
	
	
};