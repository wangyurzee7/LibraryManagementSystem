#include <bits/stdc++.h>

using namespace std;

#include "record.h"

string Record::typeName(){
	return "Record";
}

vector<string> Record::uniqueKey(){
	return {"Id"};
}
vector<string> Record::implicitKey(){
	return {"Type"}; // This field has 4 possible values: "Borrowing", "Returning", "Browsing", "Previewing"
}
vector<string> Record::explicitKey(){
	return {"Id","Username","BookNo","BookIndex","Remarks","Time"};
}


Record::Record(const string& id){
	Object::update("Id",id);
}