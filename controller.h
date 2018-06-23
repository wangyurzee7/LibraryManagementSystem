/************************************************* 
Copyright:bupt 
Author: Eric Sam
Date:2018-06-11 
Description:serve between server and client 
tips: every information return will in the form of string
**************************************************/  
#include <bits/stdc++.h>

#include "server.h"

using namespace std;

class AbstractController{
	protected:
		Server *server;
		User user;
	public:
		AbstractController(Server *_server):server(_server){}
		AbstractController(Server *_server,User _user):server(_server),user(_user){}
		~AbstractController();
		User getSelf();//拷贝protected成员user
};

class LoginController:public AbstractController
{
	public:
		LoginController(Server *_server):AbstractController(_server){}
		vector<string> login(const string &userName,const string &password);//登录
};


class ReaderController:public AbstractController
{//找书
	protected:
		vector<Book> books;//检索出来的书类放在这里
		vector<PracticalBook> practicalBooks;//检索出来的单本放在这里
		vector<Record> records;//存放检索出来的记录
		void infoBook();//不同书类的简要信息填进vector<Book> books
		void infoPracticalBook();//不同书的简要信息填进vector<PracticalBook> practicalBooks
		void infoRecord();//不同记录的简要信息填进vector<Record> records
	public:
		vector<string> commands;//不确定行数的命令放这里(即时),由client负责输入,controller负责清理
		
		vector<string> info;//搜索出的object的简要信息存放在这里,controller负责在每次需要用的之前清理
		vector<string> deepInfo;//一个object的繁琐信息存放在这里,在每次需要用之前清理
		ReaderController(Server *_server,LoginController *loginController):AbstractController(_server,loginController->getSelf()){}
		~ReaderController(){}
		
		virtual string type();//返回用户级别
		Book getBook(int number);//返回列表第number类书
		string searchBook();//普通搜索书类
		string browseBook(Book &book);//浏览一类书
		
		PracticalBook getPracticalBook(int number);//返回列表第number本书
		
		void bookToPractical(const Book &book);//从一类书找具体的书
		string borrowBook(const Book &book);//借一本书
		
		Record getRecord(int number);//返回列表第number条记录
		
		template<class ObjType>
			void show(const ObjType &object);//将object的信息全部输入vector<string> deepInfo
			
		string listBorrowingBooks(const User &_user);//展示在借书籍
		string returnBook(PracticalBook book);//归还一本书
		
		string modifyPassword(string password1,string password2);//修改密码
		string readRecord(User _user);//阅读一个用户的记录
		
//========the followings are all virtual functions,do not care=============下列皆为接口，无有意义的实现
		virtual User getUser(int number){};
		virtual string findUser(const string &username){}
		virtual string registerUser(const string &username,const string &password,string identity){}
		virtual string addBook(Book &book);
		virtual string addNewBook(){}
		virtual string showPendingBook(){}
		virtual string deal(Record record,bool accept){}
		virtual string editBook(Book book){}
		
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
			string freeze(ObjType obj);//freeze函数的骨架
		template<class ObjType>
			string unfreeze(ObjType obj);//unfreeze函数的骨架
	public:
		AdminController(Server *_server,LoginController *loginController):ReaderController(_server,loginController){}
		virtual string type();
		virtual User getUser(int number) ;//返回列表第number个用户
		virtual string findUser(const string &username);//搜索用户
		virtual string registerUser(const string &username,const string &password,string identity);//注册一个账号（只能是Reader或者Administrator，默认前者）
		
		virtual string addBook(Book &book);//添加一本有同类的书
		virtual string addNewBook();//添加一本新的书
		virtual string showPendingBook();//搜索需要处理的书
		virtual string deal(Record record,bool accept);//处理一本书
		virtual string editBook(Book book);//编辑一本书
		
		virtual string freezeBook(PracticalBook &practicalBook){return freeze(practicalBook);}//冻结一本书
		virtual string freezeUser(User &user){return freeze(user);}//冻结一个用户
		
		
		virtual string showFreezeBook();//搜索冻结的书
		virtual string showFreezeUser();//搜索冻结的用户
		
		virtual string readBookRecord(const PracticalBook &practicalBook);
		
		virtual string unfreezeBook(PracticalBook &practicalBook){return unfreeze(practicalBook);}//解冻一本书
		virtual string unfreezeUser(User &_user){return unfreeze(_user);}//解冻一个用户
};

class RootController:public AdminController
{
	template<class ObjType>
		string removeObject(ObjType obj);//remove的骨架
	public:
		RootController(Server *_server,LoginController *loginController):AdminController(_server,loginController){}
		virtual string type();
		virtual string removeUser(User user);//删除一个用户
		virtual string removePracticalBook(PracticalBook practicalBook);//删除一本书
};
