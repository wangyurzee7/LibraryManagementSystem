#pragma once

#include <bits/stdc++.h>


#include "object/object.h"
#include "object/user.h"
#include "object/book.h"
#include "object/practicalbook.h"
#include "object/record.h"

#include "object/search.h"

#include "errorcode.h"
// #include "searchstrategy.h"


using namespace std;

// #include <bsoncxx/builder/stream/document.hpp>
// #include <bsoncxx/json.hpp>
// #include <mongocxx/instance.hpp>
// #include <mongocxx/client.hpp>
// #include <mongocxx/stdx.hpp>
// #include <mongocxx/pool.hpp>
// #include <mongocxx/uri.hpp>
// #include <mongocxx/cursor.hpp>
// #include <bsoncxx/types.hpp>
// #include <bsoncxx/builder/stream/array.hpp>
// #include <bsoncxx/builder/stream/helpers.hpp>
// #include <bsoncxx/builder/concatenate.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::document;


class Database{
private:
	// mongocxx::instance inst;
	string dbName;
	mongocxx::client client;
	mongocxx::database db;
	
	document toDocument(const Object &obj);
	document toDocument(const Search &obj);
	// document toDocument(const CompleteMatchingSearch &obj);
	// bsoncxx::builder::stream::document toDocument(const FuzzyMatchingSearch &obj);
	// document toDocument(const ReSearch &obj);
	
	document toCompleteDocument(const Object &obj);
	document toCompleteDocument(const User &obj);
	document toDocumentForFind(const Object &obj);
public:
	#ifdef TEST
	void test(){cout<<"Databse address = "<<this<<endl;cout<<dbName<<endl;}
	#endif
	Database(const string& _name="LMS");
	bool userExist(const User& user); // check "Role" at the same time
	bool isAdmin(const User& user);
	bool isRoot(const User& user);
	bool findOne(Object& obj); // Find according to uniqueKey
	bool objectExist(const Object& obj);
	// Functions above ensures that objects are not Frozen.
	
	string newRecordId();
	ErrorCode modifyPassword(const User& user,const Password& newPwd);
	
	template<typename ObjType>
	ErrorCode add(const ObjType& obj);
	template<typename ObjType>
	ErrorCode update(const ObjType& obj);
	template<typename ObjType>
	ErrorCode remove(const ObjType& obj);
	template<typename ObjType>
	ErrorCode search(const Search& key,vector<ObjType> &ret);
};


template<typename ObjType>
ErrorCode Database::add(const ObjType &obj){
	auto collection=db[obj.typeName()];
	if (collection.find_one(toDocumentForFind(obj).view())) return objectExists;
	
	document doc=toCompleteDocument(obj);
	
	collection.insert_one(doc.view());
	return noError;
}
template<typename ObjType>
ErrorCode Database::update(const ObjType& obj){
	auto collection=db[obj.typeName()];
	document doc=toDocumentForFind(obj);
	if (!collection.find_one(doc.view())) return objectNotFound;
	
	document newDoc;
	newDoc<<"$set"<<toDocument(obj);
	
	collection.update_one(doc.view(),newDoc.view());
	return noError;
}
template<typename ObjType>
ErrorCode Database::remove(const ObjType& obj){
	auto collection=db[obj.typeName()];
	document doc=toDocumentForFind(obj);
	if (!collection.find_one(doc.view())) return objectNotFound;
	
	collection.remove(doc.view());
	return noError;
}


template<typename ObjType>
ErrorCode Database::search(const Search& key,vector<ObjType> &ret){
	auto collection=db[ObjType().typeName()];
	document doc=toDocument(key);
	
	auto cursor=collection.find(doc.view());
	
	ret.clear();
	for (auto&& element:cursor){
		ObjType obj;
		for (int k=0;k<2;++k)for (auto key:(k?obj.explicitKey():obj.implicitKey())){
			obj.update(string(key),string(element[key].get_utf8().value));
		}
		ret.push_back(obj);
	}
	
	return noError;
}
