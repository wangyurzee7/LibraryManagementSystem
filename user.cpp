#include <bits/stdc++.h>

using namespace std;

#include "user.h"

string User::typeName(){
	return "User";
}

vector<string> User::uniqueKey(){
	return {"Username"};
}
vector<string> User::implicitKey(){
	return {};
}
vector<string> User::explicitKey(){
	return {"Username","Role","Realname","School","Remarks"};
}


User::User(const string& username){
	Object::update("Username",username);
}
User::User(const string& username,const string& pwd):password(pwd){
	Object::update("Username",username);
}
void User::enterPassword(const string& pwd){
	password=Password(pwd);
}