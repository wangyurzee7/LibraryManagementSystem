#include<bits/stdc++.h>
#include "controller.h"
using namespace std;

class Client
{
	LoginController *loginController;
	ReaderController *controller;
	Server *server;
	string login();
	
	void showBook(int number);
	void showRecord(int number);
	void showUser(int number);
	
	void outputInfo();//输出消息
	
	void searchBook();
	void higherSearchBook();//高级搜索
	void afterSearchBook();//看完书之后的
	
	
	void browseBook(int number);
	void editBook(const Book &book);
	void borrowBook(int number);
	
	void listBorrowingBooks();//借着的practicalBook
	void returnBook(int number);
	

	void modifyPassword();
	
	
	void searchUser();
	void afterSearchUser();
	
	void showPending();
	void deal(int number,bool accept);
	
	void freezeUser (int number);
	void freezeBook(int number);
	void listFreezeBook();
	void listFreezeUser();
	void unfreezeBook(int number);
	void unfreezeUser(int number);
	
	void readUserRecord(int number);
	void readBookRecord(int number);
	void readSelfRecord();
	void switchRecord();
	
	void addNewBook();
	void addBookFromExist(int number);
	
	void registerUser();
	
//root
	void removeBook (int number);
	void removeUser (int number);
	public:
		void main();
};
