#include <bits/stdc++.h>

using namespace std;

#define TEST
#include "database.h"
#include "server.h"
#include "client.h"

namespace LMS{

class LibraryManagementSystem{ // Factory mode.
private:
	Database* db;
	Server* server;
	Client* client;
public:
	LibraryManagementSystem(){
		db=new Database("TestLMS");
		server=new Server(db);
		client=new Client(server);
	}
	void __main__(){
		client->main();
	}
	~LibraryManagementSystem(){
		delete client;
		delete server;
		delete db;
	}
};

};

int main(){
	LMS::LibraryManagementSystem().__main__();
	return 0;
}
