#include <bits/stdc++.h>

using namespace std;

#include "book.h"

string Book::typeName() const{
	return "Book";
}

vector<string> Book::uniqueKey() const{
	return {"No"};
}
vector<string> Book::implicitKey() const{
	return {"Url","Status"};
}
vector<string> Book::explicitKey() const{
	return {"No","Name","Author","ISBN","Publisher","Remarks"};
}


Book::Book(){}
Book::Book(const string& no){
	Object::update("No",no);
}