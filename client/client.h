class loginclient
{
	LoginController login;
	string a,b;
	public:
	loginclient(Server *_server):login(_server){}
	string start();
}

string loginclient::start()
{
		cout<<"输入用户名和密码";
		cin >> a >> b;
		vector<string> s=login.login(a,b);
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

class Readerclient{
	ReaderController rct;
	public:
		Readerclient(){}
		void outputinfo();
		void searchbook();
		void highersearchbook();
		void showbook(int i);
		void borrowbook(int i);
		void browsebook(int i);
		void listborrowingbooks();
		void returnbook(int i);
		void modifyPassword();
		void readselfrcd();
	}

void ReaderController::outputinfo()
{
	int j=info.size();
	if(j==0) return;
	for(string i=0;i<j;i++)
	{
		cout<<info[i]<<" ";
		if(i%3==2)
		cout<<endl;
	}
	return;
}

void ReaderController::searchbook()
{
	string name;
	cin >> name;
	rct.commands.push_back(name);
	cout<<rct.searchbook();
	outputinfo();
}

void ReaderController::highersearchbook()
{
	string that[6];// no >> name >> author>>isbn>>publisher>>remarks
	for(int i=0;i<6;i++)
	{
		cin >> that[i];
		rct.commands.push_back(that[i]);
		}
	cout<<rct.searchbook()<<endl;
	outputinfo();
}

void ReaderController::showbook(int i)
{
	show(rct.books[i-1]);
	for(string j:rct.deepinfo)
		cout<<s<<" ";
}

void ReaderController::borrowbook(int i)
{
	Book book=rct.books[i-1];
	cout<<rct.borrow(book)<<endl;
}

void ReaderController::browsebook(int i)
{
	Book book=rct.books[i-1];
	cout<<rct.browsebook(book)<<endl;
}

void ReaderController::listborrowingbook()
{
	cout<<rct.listborrowingbooks(rct.user);
	outputinfo();
}

void ReaderController::returnbook(int i)
{
	record j=rct.record[i-1];
	rct.server->search(rct.user,multiset<Field>{Field("No",j["BookNo"]),Field("BookIndex",j["BookIndex"])},prbooks);
	string s=rct.returnbook(prbook[0]);
	cout<<s<<endl;
	if(s=="已发出归还请求")
	{
		info[3*i-1]+=" 已归还";
		outputinfo();
	}
}

void ReaderController::modifyPassword()
{
	string first,second;
	cin >> first>>second;
	cout<<rct.modifypasswd(first,second)<<endl;
}

void ReaderController::readselfrcd()
{
	cout<<rct.readrecord(rct.user)<<endl;
	outputinfo();
}

/*class Adminclient
{
	AdminController rct;
	
}

class Rootclient
{
	RootController rct;
}*/
//======Gu Gu Gu!======

