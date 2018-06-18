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
		User user;//
	public:
		AbstractController(Server *_server):server(_server){}
		AbstractController(Server *_server,User _user):server(_server),user(_user){}
		~AbstractController();
		User getSelf();//首先看生命周期(该不该引用),然后看是否能够修饰const
};

class LoginController:public AbstractController
{
	public:
		LoginController(Server *_server):AbstractController(_server){}
		vector<string> login(const string &userName,const string &password);
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
		
		vector<string> info;//搜索出的object的简要信息存放在这里,controller负责在每次需要用的之前清理
		vector<string> deepInfo;//一个Obj的繁琐信息存放在这里,在每次需要用之前清理
		ReaderController(Server *_server,LoginController &loginController):AbstractController(_server,loginController.getSelf()){}
		~ReaderController(){}
		
		virtual string type();//标志着类别的"type"设计模式
		Book getBook(int number);
		string searchBook();
		string browseBook(Book &book);
		
		PracticalBook getPracticalBook(int number);
		
		void bookToPractical(const Book &book);
		string borrowBook(const Book &book);
		
		Record getRecord(int number);
		
		template<class ObjType>
			void show(const ObjType &object);
			
		string listBorrowingBooks(const User &_user);
		string returnBook(PracticalBook book);
		
		string modifyPassword(string password1,string password2);
		string readRecord(User _user);
		
//========the followings are all virtual functions,do not care=============
		virtual User getUser(int number){};
		virtual string findUser(const string &username){}//找读者(普通用户/高级用户)
		virtual string registerUser(const string &username,const string &password,string identity){}//注册一个账号(reader或者administrator)
		virtual string addBook(Book &book);
		virtual string addNewBook(){}//增加一本新的书
		virtual string showPendingBook(){}//展示需要处理的书
		virtual string deal(Record record,bool accept){}
		virtual string editBook(Book book){}//
		
		virtual string freezeBook(PracticalBook &practicalBook){}
		virtual string freezeUser(User &user){}
		
		virtual string showFreezeBook(){}
		virtual string showFreezeUser(){}
		
		virtual string readBookRecord(const PracticalBook &practicalBook){}
		
		virtual string unfreezeBook(PracticalBook &practicalBook){}
		virtual string unfreezeUser(User &_user){}
		
		virtual string removeUser(User user){};
		virtual string removePracticalBook(PracticalBook practicalBook){};
};

class AdminController:public ReaderController
{
	protected:
		vector<User> users;
		void infoUser();
		template<class ObjType>
			string freeze(ObjType obj);//freeze的骨架
		template<class ObjType>
			string unfreeze(ObjType obj);//unfreeze的骨架
	public:
		AdminController(Server *_server,LoginController &loginController):ReaderController(_server,loginController){}
		virtual string type() ;
		virtual User getUser(int number) ;
		virtual string findUser(const string &username);//找读者(普通用户/高级用户)
		virtual string registerUser(const string &username,const string &password,string identity);//注册一个账号(reader或者administrator)
		
		virtual string addBook(Book &book);
		virtual string addNewBook();//增加一本新的书
		virtual string showPendingBook();//展示需要处理的书
		virtual string deal(Record record,bool accept);
		virtual string editBook(Book book);//
		
		virtual string freezeBook(PracticalBook &practicalBook){return freeze(practicalBook);}
		virtual string freezeUser(User &user){return freeze(user);}
		
		
		virtual string showFreezeBook();
		virtual string showFreezeUser();
		
		virtual string readBookRecord(const PracticalBook &practicalBook);
		
		
		virtual string unfreezeBook(PracticalBook &practicalBook){return unfreeze(practicalBook);}
		virtual string unfreezeUser(User &_user){return unfreeze(_user);}
};

class RootController:public AdminController
{
	template<class ObjType>
		string removeObject(ObjType obj);//remove的骨架
	public:
		RootController(Server *_server,LoginController &loginController):AdminController(_server,loginController){}
		virtual string type();
		virtual string removeUser(User user);
		virtual string removePracticalBook(PracticalBook practicalBook);
		//template<class ObjType>
			//string modify(const Obj);
};
