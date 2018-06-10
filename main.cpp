#include <bits/stdc++.h>

using namespace std;

#include "database.h"
#include "server.h"
#include "client.h"

namespace LMS{

void __main__(){
	Database db=new Database("TestLMS");
	Server server=new Server(db);
	Client client=new Client(server);

	client.main();

	delete client;
	delete server;
	delete db;
}

};

int main(){
	LMS::__main__();
	return 0;
}
