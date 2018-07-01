#include <bits/stdc++.h>

using namespace std;

#include "practicalbook.h"

string PracticalBook::typeName() const{
	return "PracticalBook";
}

vector<string> PracticalBook::uniqueKey() const{
	return {"No","Index"};
}
vector<string> PracticalBook::implicitKey() const{
	return {};
}
vector<string> PracticalBook::explicitKey() const{
	return {"No","Index","Status","Remarks"};
}


PracticalBook::PracticalBook(){}
PracticalBook::PracticalBook(const string& no){
	Object::update("No",no);
}
PracticalBook::PracticalBook(const string& no,const string& index){
	Object::update("No",no);
	Object::update("Index",index);
}