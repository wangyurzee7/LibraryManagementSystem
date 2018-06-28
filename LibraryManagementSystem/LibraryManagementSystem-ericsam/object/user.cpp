#include <bits/stdc++.h>

using namespace std;

#include "user.h"

string User::typeName() const{
	return "User";
}

vector<string> User::uniqueKey() const{
	return {"Username"};
}
vector<string> User::implicitKey() const{
	return {"Status"};
}
vector<string> User::explicitKey() const{
	return {"Username","Role","Realname","School","Remarks"};
}


User::User(){}
User::User(const string& username){
	Object::update("Username",username);
}
User::User(const string& username,const string& pwd):password(pwd){
	Object::update("Username",username);
}
void User::enterPassword(const string& pwd){
	password=Password(pwd);
}