/************************************************* 
Copyright:bupt 
Author: Eric Sam
Date:2018-06-11 
Description:serve between server and client 
**************************************************/  
#include <bits/stdc++.h>

#include "../server.h"

using namespace std;

class AbstractController{
	protected:
		Server *server;//which server to use
		User *user;//
		bool finalQuit;
	public:
		AbstractController(Server *_server):server(_server),finalQuit(1){}
		~AbstractController();
		User* getUser();
};

class LoginController:public AbstractController
{
	public:
		LoginController(Server *_server):AbstractController(_server){}
		vector<string> login(const string &userName,const string &passWord);
	//Server* getServer();
	//User* getuser();
};
//放到后面的AdminRegister里面

class ReaderController:public AbstractController
{//找书
	protected:
		vector<Book> books;//检索出来的书放在这里
		vector<PracticalBook> practicalBooks;//检索出来的书目放在这里
		vector<Record> records;
		void infoBook();
		void infoPracticalBook();
		void infoRecord();
	public:
		vector<string> commands;//不确定行数的命令放这里(即时)
		vector<string> info;//搜索出的object的简要信息存放在这里
		vector<string> deepInfo;//一个Obj的繁琐信息存放在这里
		ReaderController(Server *_server,User *_user):AbstractController(_server),use(_user){}
		~ReaderController(){}
		
		Book getBook(int number);
		string searchBook();
		string browseBook(const Book &book);
		
		PracticalBook getPracticalBook(int number);
		
		void bookToPractical(const Book &book);
		string borrowBook(const Book &book);
		
		Record getRecord(int number);
		
		template<class ObjType>
			void show(const ObjType &objectVector);
			
		string listBorrowingBooks(User *_user);
		string returnBook(const PracticalBook &book);
		
		string modifyPassword(string password1,string password2);
		string readRecord(User *_user);
		
};

class AdminController:public ReaderController
{
	protected:
		vector<User> users;
		void infoUser();
	public:
		AdminController(Server *_server,User *_user):ReaderController(_server,_user){}
		User getUser(int number);
		string findUser();//找读者(普通用户/高级用户)
		string registerUser();//注册一个账号(reader或者administrator)
		string addBook(const Book &book);//输入一本书的信息
		string addNewBook();
		string showPendingBook();//展示需要处理的书
		string deal(const Record &record,bool accept);
		string editBook(const Book &book);
		
		template<class ObjType>
			string freeze(const ObjType &obj);
		
		string showFreezeBook();
		string showFreezeUser();
		
		string readBookRecord(const PracticalBook &practicalBook);
		
		template<class ObjType>
			string unfreeze(ObjType &obj);
};

class RootController:public AdminController
{
	public:
		RootController(Server *_server,User *_user):AdminController(_server,_user){}
		template<class ObjType>
			string remove(const ObjType &obj);
		//template<class ObjType>
			//string modify(const Obj);
};