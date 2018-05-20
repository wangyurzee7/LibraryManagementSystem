#include <bits/stdc++.h>
using namespace std;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "database.h"

Database::Database(const string &dbName="LMS"){
	client=mongocxx::uri{};
	db=client[dbName];
	userCollection=db["user"];
	bookCollection=db["book"];
	recordCollection=db["record"];
}

