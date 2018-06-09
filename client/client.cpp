#include <bits/stdc++.h>
#include "client.h"
using namespace std;

string Loginclient::start(LoginController *log)
{
		cout<<"输入用户名和密码";
		string a,b;
		cin >> a >> b;
		vector<string> s=log->login(a,b);
			cout<<s[1]<<" "<<s[2]<<endl;
		if(s[0]!="错误")
		{
			return s[1];
		}
		else
		{
			return "错误";
			}
	}
	
void ReaderController::switchrecord(ReaderController *rct)
{
	string i;
	cin >> i;
	while(i=="详细信息")
	{
		int j;cin>>j;
		showrcd(rct,j);
		cin >> i;
	}
}

void ReaderController::outputinfo(ReaderController *rct)
{
	int j=rct->info.size();
	if(j==0) return;
	for(string i=0;i<j;i++)
	{
		cout<<rct->info[i]<<" ";
		if(i%3==2)
		cout<<endl;
	}
}

void ReaderController::searchbook(ReaderController *rct)
{
	string name;
	cin >> name;
	rct->commands.push_back(name);
	cout<<rct->searchbook()<<endl;
	outputinfo(rct);
	Aftersearch(rct);
}

void ReaderController::highersearchbook(ReaderController *rct)
{
	string that[6];// no >> name >> author>>isbn>>publisher>>remarks
	for(int i=0;i<6;i++)
	{
		cin >> that[i];
		rct->commands.push_back(that[i]);
		}
	cout<<rct->searchbook()<<endl;
	outputinfo(rct);
	Aftersearch(rct);
	
}

void ReaderController::Aftersearch(ReaderController *rct)
{
	string i;
	cin >> i;
	while(i!="退出")
	{
		int j;
		cin >>j;
		switch(i)
		{
			case "详细信息":
				showbook(rct,j);
			case "借阅":
				borrowbook(rct,j);
			case "浏览":
				browsebook(rct,j);
			case "记录":
				readbookrcd(rct,j);
			case "冻结":
				freezebook(rct,j);
			case "删除":
				removebook(rct,j);
		}
		cout<<rct->searchbook()<<endl;
		outputinfo(rct);
		cin >> i;
	}
	rct->commands.clear();
}

void ReaderController::showbook(ReaderController *rct,int i)
{
	Book book=rct->getbook(i);
	rct->show(book);
	for(string j:rct->deepinfo)
		cout<<s<<" ";
	cout<<endl;
}

void ReaderController::borrowbook(ReaderController *rct,int i)
{
	Book book=rct->getbook(i);
	cout<<rct->borrow(book)<<endl;
}

void ReaderController::browsebook(ReaderController *rct,int i)
{
	Book book=rct->getbook(i);
	cout<<rct->browsebook(book)<<endl;
}

void ReaderController::listborrowingbook(ReaderController *rct)
{
	cout<<rct->listborrowingbooks(rct->getus())<<endl;
	outputinfo(rct);
}

void ReaderController::returnbook(ReaderController *rct,int i)
{
	PracticalBook prbook=rct->getprbook(i);
	string s=rct->returnbook(prbook);
	cout<<s<<endl;
	if(s=="已发出归还请求")
	{
		rct->listborrowingbooks(rct->getus());
		outputinfo(rct);
	}
}

void ReaderController::modifyPassword(ReaderController *rct)
{
	string first,second;
	cin >> first >> second;
	cout<<rct->modifypasswd(first,second)<<endl;
}

void ReaderController::readselfrcd(ReaderController *rct)
{
	cout<<rct->readrecord(rct->user)<<endl;
	outputinfo(rct);
	switchrecord(rct);
}

void ReaderController::showrcd(ReaderController *rct, int i)
{
	Record record=rct->getrcd(i);
	rct->show(record);
	for(string i:rct->deepinfo)
		cout<< i <<" "
	cout<<endl;
}

void Adminclient::viewusr(AdminController *rct)//对于user的操作
{
	string username,name;
	cin >> username >> name;
	rct->commands.push_back(username);
	rct->commands.push_back(name);
	cout<<rct->finduser()<<endl;
	outputinfo(rct);
	string i;
	cin >> i;
	while(i!="退出")
	{
		int j;
		cin >> j;
		switch(i){
		case "详细信息":
			showusr(rct,j);
		case "冻结":
			freezeusr(rct,j);
		case "记录":
			readusrrcd(rct,j);
		case "删除":
			removeusr(rct,j)
		}
		cout<<rct->finduser()<<endl;
		outputinfo(rct);
		cin >> i;
	}
	rct->commands.clear();
}

void Adminclient::showusr(AdminController *rct, int i)
{
	User user=rct->getusr(i);
	rct->show(user);
	for(string j:rct->deepinfo)
		cout<<s<<" ";
	cout<<endl;
}

void Adminclient::readusrrcd(AdminController *rct,int i)
{
	User user=rct->getusr(i);
	cout<<rct->readrecord(&user)<<endl;
	outputinfo(rct);
}

void Adminclient::addbk(AdminController *rct)
{
	string number,index;
	cin >> number>>index;
	rct->commands.push_back(number);
	rct->commands.push_back(index);
	cout<<rct->addbook()<<endl;
}

void Adminclient::freezeusr(AdminController *rct,int i)
{
	User user=rct->showusr(i);
	cout<<rct->freezeusr(user)<<endl;
}

void Adminclient::freezebook(AdminController *rct,int i)
{
	User user=rct->showprbook(i);
	cout<<rct->freezeusr(user)<<endl;
}

void Adminclient::listfreezebook(AdminController *rct)
{
	cout<<rct->showfreezebk()<endl;
	outputinfo(rct);
	string i;
	cin >> i;
	while(i=="解冻")
	{
		int j;
		cin >> j;
		unfreezebk(rct,j);
		rct->showfreezebk();
		outputinfo(rct);
		cin >> i;
	}
}

void Adminclient::listfreezeuser(AdminController *rct)
{
	cout<<rct->showfreezeusr()<endl;
	outputinfo(rct);
	string i;
	cin >> i;
	while(i=="解冻")
	{
		int j;
		cin >> j;
		unfreezeusr(rct,j);
		cout<<rct->showfreezeusr()<<<endl;
		outputinfo(rct);
		cin >> i;
	}
}

void Adminclient::unfreezebk(AdminController *rct,int i)
{
	PracticalBook book=rct->showprbook(i);
	cout<<rct->freezeusr(book)<<endl;
}

void Adminclient::unfreezeusr(AdminController *rct,int i)
{
	User user=rct->showusr(i);
	cout<<rct->freezeusr(user)<<endl;
}


void showpending(AdminController *rct)
{
	cout<<rct->showpendingbk()<<endl;
	outputinfo(rct);
	string i;
	cin >> i;
	while(i=="处理")
	{
		int k;
		string j;
		cin >>k >> j;
		switch(j)
		{
			case "拒绝":
				deal(k,0);
			case "接受":
				deal(k,1);
		}
		cout<<rct->showpendingbk()<<endl;
		outputinfo(rct);
		cin >> i;
	}
}

void deal(AdminController *rct, int i, bool j)
{
	Record rcd=rct->showrcd(i);
	cout<< rct->deal(rcd,j)<<endl;
}

void readbookrcd(AdminController *rct, int i)
{
	Book book=rct->getbook(i);
	cout<<rct->higherreadrecord(book)<<endl;
	outputinfo(rct);
	switchrecord(rct);
}

void readusrrcd(AdminController *rct,int i)
{
	User user=rct->getusr(i);
	cout<<rct->readrecord(&user)<<endl;
	outputinfo(rct);
	switchrecord(rct);
}

void Rootclient::removebook(RootController *rct,int i)
{
	rct->info.clear();
	Book book=rct->getbook(i);
	rct->booksearchpr(book);
	outputinfo(rct);
	cout<<"您要删除第几本？"<<endl;
	int j;
	cin >> j;
	PracticalBook book=rct->getprbook(i);
		cout<<rct->remove(book)<<endl;
}

void Rootclient::removeusr(RootController *rct,int i)
{
	User user=rct->getusr(i);
	cout<<rct->remove(user)<<endl;
}
//======Gu Gu Gu!======

