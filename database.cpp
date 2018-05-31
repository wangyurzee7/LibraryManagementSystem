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
	
	if (!objectExist(User({Field("Role","Root")})))
		add(User("root","root"));
}

document toDocument(const Object &obj){
	document doc;
	for (int k=0;k<2;++k)for (auto key:(k?obj.explicitKey():obj.implicitKey())){
		if (obj[key]!="")
			doc<<key<<obj[key];
	}
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
	auto info=collection.find_one(doc.view());
	return bool(info);
}

bool Database::isAdmin(const User& user){
	auto collection=db["User"];
	document doc=toDocumentForFind(user);
	doc<<"Password"<<user.password.toString()<<"Status"<<"Accessible";
	auto info=collection.find_one(doc.view());
	if (info){
		string role(info->view()["Role"].get_utf8().value);
		if (role=="Admin"||role=="Root") return 1;
	}
	return 0;
}

bool Database::isRoot(const User& user){
	auto collection=db["User"];
	document doc=toDocumentForFind(user);
	doc<<"Password"<<user.password.toString()<<"Status"<<"Accessible";
	auto info=collection.find_one(doc.view());
	if (info){
		string role(info->view()["Role"].get_utf8().value);
		if (role=="Root") return 1;
	}
	return 0;
}

// Find according to uniqueKey
bool Database::findOne(Object& obj){
	auto collection=db[obj.typeName()];
	document doc=toDocumentForFind(obj);
	auto info=collection.find_one(doc.view());
	if (info&&info->view()["Status"].get_utf8().value!="Frozen"){
		for (int k=0;k<2;++k)for (auto key:(k?obj.explicitKey():obj.implicitKey())){
			obj.update(key,string(info->view()[key].get_utf8().value));
		}
		return 1;
	}
	return 0;
}

bool Database::objectExist(const Object& obj){
	auto collection=db[obj.typeName()];
	document doc=toDocument(obj);
	auto info=collection.find_one(doc.view());
	if (info&&info->view()["Status"].get_utf8().value!="Frozen"){
		return 1;
	}
	return 0;
}

string Database::newRecordId(){
	document doc;
	string ret;
	auto info=db["LastRecordId"].find_one({});
	if (info){
		string _last=string(info->view()["LastRecordId"].get_utf8().value);
		ret=to_string(stoi(_last)+1);
		doc<<"LastRecordId"<<ret;
		db["LastRecordId"].update_one({},doc.view());
	}
	else{
		ret="1";
		doc<<"LastRecordId"<<ret;
		db["LastRecordId"].insert_one(doc.view());
	}
	return ret;
}

ErrorCode Database::modifyPassword(const User& user,const Password& newPwd){
	auto collection=db["User"];
	document doc=toDocumentForFind(user);
	doc<<"Status"<<"Accessible";
	auto info=collection.find_one(doc.view());
	if (info){
		if (string(info->view()["Password"].get_utf8().value)!=user.password.toString())
			return wrongPassword;
		document newDoc;
		newDoc<<"$set"<<open_document<<"Password"<<newPwd.toString()<<close_document;
		collection.update_one(doc.view(),newDoc.view());
		return noError;
	}
	else return userNotFound;
}


ErrorCode Database::add(const Object& object){
	
}
ErrorCode Database::update(const Object& object){
	
}
ErrorCode Database::remove(const Object& object){
	
}

template<typename ObjType>
ErrorCode Database::search(const Search& key,vector<ObjType> &ret){
	
}
