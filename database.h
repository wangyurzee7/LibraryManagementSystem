#pragma once

#include <bits/stdc++.h>
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
	mongocxx::collection userCollection;
	mongocxx::collection bookCollection;
	mongocxx::collection recordCollection;
public:
	Database();
	Database(const string&);
};

