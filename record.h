#pragma once
#include <bits/stdc++.h>

#include "object.h"
#include "password.h"

using namespace std;


class Record:public Object{
public:
	using Object::Object;
	Record(const string&);
	
	virtual string typeName() override;
	virtual vector<string> uniqueKey() override;
	virtual vector<string> implicitKey() override;
	virtual vector<string> explicitKey() override;
	
	
	
};