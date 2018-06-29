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
			cout<<"是否退出?请输入Y/N"<<endl;
			char quit;
			cin >> quit;
			if(quit=='Y'||quit=='y')//这种东西是需要记住,大小写不能分!
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
		if(i%3==0)
			cout<<endl;
	}
}

void Client::searchBook()
{
	cout<<"请输入书名"<<endl;
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
	string instruction="可选操作为:1.高级搜索,2.查看详细信息,3.借阅书籍,4.浏览书籍,退出请按0";
	int key;
	while(true)
	{
		cout<<instruction<<endl;
		cin >> key;
		switch(key)
		{
			case 0:
				return;
			case 1:
			{
				higherSearchBook();
				return;
			}
			default:
			{
				cout<<"请输入书的序号"<<endl;
				int number;
					cin >>number;
				switch(key)
				{
					case 2:
						showBook(number);
					case 3:
						borrowBook(number);//通过book来借阅
					case 4:
						browseBook(number);//通过book来浏览
					default:
						cout<< "命令错误"<<endl;
				}
			}
		}
	}
	}

void Client::showBook(int number)
{
	string instruction="";
	string identity=controller->type();
	if(identity!="Reader")
		instruction="可选操作为:1.编辑书籍信息,2.查看书籍记录,3.冻结此书籍,4.添加一本书,";
	if(identity=="Root")
		instruction+="5.删除一本书,";
	instruction+="退出请输入0";
	Book book=controller->getBook(number);
	controller->show(book);
	for(string j:controller->deepInfo)
		cout<<j<<" ";
	cout<<endl;
	int key;
	while(true)
	{
		cout<<instruction<<endl;
		cin >> key;
		if(key==0)
			return;
		if(controller->type()=="Reader")
			cout<<"命令错误或您无权执行此操作"<<endl;
		else
		{
			switch(key)
			{
				case 1:
					editBook(book);
			default:
			{
				int number;
				cin >> number;
				switch(key)
				{
				case 2:
					readBookRecord(number);
				case 3:
					freezeBook(number);
				case 4:
					addBookFromExist(number);
				case 5:
				{
					if(controller->type()=="Root")
						removeBook(number);
					else
						cout<<"命令错误或您无权执行此操作"<<endl;
				}
				default:
					cout<<"命令错误或您无权执行此操作"<<endl;
				}
				}
				}
			}
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
	
	int key;
	while(true)
	{	cout<<"还书请输入1,退出请输入0"<<endl;
		cin >> key;
		if(key==0)
			return;
		if(key==1)
		{
			cout<<"请输入归还书籍的序号";
			int number;
			cin >> number;
			returnBook(number);
		}
		else
			cout<<"命令错误"<<endl;
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
	system("stty -echo");
	cout<<"请输入密码"<<endl;
	cin >> first;
	cout<<"请再次输入密码"<<endl;
	cin >>second;
	system("stty echo");
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
	string instruction="可选操作为:1.查看用户详细信息,2.冻结用户,3.查看用户记录,";
	if(controller->type()=="Root")
		instruction+="4.删除用户,";
	instruction+="退出请输入0";
	int key;

	while(key!=0)
	{
		cout<<instruction<<endl;
		cin >> key;
		cout<<"请输入用户的序号"<<endl;
		int number;
		cin >> number;
		switch(key)
		{
			case 0:
				return;
			case 1:
				showUser(number);
			case 2:
				freezeUser(number);
			case 3:
				readUserRecord(number);
			case 4:
			{
				string s=controller->type();
				if(s=="Root")
					removeUser(number);
				else
					cout<<"命令错误"<<endl;
			}
			default:
				cout<<"命令错误"<<endl;
		}
		}
		
	}

void Client::searchUser()//对于user的操作
{
	cout<<"请输入用户名"<<endl;
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
	string userName,password1,password2,identity;
	cout<<"请输入用户名"<<endl;
	cin >> userName;
	cout<<"请输入密码"<<endl;
	system("stty -echo");
	cin >> password1;
	cout<<"请再次输入密码"<<endl;
	bool pass=false;
	while(!pass)
	{
		cin >> password2;
		if(password1==password2)
			pass=true;
		else
			cout<<"两次密码不一致,请重新输入!"<<endl;
	}
	system("stty echo");
	cout<<"请输入身份:Reader/Admin/Root"<<endl;
		cin>>identity;
	cout<< controller->registerUser(userName,password1,identity)<<endl;
}

void Client::switchRecord()
{
	
	int key;
	while(true)
	{
		cout<<"查看记录请输入1,退出请输入0"<<endl;
		cin >> key;
		if(key==0)
			return;
		if(key==1)
		{
			cout<<"请输入记录序号"<<endl;
			int number;
			cin >> number;
			showRecord(number);
		}
		else
			cout<<"命令错误"<<endl;
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
	cout<<"请依次输入书号,书名,作者,ISBN号,出版社和评论,如果暂时不知道请输入'-'"<<endl;
	string that[6];// no >> name >> author>>isbn>>publisher>>remarks
	for(int i=0;i<6;i++)
	{
		cin >> that[i];
		controller->commands.push_back(that[i]);
		}
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
	while(true)
	{	
		cout<<"解冻书籍请输入1,退出请输入0"<<endl;
		int key;
		cin >> key;
		if(key==0)
			return;
		else if(key==1)
		{
			cout<<"请输入书籍编号"<<endl;
			int number;
			cin >> number;
			unfreezeBook(number);
			controller->showFreezeBook();
			outputInfo();
		}
		else
			cout<<"命令错误"<<endl;
	}
}

void Client::listFreezeUser()
{
	cout<<controller->showFreezeUser()<<endl;
		outputInfo();
	while(true)
	{	
		cout<<"解冻用户请输入1,退出请输入0"<<endl;
		int key;
		cin >> key;
		if(key==0)
			return;
		else if(key==1)
		{
			cout<<"请输入用户编号"<<endl;
			int number;
			cin >> number;
			unfreezeUser(number);
			controller->showFreezeUser();
			outputInfo();
		}
		else
			cout<<"命令错误"<<endl;
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
	while(true)
	{
		cout<<controller->showPendingBook()<<endl;
		outputInfo();
		cout<<"接受一个操作请输入1,拒绝一个操作请输入2,退出请输入0"<<endl;
		int key;
		cin >> key;
		switch(key)
		{
		case 0:
			return;
		default:
		{
			cout<<"请输入操作序号"<<endl;
			int number;
			cin >> number;
			switch(key)
			{
				case 1:
					deal(number,1);
				case 2:
					deal(number,0);
				default:
					cout<<"操作输入错误"<<endl;
			}
		}
		}
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

void Client::readUserRecord(int number)//无
{
	User user=controller->getUser(number);
	cout<<controller->readRecord(user)<<endl;
	outputInfo();
	switchRecord();
}

void Client::removeBook(int number)//说了
{
	controller->info.clear();
	Book book=controller->getBook(number);
	controller->bookToPractical(book);
	cout<<"您要删除第几本？"<<endl;
	outputInfo();
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

Client::Client(Server *_server):server(_server){}

void Client::main()
{
	string instruction;
	loginController=new LoginController(server);
	string identity=login();
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
			instruction="一级操作为1.普通搜索,2.高级搜索,3.查看记录,4.修改密码,5.归还书籍,退出请按0";
			}
		else 
		{
			if(identity=="Admin")
				controller=new AdminController(server,loginController);
			if(identity=="Root")
				controller=new RootController(server,loginController);
				instruction="一级操作为1.普通搜索,2.高级搜索,3.查看记录,4.修改密码,5.归还书籍,6.添加书籍,7.处理借还,8.搜索用户,9.注册用户,请输入操作编号,退出请按0";
		}
	}
	int option;
	while(true)
	{
		cout<<instruction<<endl;
		cin >> option;
		switch (option)
		{
			case 0:
			{
				cout<<"成功退出"<<endl;
				delete controller;
				return;
			}
			case 1:
				searchBook();
			case 2:
				higherSearchBook();
			case 3:
				readSelfRecord();
			case 4:
				modifyPassword();
			case 5:
				listBorrowingBooks();
			default:
			{
				if(controller->type()=="Reader")
					cout<<"命令错误"<<endl;
				else
				{
					switch(option)
					{
						case 6:
							addNewBook();
						case 7:
							showPending();
						case 8:
							searchUser();
						case 9:
							registerUser();
						default:
							cout<<"命令错误"<<endl;	
						}
					}
				}		
			}
		}
}
//======Gu Gu Gu!======

