#include<bits/stdc++.h>
#include<../controller/controller.h>
using namespace std;

class Loginclient
{
	public:
		string start(LoginController *log);
}

class Readerclient{
	protected:
		void Aftersearch(ReaderController *rct);//看完书之后的
		void freezebook(ReaderController *rct, int i);
		void showbook(ReaderController *rct,int i);
		void outputinfo(ReaderController *rct);//输出消息
		void borrowbook(ReaderController *rct,int i);
		void browsebook(ReaderController *rct,int i);
		void returnbook(ReaderController *rct,int i);
		void showrcd(ReaderController *rct);
		virtual void readbookrcd(ReaderController *rct, int i){cout<<"您无权阅读此书记录"<<endl;}
		virtual void freezebook(ReaderController *rct, int i){cout<<"您无权冻结此书"<<endl;}
		virtual void removebook(ReaderController *rct, int i){cout<<"您无权删除此书"<<endl;}
		void switchrecord(ReaderController *rct);
	public:
		Readerclient(){}
		void searchbook(ReaderController *rct);
		void highersearchbook(ReaderController *rct);
		void listborrowingbooks(ReaderController *rct);
		void modifyPassword(ReaderController *rct);
		void readselfrcd(ReaderController *rct);
	}

class Adminclient:public Readerclient
{
	protected:
		void deal(AdminController *rct, int i,bool j);
		void showusr(AdminController *rct, int i);
		void readusrrcd(AdminController *rct,int i);
		virtual void freezeusr (AdminController *rct,int i)override final;
		virtual void freezebook (AdminController *rct,int i)override final;
		void unfreezebk(AdminController *rct,int i);
		void unfreezeusr(AdminController *rct,int i);
		virtual void removeusr(AdminController *rct,int i){cout<<"您无权删除此用户"<<endl;}
	public:
		void viewusr(AdminController *rct);
		
		void addbk(AdminController *rct);
		
		void listfreezebook(AdminController *rct);
		void listfreezeuser(AdminController *rct);
		
		void Register(AdminController *rct);
		void showpending(AdminController *rct);
		
		void readbookrcd(AdminController *rct, int i);
		void readusrrcd(AdminController *rct,int i);
}

class Rootclient:public Adminclient
{
	protected:
		virtual void removebook (RootController *rct,int i)override final;
		virtual void removeusr (RootController *rct,int i)override final;
}
