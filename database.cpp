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
	
	// userCollection=db["user"];
	// bookCollection=db["book"];
	// recordCollection=db["record"];
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

/*

bool userExist(const User& user); // check "Role" at the same time
bool isAdmin(const User& user);
bool isRoot(const User& user);
bool findOne(Object& obj); // Find according to uniqueKey
bool objectExist(const Object& obj);

string newRecordId();
ErrorCode add(const Object& object);
ErrorCode update(const Object& object);
ErrorCode remove(const Object& object);
template<typename ObjType>
ErrorCode search(const Search& key,vector<ObjType> &ret);

*/