#pragma once

#include <bits/stdc++.h>


#include "object/object.h"
#include "object/user.h"
#include "object/book.h"
#include "object/practicalbook.h"
#include "object/record.h"

#include "search/search.h"
#include "search/completematching.h"
#include "search/fuzzymatching.h"
#include "search/re.h"

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
public:
	Database();
	Database(const string&);
	bool userExist(const User& user);
	bool isAdmin(const User& user);
	bool isRoot(const User& user);
	ErrorCode add(const Object& object);
	ErrorCode update(const Object& object);
	ErrorCode remove(const Object& object);
	template<typename ObjType>
	ErrorCode search(const Search& key,vector<ObjType> &ret);
};

