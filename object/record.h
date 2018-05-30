#pragma once
#include <bits/stdc++.h>

#include "object.h"

using namespace std;


class Record:public Object{
public:
	using Object::Object;
	Record(const string&);
	
	virtual string typeName() const override;
	virtual vector<string> uniqueKey() const override;
	virtual vector<string> implicitKey() const override;
	virtual vector<string> explicitKey() const override;
	
	
	
};