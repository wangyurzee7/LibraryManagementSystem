#pragma once
#include <bits/stdc++.h>

using namespace std;

class Field{
public:
	string key;
	string value;
	Field();
	Field(const string&);
	Field(const string&,const string&);
	Field(const pair<string,string>&);
	bool operator ==(const Field &ele) const; // Two Fields are equal iff their keys are equal.
	bool operator <(const Field &ele) const; // The same as above.
	bool operator >(const Field &ele) const; // The same as above.
	operator pair<string,string>();
};
