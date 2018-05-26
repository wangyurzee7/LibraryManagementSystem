#include <bits/stdc++.h>

using namespace std;

#include "practicalbook.h"

string PracticalBook::typeName(){
	return "PracticalBook";
}

vector<string> PracticalBook::uniqueKey(){
	return {"No","Index"};
}
vector<string> PracticalBook::implicitKey(){
	return {};
}
vector<string> PracticalBook::explicitKey(){
	return {"No","Index","Status","Remarks"};
}


PracticalBook::PracticalBook(const string& no){
	Object::update("No",no);
}
PracticalBook::PracticalBook(const string& no,const string& index){
	Object::update("No",no);
	Object::update("Index",index);
}