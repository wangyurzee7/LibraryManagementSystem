#pragma once
#include <bits/stdc++.h>

using namespace std;

class Field{
public:
	string key;
	string value;
	Field(const string&);
	Field(const string&,const string&);
	Field(const pair<string,string>&);
	bool operator ==(const Field &ele) const;
	bool operator <(const Field &ele) const;
	bool operator >(const Field &ele) const;
	operator pair<string,string>();
};
