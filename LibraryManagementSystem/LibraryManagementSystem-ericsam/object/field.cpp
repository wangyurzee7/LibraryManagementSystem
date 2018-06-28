#include <bits/stdc++.h>

using namespace std;
#include "field.h"


Field::Field():key(""),value(""){}
Field::Field(const string &_key):key(_key),value(""){}
Field::Field(const string &_key,const string &_value):key(_key),value(_value){}
Field::Field(const pair<string,string> &_f):key(_f.first),value(_f.second){}
bool Field::operator ==(const Field &ele) const{
	return this->key==ele.key;
}
bool Field::operator <(const Field &ele) const{
	return this->key<ele.key;
}
bool Field::operator >(const Field &ele) const{
	return this->key>ele.key;
}
Field::operator pair<string,string>(){
	return make_pair(this->key,this->value);
}