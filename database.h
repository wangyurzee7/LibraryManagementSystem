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

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

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
	document toDocumentForFind(const Object &obj);
public:
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

