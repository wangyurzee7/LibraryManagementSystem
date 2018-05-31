#include <bits/stdc++.h>
using namespace std;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "database.h"

Database::Database(const string& _name):dbName(_name){
	client=mongocxx::uri{};
	db=client[_name];
	
	// userCollection=db["User"];
	// bookCollection=db["Book"];
	// recordCollection=db["Record"];
}

document toDocument(const Object &obj){
	document doc;
	for (auto key:obj.explicitKey()) doc<<key<<obj[key];
	for (auto key:obj.implicitKey()) doc<<key<<obj[key];
	return doc;
}
document toDocument(const CompleteMatchingSearch &obj){
	document doc;
	for (auto ele:obj.fields()) doc<<ele.key<<ele.value;
	return doc;
}
document toDocument(const ReSearch &obj){
	document doc;
	for (auto ele:obj.fields()) doc<<ele.key<<open_document<<"$regex"<<ele.value<<close_document;
	return doc;
}

document toDocumentForFind(const Object &obj){
	document doc;
	for (auto key:obj.uniqueKey()) doc<<key<<obj[key];
	return doc;
}


// check "Role" at the same time
bool Database::userExist(const User& user){
	auto collection=db["User"];
	document doc=toDocumentForFind(user);
	doc<<"Password"<<user.password.toString()<<"Status"<<"Accessible"<<"Role"<<user["Role"];
	auto cursor=collection.find(doc.view());
	return cursor.begin()!=cursor.end();
}

bool Database::isAdmin(const User& user){
	auto collection=db["User"];
	document doc=toDocumentForFind(user);
	doc<<"Password"<<user.password.toString()<<"Status"<<"Accessible";
	auto cursor=collection.find(doc.view());
	if (cursor.begin()!=cursor.end()){
		string role((*cursor.begin())["Role"].get_utf8().value);
		if (role=="Admin"||role=="Root") return 1;
	}
	else{
		return 0;
	}
}

bool Database::isRoot(const User& user){
	auto collection=db["User"];
	document doc=toDocumentForFind(user);
	doc<<"Password"<<user.password.toString()<<"Status"<<"Accessible";
	auto cursor=collection.find(doc.view());
	if (cursor.begin()!=cursor.end()){
		string role((*cursor.begin())["Role"].get_utf8().value);
		if (role=="Root") return 1;
	}
	else{
		return 0;
	}
}

/*
bool Database::findOne(Object& obj); // Find according to uniqueKey
bool Database::objectExist(const Object& obj);

string Database::newRecordId();
ErrorCode Database::add(const Object& object);
ErrorCode Database::update(const Object& object);
ErrorCode Database::modifyPassword(const User& user,const Password& newPwd);
ErrorCode Database::remove(const Object& object);
template<typename ObjType>
ErrorCode Database::search(const Search& key,vector<ObjType> &ret);

*/