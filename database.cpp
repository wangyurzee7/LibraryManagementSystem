#include <bits/stdc++.h>
using namespace std;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "database.h"

Database::Database(){
	client=mongocxx::uri{};
	db=client["LMS"];
	// userCollection=db["user"];
	// bookCollection=db["book"];
	// recordCollection=db["record"];
}

Database::Database(const string &dbName){
	client=mongocxx::uri{};
	db=client[dbName];
	// userCollection=db["user"];
	// bookCollection=db["book"];
	// recordCollection=db["record"];
}

