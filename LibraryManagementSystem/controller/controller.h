/************************************************* 
Copyright:bupt 
Author: Eric Sam
Date:2018-06-11 
Description:serve between server and client 
tips: every information return will in the form of string
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
		User* getSelf();
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
		vector<Record> records;//书的一些记录
		void infoBook();//书的内容填进vector
		void infoPracticalBook();//把书的内容填进信息
		void infoRecord();//把记录摘要填进信息
	public:
		vector<string> commands;//不确定行数的命令放这里(即时),由client负责输入,controller负责清理
		virtual string type();//标志着类别的"type"设计模式
		vector<string> info;//搜索出的object的简要信息存放在这里,controller负责在每次需要用的之前清理
		vector<string> deepInfo;//一个Obj的繁琐信息存放在这里,在每次需要用之前清理
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
			void show(const ObjType &object);
			
		string listBorrowingBooks(User *_user);
		string returnBook(const PracticalBook &book);
		
		string modifyPassword(string password1,string password2);
		string readRecord(User *_user);
		
//========the followings are all virtual functions,do not care=============
		virtual User getUser(int number){};
		virtual string findUser(const string &username){}//找读者(普通用户/高级用户)
		virtual string registerUser(){}//注册一个账号(reader或者administrator)
		virtual string addNewBook(){}//增加一本新的书
		virtual string showPendingBook(){}//展示需要处理的书
		virtual string deal(const Record &record,bool accept){}
		virtual string editBook(const Book &book){}//
		
		virtual string freezeBook(const PracticalBook &practicalBook){}
		virtual string freezeUser(const User &user){}
		
		virtual string showFreezeBook(const PracticalBook &practicalBook){}
		virtual string showFreezeUser(const User &user){}
		
		virtual string readBookRecord(const PracticalBook &practicalBook){}
		
		virtual string unfreezeBook(const PracticalBook &practicalBook){}
		virtual string unfreezeUser(const User &user){}
		virtual string removeUser(const User &user){};
		virtual string removePracticalBook(const PracticalBook &practicalBook){};
};

class AdminController:public ReaderController
{
	protected:
		vector<User> users;
		void infoUser();
		template<class ObjType>
			string freeze(const ObjType &obj);//freeze的骨架
		template<class ObjType>
			string unfreeze(ObjType &obj);//unfreeze的骨架
		 string addBook(const Book &book);//输入一本书的信息
	public:
		AdminController(Server *_server,User *_user):ReaderController(_server,_user){}
		virtual string type() override;
		virtual User getUser(int number) override;
		virtual string findUser(const string &username)override;//找读者(普通用户/高级用户)
		virtual string registerUser()override;//注册一个账号(reader或者administrator)
		
		virtual string addNewBook()override;//增加一本新的书
		virtual string showPendingBook()override;//展示需要处理的书
		virtual string deal(const Record &record,bool accept)override;//Santa Lucia!
		virtual string editBook(const Book &book)override;//
		
		virtual string showFreezeBook()override;
		virtual string showFreezeUser()override;
		
		virtual string readBookRecord(const PracticalBook &practicalBook)override;
		
		virtual string freezeBook(const PracticalBook &practicalBook)override{return freeze(practicalBook);}
		virtual string freezeUser(const User &user)override{return freeze(user);}
		
		virtual string unfreezeBook(const PracticalBook &practicalBook)override{return unfreeze(practicalBook);}
		virtual string unfreezeUser(const User &user)override{return unfreeze(user);}
};

class RootController:public AdminController
{
	template<class ObjType>
		string remove(const ObjType &obj);//remove的骨架
	public:
		RootController(Server *_server,User *_user):AdminController(_server,_user){}
		virtual string type() override;
		virtual string removeUser(const User &user)override;
		virtual string removePracticalBook(const PracticalBook &practicalBook)override;
		//template<class ObjType>
			//string modify(const Obj);
};
