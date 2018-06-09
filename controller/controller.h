#include <bits/stdc++.h>

#include "../server.h"

using namespace std;

class AbstractController{//所有Controller的基类,派生两个类:登陆界面,读者/管理员/root界面
	protected:
		Server *server;//用到的服务器名称
		User *user;//用到的用户
		bool a;
	public:
		AbstractController(Server *_server):server(_server),a(0){}
		~AbstractController(){
		if(a==0)//保证登录成功退出LoginController的时候不会删除服务器和用户
		{
			if(server!=nullptr)
				delete server;
			if(user!=nullptr)
				delete user;
		}
	}
		User* getus();

}

class LoginController:public AbstractController{
public:
	LoginController(Server *_server):AbstractController(_server){}
	vector<string> login(const string &usnm,const string &passwd);
	Server* getserver();
	User* getuser();
}
};//放到后面的AdminRegister里面

class ReaderController:public AbstractController{//找书
	protected:
		vector<Book> books;//检索出来的书籍存放在这里
		vector<PracticalBook> prbooks;
		vector<Record> record;
		void infobks();
		void infoprbook();
		void inforcd();
	public:
		vector<string> commands;//不确定行数的命令放这里(即时)
		vector<string> info;//多个Obj的简要信息存放在这里
		vector<string> deepinfo;//一个Obj的繁琐信息存放在这里
		ReaderController(Server *_server,User *_user):AbstractController(_server),use(_user){}
		~ReaderController(){}
		//接下来3个为client接口
		Book getbook(int i);//第几条书
		PracticalBook getprbook(int i);//第几本书
		Record getrcd(int i);//第几条记录
		//接下来是
		string searchbook();
		void booksearchpr(const Book &book);
		template<class ObjType>
			void show(ObjType &vect);//显示一个ObjType的全部信息
		string listborrowingbooks(User *use);//列出自己借阅过/正在借阅的书籍
		string borrow(Book &book);//yc可以把show出来的书籍里给出一个"借阅"按钮

		string returnbook(PracticalBook &book);//
		string modifypasswd(string pwd1,string pwd2);
		string readrecord(User *use);//读阅记录
		string browsebook(Book &book);//
}

class AdminController:public ReaderController
{
	protected:
		vector<User> users;
		void infousrs();
	public:
		AdminController(Server *_server,User *_user):ReaderController(_server,_user){}
		User getusr(int i);
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
		RootController(Server *_server,User *_user):AdminController(_server,_user){}
		template<class ObjType>
			string remove(const ObjType &obj);
		//template<class ObjType>
			//string modify(const Obj);
}
