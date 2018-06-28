#include<bits/stdc++.h>
#include "controller.h"
using namespace std;

class Client
{
	LoginController *loginController;
	ReaderController *controller;
	Server *server;
	//登录需要
	string login();
	
	//对于列表里第number本书的后续操作
	void showBook(int number);
	//对于列表里第number项记录的后续操作
	void showRecord(int number);
	//对于列表里第number位用户的
	void showUser(int number);
	
	//输出controller::info包含的列表信息
	void outputInfo();
	
	//搜索书籍
	void searchBook();
	//高级搜索
	void higherSearchBook();
	//搜索书籍之后的操作
	void afterSearchBook();
	
	//浏览列表里第number本书
	void browseBook(int number);
	//编辑一本书
	void editBook(const Book &book);
	//对列表里第number本书提出借阅请求
	void borrowBook(int number);
	
	//搜索借阅中的书籍
	void listBorrowingBooks();
	
	//对列表第number本书提出归还请求
	void returnBook(int number);
	
	//修改密码
	void modifyPassword();
	
	//从用户名搜索用户
	void searchUser();
	//对于列表的操作
	void afterSearchUser();
	
	//需要处理的书
	void showPending();
	//处理第number本书
	void deal(int number,bool accept);
	
	//冻结列表里第number位用户
	void freezeUser (int number);
	//冻结列表里第number本书
	void freezeBook(int number);
	//搜索冻结中的书
	void listFreezeBook();
	//搜索冻结中的用户
	void listFreezeUser();
	//解冻列表里第number本书
	void unfreezeBook(int number);
	//解冻列表里第number位用户
	void unfreezeUser(int number);
	
	//显示列表中第number位用户的记录
	void readUserRecord(int number);
	//显示列表中第number本书的记录
	void readBookRecord(int number);
	//显示自己的记录
	void readSelfRecord();
	//搜索记录之后的操作
	void switchRecord();
	
	//添加新书
	void addNewBook();
	//添加一本有同类的书
	void addBookFromExist(int number);
	
	//注册用户
	void registerUser();

	//删除列表第number本书
	void removeBook (int number);
	//删除列表第number位用户
	void removeUser (int number);
	public:
		Client(Server *_server);
		void main();
};
