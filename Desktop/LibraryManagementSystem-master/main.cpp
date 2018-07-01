#include <bits/stdc++.h>

using namespace std;

#define TEST
#include "database.h"
#include "server.h"
#include "client.h"
#include "object/user.h"

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
		//cout<<"Fuck!"<<endl;
		client->main();
	}
	~LibraryManagementSystem(){
		delete client;
		delete server;
		delete db;
	}
};

};

int main()
{	
	
    mongocxx::instance inst{};
	LMS::LibraryManagementSystem().__main__();
	return 0;
}

