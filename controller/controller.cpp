#include <bits/stdc++.h>
#include "controller.h"
using namespace std;

User* AbstractController::getus()
{
	return user;
}

vector<string> LoginController::login(const string &usnm,const string &passwd)//LoginController中的update是登录的操作,先把用户名和密码明文打包成vector
{
	user= new User(usnm,passwd);
	commands.clear();
	ErrorCode errorcode=server->userlogin(*user);
	if(errorcode==noError)
	{	a=1;
		return {"登录成功,您的身份是"+user["Role"],
		user["Role"]};
	}
	else
	{
		delete user;
		vector<string> cmd;
		cmd.push_back("错误");
		switch(errorcode){
		case noSuchUser:
			cmd.push_back("并没有这个用户");
		case wrongPassword:
			cmd.push_back("密码错误");
		case loginFailed:
			cmd.push_back("用户被冻结");
			}
		return cmd;
		}
	}
	
Book ReaderController::getbook(int i)
{
	return books[i-1];
}

PracticalBook ReaderController::getprbook(int i)
{
	return books[i-1];
}

Record ReaderController::getrcd(int i)
{
	return record[i-1];
}

void ReaderController::infobks()
{
	for(Book j:books)//列表内只显示名字/出版社
		{
			info.push_back(j["Name"]);
			info.push_back(j["Author"]);
			info.push_back(j["Publisher"]);
		}
}

void ReaderController::infoprbook()
{
	for(Book j:prbooks)//列表内只显示名字/出版社
		{
			info.push_back(j["No"]);
			info.push_back(j["Index"]);
			info.push_back(j["Status"]);
		}
}

void ReaderController::inforcd()
{

	for(Record i:record)
	{
		info.push_back(i["BookNo"]);
		info.push_back(i["Time"]);
		info.push_back(i["Type"]);
	}
}

string ReaderController::searchbook();//找书,关键信息全部输入commands里
{
	info.clear();
	if(command.size()==1)
		server->search(*user,multiset<Field>{Field("Name",commands[0])},books);
	else //如果点击"高级搜索"则在commands里push一个"others",你可以给出一张页面(输入不同方法检索串的bar)
	{
		for(string i:commands)
		{	if(i=="-")
				i="";
			}
		server->search(*user,multiset<Field>{Field("No",commands[0]),Field("Name",commands[1]),Field("Author",commands[2]),Field("ISBN",commands[3]),Field("Publisher",commands[4]),Field("Remarks",commands[5])},books);//分词怎么分?最好用
		}//使用完命令即删除
		int i=books.size();
		infobks();
		return{"共发现"+int2str(i)+"种书"};
}

template<class ObjType>
void ReaderController::show(ObjType &vect)//点击一下,深度显示一个对象(操作).每"显示"一个对象,就把其所有的explicitkey全部推到里面去.
{
	deepinfo.clear();//每次只show一个对象
	vector<string> temp=vect.explicitKey();
	for(string i:temp)
	{
		deepinfo.push_back(i);
		deepinfo.push_back(ObjType[i]);
	}
}

string ReaderController::listborrowingbooks(User *use)
{
	info.clear();
	prbooks.clear();
	vector<string> s;
	server->search(*use,multiset<Field>{Field("Username",currentUser["Username"]),Field("Status","Accepted"),},record);
	for(auto i:record)
	{
		server->search(*use,multiset<Field>{Field("No",i["BookNo"]),Field("BookIndex",i["BookIndex"])},s);
		prbooks.push_back(s[0]);
	}
	inforcd();
	int j=prbooks.size();
	return "共借阅"+int2str(j)+"本书"
}

string ReaderController::booksearchpr(const Book &book)
{
	info.clear();
	server->search(*user,multiset<Field>{Field("No",book["No"]),prbooks});
	infoprbook();
}

string ReaderController::borrow(Book& book)//点击操作
{
	for(auto i:prbooks)
	{
		ErrorCode errorcode=server->borrowBook(*user,i);
		if(errorcode==noError||errorCode==requestAlreadyProcessed)
		{
			prbooks.clear();
			return "已发出借阅请求";
		}
	}
	return "借阅失败";
}

string ReaderController::returnbook(PracticalBook &book)//点击操作
{
	ErrorCode errorcode=server->returnBook(*user,Book)
	switch(errorcode)
	{
		//case bookNotFound:
			//return "错误,未找到书籍"; //留给后续支持
		case bookNotBorrowed:
			return "错误,该书籍未被借阅";
		default:
			return "已发出归还请求";
	}
}



string ReaderController::modifypasswd(string pwd1,string pwd2)//只有进入才能修改密码!
{
	if(pwd1!=pwd2)
		return "两次密码不一致请重输";//再次输入密码检验
	server->modifyPassword(*user,Password(pwd1));
		return "成功修改密码";
}

string ReaderController::readrecord(User *use)
{
	info.clear();
	ErrorCode err=server->search(*user,multiset<Field>{Field("Username",(*use)["Username"])},record)
	switch (err)
	{
	case permissionDenied:
		return "您无权读取此用户的历史记录";
	case noError:
		inforcd();
		int i=record.size();
		return "共发现"+int2str(i)+"条历史记录";
	}
}


string ReaderController::browsebook(Book &book)
{
	info.clear();
	ErrorCode err=server->browseBook(*user,book);
	switch(err)
	{
		//case bookNotFound:
			//return "错误:未找到书籍";
		case bookInaccessible:
			return "错误:不可查看该书籍";
		case noError:
			{
				Content *content;
				ErrorCode errorcode=server->previewBookContent(*user,book,content);
				switch(errorcode):
				{
					case unknownContentSuffix:
						return "错误:格式不支持";
					case bookContentMissing:
						return "错误:内容丢失";
					case noError:
						{
							return content->show();
						}
				}
			}
	}
}

User AdminController::getusr(int i)
{
	return users[i-1];
}

void AdminController::infousrs()
{
	for(User i:users)
	{
		info.push_back(i["Username"]);
		info.push_back(i["Realname"]);
		info.push_back(i["School"]);
	}
}

string AdminController::finduser()//通过ID或者真名查找用户(非管理员),(保证username是unique的)?
{
	info.clear();
	for(string i:commands)
		{	if(i=="-")
				i="";
			}
	server->search(*user,multiset<Field>{Field("Username",commands[0]),Field("Realname",commands[1])},users);
	infousrs();
	int j=users.size();
	return "共发现"+int2str(j)+"位用户";
	}

string AdminController::Register()//从vector<>command里面提供材料
{
	User user=User(commands[0],commands[1]);
	User.update("Role",commands[2]);//commands[2]里存放身份,在client操作(前端衔接)的时候直接补
	ErrorCode errorcode=server->add(user);
	commands.clear();//踢去这种command
	switch(errorcode){
		case invalidInfo:
			return "信息无效";
		case permissionDenied:
			return "您无权创建此用户";
		case noError:
			return "创建用户成功";
	}
}

string AdminController::addbook()//从原来
{
	PracticalBook book(commands[0],commands[1]);
	book.update("Status","Available");
	server->search(*user,multiset<Field>{Field("No",commands[0])},books);
	if(books.empty())
	{
		Book bk=Book(commands[0]);//创建一类新的信息
		server->add(*user,bk);
		return "原本无相同书,添加后请编辑详细信息";
	}
	server->add(*user,book);
	return "成功添加";
}

string AdminController::showpendingbk()
{	
	info.clear();
	ErrorCode err=server->search(*user,multiset<Field>{Field("Status","Pending")},record);
	inforcd();
	int i=record.size();
	return "共发现"+int2str(i)+"本需要处理的书";
}

string AdminController::deal(const Record& rcd,bool a)//管理员接受处理借书请求
{
	if(a==1)
		ErrorCode err=server->acceptRequest(*user,rcd);
	else
		ErrorCode err=server->rejectRequest(*user,rcd);
	switch(err)
	{
		case requestNotFound:
			return "错误:未找到可操作记录";
		case requestAlreadyProcessed
			return "错误:已经处理过该记录了";
		case noError:
			return "成功处理记录";
	
	}
}

template<class ObjType>
string AdminController::freeze(ObjType &obj)//冻结书籍或者用户专用
{
	ErrorCode err=server->freeze(*user,obj);
	switch(err){
	case objectNotAccessible:
		return "对象不可被冻结";
	case loginAgain:
		return "您已掉线,请再次登录";
	case permissionDenied:
		return "您无权限冻结此对象";
	case noError:
		return "冻结成功";
	}
} 

string AdminController::showfreezebk()
{
	info.clear();
	server->search(*user,multiset<Field>{Field("Status","Frozen")},prbooks);
	for(PracticalBook i:prbooks)
	{
		info.push_back(i["No"]);
		info.push_back(i["Index"]);
	}
	int j=prbooks.size();
	return "共发现"+int2str(j)+"本被冻结的书";
}

string AdminController::showfreezeusr()
{
	server->search(*user,multiset<Field>{Field("Status","Frozen")},users);
	infousrs();
	int j=users.size();
		return "共发现"+int2str(j)+"个被冻结的用户";
}

string AdminController::higherreadrecord(const PracticalBook &prbook)
{
	info.clear();
	ErrorCode err=server->search(*user,multiset<Field>{Field("BookNo",prbook["BookNo"]),
			Field("BookIndex",prbook["BookIndex"])},record)
	switch (err)
	{
	//case permissionDenied:
		//return "您无权读取此书的历史记录";
	case noError:
		inforcd();
		int i=record.size();
		return "共发现"+int2str(i)+"条历史记录";
	}
}


template<class ObjType>
string AdminController::unfreeze(ObjType &obj)
{
	ErrorCode err=server->unfreeze(*user,obj);
	switch(err){
	case objectNotAccessible:
		return "对象未被冻结";
	case loginAgain:
		return "您已掉线,请再次登录";
	case permissionDenied:
		return "您无权限解冻此对象";
	case noError:
		return "解冻成功";
	}
}

template<class ObjType>
string RootController::remove(const ObjType &obj)
{
	server->remove(*user,obj);
		return "移除成功";
}

//string RootController::modify()

//=====================Gu Gu Gu!========================================================
//string AdminController::higherrecord()"需要新的问题!"


