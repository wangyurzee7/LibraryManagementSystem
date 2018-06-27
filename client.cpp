#include <bits/stdc++.h>
#include "client.h"
using namespace std;

string Client::login()
{
		cout<<"请输入用户名和密码"<<endl;
		string a,b;
		cin >> a >> b;
		vector<string> s=loginController->login(a,b);
		if(s[0]!="错误")
		{	
			cout<<s[0]<<endl;
			return s[1];
		}
		else
		{	cout<<s[0]<<","<<s[1]<<endl;
			cout<<"是否退出?Y/N"<<endl;
			char quit;
			cin >> quit;
			if(quit=='Y'||quit=='y')
				return "退出";
			else
				return login();
		}
	}

void Client::outputInfo()
{
	int j=controller->info.size();
	if(j==0) return;
	for(int i=0;i<j;i++)
	{
		if(i%3==0)
			cout<<i/3+1<<" ";
		cout<<controller->info[i]<<" ";
		if(i%3==2)
			cout<<endl;
	}
}

void Client::searchBook()
{
	string name;
	cin >> name;
	controller->commands.push_back(name);
	cout<<controller->searchBook()<<endl;
	outputInfo();
	afterSearchBook();
}

void Client::higherSearchBook()//高级搜索
{
	cout<<"请依次输入书号,书名,作者,ISBN号,出版社和评论,如果没有请输入'-'"<<endl;
	string that[6];// no >> name >> author>>isbn>>publisher>>remarks
	for(int i=0;i<6;i++)
	{
		cin >> that[i];
		controller->commands.push_back(that[i]);
		}
	cout<<controller->searchBook()<<endl;
	outputInfo();
	afterSearchBook();
}

void Client::afterSearchBook()
{
	string key;
	cin >> key;
	while(key!="退出")
	{
		if(key=="高级搜索")
		{
			higherSearchBook();
			return;
		}
		int number;
		cin >>number;
		if(key=="详细信息")
			showBook(number);
		else if(key=="借阅")
			borrowBook(number);//通过book来借阅
		else if(key=="浏览")
			browseBook(number);//通过book来浏览
		else
			cout<< "命令错误"<<endl;
		cin >> key;
		}
	}

void Client::showBook(int number)
{
	Book book=controller->getBook(number);
	controller->show(book);
	for(string j:controller->deepInfo)
		cout<<j<<" ";
	cout<<endl;
	controller->bookToPractical(book);
	outputInfo();
	string key;
	cin >> key;
	while(key!="退出")
	{
		if(controller->type()=="Reader")
			cout<<"命令错误或您无权执行此操作"<<endl;
		else
		{
			if(key=="编辑")
				editBook(book);
			else
			{
				int number;
				cin >> number;
				if(key=="查看书籍记录")
					readBookRecord(number);
				else if(key=="冻结")
					freezeBook(number);
				else if(key=="添加")
					addBookFromExist(number);
				else if(key=="删除")
				{
					if(controller->type()=="Root")
						removeBook(number);
					else
						cout<<"命令错误或您无权执行此操作"<<endl;
				}
				else
					cout<<"命令错误或您无权执行此操作"<<endl;
				}
			}
		cin >> key;
	}
}


void Client::browseBook(int number)
{
	Book book=controller->getBook(number);
	cout<<controller->browseBook(book)<<endl;
}

void Client::borrowBook(int number)
{
	Book book=controller->getBook(number);
	cout<<controller->borrowBook(book)<<endl;
}

void Client::editBook(const Book &book)
{
	cout<<"依次输入书名,作者,ISBN号,出版商,和评论,如果没有请输入'-'"<<endl;
	string input;
	for(int i=0;i<5;i++)
	{
		cin >> input;
		controller->commands.push_back(input);
	}
	cout<<controller->editBook(book)<<endl;
}

void Client::listBorrowingBooks()
{
	cout<<controller->listBorrowingBooks(controller->getSelf())<<endl;
	outputInfo();
	cout<<"输入'退出'或者'归还'来进行下一步操作"<<endl;
	string command;
	cin >> command;
	while(command!="退出")
	{
		if(command=="归还")
		{
			int number;
			cin >> number;
			returnBook(number);
		}
		else
			cout<<"命令错误"<<endl;
		cin >> command;
	}
}

void Client::returnBook(int number)//无问题
{
	string s=controller->returnBook(controller->getPracticalBook(number));
	cout<<s<<endl;
	if(s=="已发出归还请求")
	{
		controller->listBorrowingBooks(controller->getSelf());
		outputInfo();
	}
}

void Client::modifyPassword()//无问题
{
	string first,second;
	cin >> first >> second;
	cout<<controller->modifyPassword(first,second)<<endl;
}

void Client::readSelfRecord()
{
	cout<<controller->readRecord(controller->getSelf())<<endl;
	outputInfo();
	switchRecord();
}

void Client::afterSearchUser()
{
	string key;
	cin >> key;
	while(key!="退出")
	{
		int number;
		cin >> number;
		if(key=="详细信息")
			showUser(number);
		else if(key=="冻结")
			freezeUser(number);
		else if(key=="查看用户记录")
			readUserRecord(number);
		else if(key=="删除")
		{
			string s=controller->type();
			if(s=="Root")
				removeUser(number);
			else
				cout<<"您无权删除该用户"<<endl;
		}
		else
			cout<<"命令错误"<<endl;
		cin >> key;
		}
		
	}

void Client::searchUser()//对于user的操作
{
	string username;
	cin >> username;
	
	cout<<controller->findUser(username)<<endl;
	outputInfo();
	
	afterSearchUser();

}

void Client::showUser(int number)
{
	User user=controller->getUser(number);
	controller->show(user);
	for(string j:controller->deepInfo)
		cout<<j<<" ";
	cout<<endl;
}

void Client::registerUser()
{
	string userName,password,identity;
	cin >> userName >> password >> identity;
	cout<< controller->registerUser(userName,password,identity)<<endl;
}

void Client::switchRecord()
{
	string key;
	cin >> key;
	while(key!="退出")
	{
		if(key=="查看")
		{
			int number;
			cin >> number;
			showRecord(number);
		}
		else
			cout<<"命令错误"<<endl;
		cin >> key;
	}
}

void Client::showRecord(int number)
{
	Record record=controller->getRecord(number);
	controller->show(record);
	for(string i:controller->deepInfo)
		cout<< i <<" ";
	cout<<endl;
}



void Client::addNewBook()
{
	string number,index;
	cin >> number>>index;
	controller->commands.push_back(number);
	controller->commands.push_back(index);
	cout<<controller->addNewBook()<<endl;
}

void Client::addBookFromExist(int number)
{
	Book book=controller->getBook(number);
	cout<<controller->addBook(book)<<endl;
}

void Client::freezeUser(int number)
{
	User user=controller->getUser(number);
	cout<<controller->freezeUser(user)<<endl;
}

void Client::freezeBook(int number)
{
	PracticalBook practicalBook=controller->getPracticalBook(number);
	cout<<controller->freezeBook(practicalBook)<<endl;
}

void Client::listFreezeBook()
{
	cout<<controller->showFreezeBook()<<endl;
	outputInfo();
	string key;
	cin >> key;
	cout<<"输入'解冻'或'退出'"<<endl;
	while(key=="解冻")
	{
		int number;
		cin >> number;
		unfreezeBook(number);
		controller->showFreezeBook();
		outputInfo();
		cin >> key;
	}
}

void Client::listFreezeUser()
{
	cout<<controller->showFreezeUser()<<endl;
	outputInfo();
	string key;
	cin >> key;
	cout<<"输入'解冻'或'退出'"<<endl;
	while(key=="解冻")
	{
		int number;
		cin >> number;
		unfreezeUser(number);
		cout<<controller->showFreezeUser()<<endl;
		outputInfo();
		cin >> key;
	}
}

void Client::unfreezeBook(int number)
{
	PracticalBook book=controller->getPracticalBook(number);
	cout<<controller->freezeBook(book)<<endl;
}

void Client::unfreezeUser(int number)
{
	User user=controller->getUser(number);
	cout<<controller->freezeUser(user)<<endl;
}


void Client::showPending()
{
	cout<<controller->showPendingBook()<<endl;
	outputInfo();
	string i;
	cin >> i;
	while(i=="处理")
	{
		int number;
		string key;
		cin >> number >> key;
		if(key=="拒绝")
			deal(number,0);
		else if(key=="接受")
			deal(number,1);
		else
			cout<<"请重新输入命令"<<endl;
		controller->showPendingBook();
		outputInfo();
		cin >> i;
		}
	}

void Client::deal(int number, bool accept)
{
	Record record=controller->getRecord(number);
	cout<< controller->deal(record,accept)<<endl;
}

void Client::readBookRecord(int number)
{
	PracticalBook practicalBook=controller->getPracticalBook(number);
	cout<< controller->readBookRecord(practicalBook)<<endl;
	outputInfo();
	switchRecord();
}

void Client::readUserRecord(int number)
{
	User user=controller->getUser(number);
	cout<<controller->readRecord(user)<<endl;
	outputInfo();
	switchRecord();
}

void Client::removeBook(int number)
{
	controller->info.clear();
	Book book=controller->getBook(number);
	controller->bookToPractical(book);
	outputInfo();
	cout<<"您要删除第几本？"<<endl;
	int index;
	cin >> index;
	PracticalBook practicalBook=controller->getPracticalBook(index);
	cout<<controller->removePracticalBook(practicalBook)<<endl;
}

void Client::removeUser(int number)
{
	User user=controller->getUser(number);
	cout<<controller->removeUser(user)<<endl;
}

void Client::main()
{
	loginController=new LoginController(server);
	string identity=login();
	// string identity="退出";
	if(identity=="退出")
	{
		delete loginController;
		cout<<"成功退出"<<endl;
		return;
	}
	else
	{
		if(identity=="Reader")
		{
			controller=new ReaderController(server,loginController);
			cout<<"一级操作为普通搜索,高级搜索,查看记录,修改密码,归还书籍"<<endl;
			}
		else if(identity=="Admin")
		{
			controller=new AdminController(server,loginController);
			cout<<"一级操作为普通搜索,高级搜索,查看记录,修改密码,归还书籍,添加书籍,处理借还,搜索用户,注册用户"<<endl;
			}
		else
		{
			controller=new RootController(server,loginController);
			cout<<"一级操作为普通搜索,高级搜索,查看记录,修改密码,归还书籍,搜索用户,添加书籍,处理借还,搜索用户,注册用户"<<endl;
		}
	}

	string option;
	cin >> option;
	while(option!="退出")
	{
		if(option=="普通搜索")
			searchBook();
		else if(option=="高级搜索")
			higherSearchBook();
		else if(option=="查看记录")
			readSelfRecord();
		else if(option=="修改密码")
			modifyPassword();
		else if(option=="归还书籍")
			listBorrowingBooks();
		else
		{
			if(controller->type()=="Reader")
				cout<<"命令错误"<<endl;
			else
			{
				if(option=="添加书籍")
					addNewBook();
				else if(option=="处理借还")
						showPending();
				else if(option=="搜索用户")
						searchUser();
				else if(option=="注册用户")
						registerUser();
					}
				}
				cout<<"请输入一级命令"<<endl;
				cin >>option;
			}
	cout<<"成功退出"<<endl;
	delete controller;
}
//======Gu Gu Gu!======

