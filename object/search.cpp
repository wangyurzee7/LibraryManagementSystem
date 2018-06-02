#include <bits/stdc++.h>

using namespace std;

#include "search.h"

string Search::typeName() const{
	return "Search";
}

const int Search::maxSize=100;
const int Search::maxLength=100;

FormatInfo Search::invalidFields() const{
	if (int(attribute.size())>maxSize) return FormatInfo({Field("Too Many Arguments","Yes")});
	FormatInfo ret;
	for (auto ele:attribute){
		if (ele.value.length()>maxLength) ret.update(ele.key,"Too Long");
	}
	return ret;
}

multiset<Field> Search::fields() const{
	return attribute;
}

Search::Search(){}