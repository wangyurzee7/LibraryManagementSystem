#include <bits/stdc++.h>

using namespace std;

#include "user.h"

string User::typeName(){
	return "User";
}

vector<string> User::uniqueKey(){
	return {"username"};
}
vector<string> User::implicitKey(){
	return {"realname","school","remarks"};
}
vector<string> User::explicitKey(){
	return {};
}


User::User(const string& username){
	Object::update("username",username);
}
User::User(const string& username,const string& pwd):password(pwd){
	Object::update("username",username);
}
void User::enterPassword(const string& pwd){
	password=Password(pwd);
}