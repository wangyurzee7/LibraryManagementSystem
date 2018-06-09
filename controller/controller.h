#include <bits/stdc++.h>

#include "../server.h"

using namespace std;

class AbstractController{//所有Controller的基类,派生两个类:登陆界面,读者/管理员/root界面
protected:
	Server *server;//用到的服务器名称
	User *user;//用到的用户
public:
	
	AbstractController(Server *_server):server(_server){}
	~AbstractController(){if(server) delete server;if(user) delete user;}
	
}

class LoginController:public AbstractController{
public:
	using AbstractController::AbstractController;
	vector<string> login(const string &usnm,const string &passwd);
};//放到后面的AdminRegister里面

class ReaderController:public AbstractController{//找书
	public:
		ReaderController(Server *_server,User *_user):server(_server),use(_user){}
		vector<string> info;//多个Obj的简要信息存放在这里
		vector<string> deepinfo;//一个Obj的繁琐信息存放在这里
		vector<string> commands;//不确定行数的命令放这里(即时)
		vector<Book> books;//检索出来的书籍存放在这里
		vector<PracticalBook> prbooks;
		vector<Record> record;
		void infobks();
		void inforcd();
		string searchbook();
		template<class ObjType>
			void show(ObjType &vect);//显示一个ObjType的全部信息
		string listborrowingbooks(PracticalBook &prbook);//列出自己借阅过/正在借阅的书籍
		string borrow(Book &book);//yc可以把show出来的书籍里给出一个"借阅"按钮

		string returnbook(PracticalBook &book);//
		string modifypasswd(string pwd1,string pwd2);
		string readrecord(User *use);//读阅记录
		string browsebook(Book &book);//
}

class AdminController:public ReaderController
{
	public:
		usng ReaderController::ReaderController;
		vector<User> users;
		void infousrs();
		string finduser();//找读者(普通用户/高级用户)
		string Register();//注册一个账号(reader或者administrator)
		string addbook();//输入一本书的信息
		string showpendingbk();//展示需要处理的书
		string deal(const Record &rcd,bool a);//a=0是reject,a=1是accept
		
		template<class ObjType>
			string freeze(ObjType &obj);
		
		string showfreezebk();
		string showfreezeusr();
		
		string higherreadrecord(const PracticalBook &prbook);
		template<class ObjType>
			string unfreeze(ObjType &obj);
}

class RootController:public AdminController
{
	public:
		usig AdminController::AdminController;
		template<class ObjType>
			string remove(const ObjType &obj);
		//template<class ObjType>
			//string modify(const Obj);
}
