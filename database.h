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


class Database{
private:
	// mongocxx::instance inst;
	mongocxx::client client;
	mongocxx::database db;
	
	bsoncxx::builder::stream::document toDocument(const Object &obj);
	bsoncxx::builder::stream::document toDocument(const CompleteMatchingSearch &obj);
	bsoncxx::builder::stream::document toDocument(const FuzzyMatchingSearch &obj);
	bsoncxx::builder::stream::document toDocument(const ReSearch &obj);
	
	bsoncxx::builder::stream::document toDocumentForFind(const Object &obj);
public:
	Database();
	Database(const string&);
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
};

