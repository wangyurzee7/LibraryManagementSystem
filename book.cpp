#include <bits/stdc++.h>

using namespace std;

#include "book.h"

string Book::typeName(){
	return "Book";
}

vector<string> Book::uniqueKey(){
	return {"No"};
}
vector<string> Book::implicitKey(){
	return {"Url","Status"};
}
vector<string> Book::explicitKey(){
	return {"No","Name","Author","ISBN","Publisher","Remarks"};
}


Book::Book(const string& no){
	Object::update("No",no);
}