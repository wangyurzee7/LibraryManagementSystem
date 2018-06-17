#include <bits/stdc++.h>
#include "controller.h"
using namespace std;

AbstractController::~AbstractController()
{
		if(finalQuit)//保证登录成功退出LoginController的时候不会删除服务器和用户
		{
			delete user;
		}
	}

User AbstractController::getSelf()
{
	return user;
}

vector<string> login(const string &userName,const string &password)
{
	user=User(userName,password);
	ErrorCode errorcode=server->userlogin(user);
	if(errorcode==noError)
	{	
		return {"登录成功,您的身份是"+user["Role"],
		user["Role"]};
	}
	else
	{
		vector<string> cmd;
		commands.push_back("错误");
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
	
string ReaderController::type()
{
	return "Reader";
}

void ReaderController::infoBook()
{
	for(Book j:books)//列表内只显示名字/出版社
		{
			info.push_back(j["Name"]);
			info.push_back(j["Author"]);
			info.push_back(j["Publisher"]);
		}
}

void ReaderController::infoPracticalBook()
{
	for(Book j:practicalBooks)//列表内只显示名字/出版社
		{
			info.push_back(j["No"]);
			info.push_back(j["Index"]);
			info.push_back(j["Status"]);
		}
}

void ReaderController::infoRecord()
{

	for(Record i:records)
	{
		info.push_back(i["BookNo"]);
		info.push_back(i["Time"]);
		info.push_back(i["Type"]);
	}
}
	
Book ReaderController::getBook(int number)
{
	return books[number-1];
}

string ReaderController::searchBook()//找书,关键信息全部输入commands里
{
	info.clear();
	if(commands.size()==1)
		server->search(user,multiset<Field>{Field("Name",commands[0])},books);
	else //如果点击"高级搜索"则在commands里push一个"others",你可以给出一张页面(输入不同方法检索串的bar)
	{
		vector<string>s=Book().explicitKey();
		multiset<Field> key;
		key.clear();
		for(int i=0;i<6;i++)
		{
			if(commands[i]!="-")
				key.push_back(Fields(s[i],commands[i]));
			}
			server->search(user,key,books);
			commands.clear();
		}
		infoBook();
		int i=books.size();
		stringstream ss;ss<<i;
		return "共发现"+ss.str()+"种书";
}

string ReaderController::browseBook(const Book &book)
{
	info.clear();
	ErrorCode err=server->browseBook(user,book);
	switch(err)
	{
		//case bookNotFound:
			//return "错误:未找到书籍";
		case bookInaccessible:
			return "错误:不可查看该书籍";
		case noError:
			{
				Content *content;
				ErrorCode errorcode=server->previewBookContent(user,book,content);
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

PracticalBook ReaderController::getPracticalBook(int number)
{
	return books[number-1];
}

void ReaderController::bookToPractical(const Book &book)
{
	info.clear();
	server->search(user,multiset<Field>{Field("No",book["No"]),practicalBooks});
	infoPracticalBook();
}

string ReaderController::borrowBook(const Book &book)//点击操作
{
	bookToPractical(book);
	for(auto i:practicalBooks)
	{
		ErrorCode errorCode=server->borrowBook(user,i);
		if(errorCode==noError||errorCode==requestAlreadyProcessed)
		{
			practicalBooks.clear();
			return "已发出借阅请求";
		}
	}
	return "借阅失败";
}


Record ReaderController::getRecord(int number)
{
	return record[number-1];
}

template<class ObjType>
void ReaderController::show(const ObjType &object)//点击一下,深度显示一个对象(操作).每"显示"一个对象,就把其所有的explicitkey全部推到里面去.
{
	deepInfo.clear();//每次只展示一个对象
	vector<string> temp=object.explicitKey();
	for(string i:temp)
	{
		deepInfo.push_back(i);
		deepInfo.push_back(object[i]);
	}
}

string ReaderController::listBorrowingBooks(const User &_user)
{
	info.clear();
	practicalBooks.clear();
	vector<string> s;
	server->search(_user,multiset<Field>{Field("Username",(&_user["Username"])),Field("Status","Accepted"),},records);
	for(auto i:records)
	{
		server->search(_user,multiset<Field>{Field("No",i["BookNo"]),Field("BookIndex",i["BookIndex"])},s);
		practicalBooks.push_back(s[0]);
	}
	infoRecord();
	int j=practicalBooks.size();
	stringstream ss;ss<<j;
	return "共借阅"+ss.str()+"本书";
}

string ReaderController::returnBook(PracticalBook book)//点击操作
{
	ErrorCode errorcode=server->returnBook(user,Book)
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

string ReaderController::modifyPassword(string password1,string password2)//只有进入才能修改密码!
{
	if(password1!=password2)
		return "两次密码不一致请重输";//再次输入密码检验
	server->modifyPassword(user,Password(password1));
		return "成功修改密码";
}

string ReaderController::readRecord(const User &_user)
{
	info.clear();
	ErrorCode errorCode=server->search(_user,multiset<Field>{Field("Username",_user["Username"])},record)
	switch (errorCode)
	{
	case permissionDenied:
		return "您无权读取此用户的历史记录";
	case noError:
		infoRecord();
		int i=records.size();
		stringstream ss;
		ss<<i;
		return "共发现"+ss.str()+"条历史记录";
	}
}

//AdminController部分

string AdminController::type()
{
	return "Administrator";
}

void AdminController::infoUser()
{
	for(User i:users)
	{
		info.push_back(i["Username"]);
		info.push_back(i["Realname"]);
		info.push_back(i["School"]);
	}
}

User AdminController::getUser(int number)
{
	return users[number-1];
}

string AdminController::findUser(const string username)//通过ID或者真名查找用户(非管理员),(保证username是unique的)?
{
	info.clear();
	
	server->search(user,multiset<Field>{Field("Username",username))},users);
	infoUser();
	int j=users.size();
	stringstream ss;
	ss<<j;
	return "共发现"+ss.str()+"位用户";
}

string AdminController::registerUser()//从vector<>command里面提供材料
{
	User _user=User(commands[0],commands[1]);
	_user.update("Role",commands[2]);//commands[2]里存放身份,在client操作(前端衔接)的时候直接补
	ErrorCode errorcode=server->add(user,_user);
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

string AdminController::addBook(Book &book)//从原来的书本中加书
{
	bookToPractical(book);
	int j=practicalBooks.size();
	PracticalBook practicalBook=PracticalBook(book["No"],int2str(j+2));
	ErrorCode errorcode=server->add(user,practicalBook);
	switch(errorcode)
	{
		case invalidInfo:
			return "信息无效";
		case permissionDenied:
			return "您无权添加书籍";
		case noError:
			return "添加书籍成功";
	}
}

string AdminController::addNewBook()
{
	Book book;
	vector<string>s=book.explicitKey();
	for(int i=0;i<6;i++)
	{
		book.update(s[i],commands[i]);
	}
	server->add(book);
	addBook(book);
}

string AdminController::showPendingBook()
{	
	info.clear();
	ErrorCode err=server->search(user,multiset<Field>{Field("Status","Pending")},records);
	infoRecord();
	int i=records.size();
	stringstream ss;ss<<i;
	return "共发现"+ss.str()+"本需要处理的书";
}

string AdminController::deal(Record &record,bool accept)//管理员接受处理借书请求
{
	ErrorCode errorCode;
	if(accept==1)
		errorCode=server->acceptRequest(user,records);
	else
		errorCode=server->rejectRequest(user,records);
	switch(errorCode)
	{
		case requestNotFound:
			return "错误:未找到可操作记录";
		case requestAlreadyProcessed:
			return "错误:已经处理过该记录了";
		case noError:
			return "成功处理记录";
	
	}
}

string AdminController::editBook(Book &book)
{
	vector<string> s=book.explicitKey();//除了No之外的都可以修改
	for(int i=1;i<6;i++)
	{
		if(commands[i-1]!="-")
			book.update(s[i],commands[i-1]);
	}
	ErrorCode errorCode=server->update(user,book);
	commands.clear();//
	switch(errorCode)
	{
		case permissionDenied:
			return "您无权限修改此书籍信息";
		case noError:
			return "成功修改此书籍信息";
	}
}

string AdminController::showFreezeBook()
{
	info.clear();
	server->search(user,multiset<Field>{Field("Status","Frozen")},practicalBooks);
	infoPracticalBook();
	int j=practicalBooks.size();
	stringstream ss;ss<<j;
		return "共发现"+ss.str()+"本被冻结的书";
}

string AdminController::showFreezeUser()
{
	server->search(user,multiset<Field>{Field("Status","Frozen")},users);
	infoUser();
	int j=users.size();
	stringstream ss;ss<<j;
		return "共发现"+ss.str()+"个被冻结的用户";
}

template<class ObjType>
string AdminController::freeze(ObjType obj)//冻结书籍或者用户专用
{
	ErrorCode err=server->freeze(user,obj);
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



string AdminController::readBookRecord(const PracticalBook &prbook)
{
	info.clear();
	ErrorCode err=server->search(user,multiset<Field>{Field("BookNo",prbook["BookNo"]),
			Field("BookIndex",prbook["BookIndex"])},records)
	//switch (err)
	//{
	//case permissionDenied:
		//return "您无权读取此书的历史记录";
	case noError:
		infoRecord();
		int i=records.size();
		stringstream ss;ss<<i;
		return "共发现"+ss.str()+"条历史记录";
	//}
}


template<class ObjType>
string AdminController::unfreeze(ObjType obj)
{
	ErrorCode err=server->unfreeze(user,obj);
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

string RootController::type()
{
	return "Root";
}

template<class ObjType>
string RootController::removeObject(ObjType obj)
{
	server->remove(user,obj);
		return "移除成功";
}

string RootController::removeUser(User user)
{
	return removeObject(user);
	}

string removePracticalBook(PracticalBook practicalBook)
{
	removeObject(practicalBook);
	server->search(user,multiset<Field>{Field("No",practicalBook["No"])},practicalBooks);
	if(practicalBooks.empty())
		server->search(user,multiset<Field>{Field("No",practicalBook["No"])},books);
	remove(books[0]);
	return "移除成功";
}

//string RootController::modify()
//在其他人用引用的时候自己不要用指针,因为在这样的时候,很多操作不能用指针来做(比如很多重载)!

//=====================Gu Gu Gu!========================================================
//string AdminController::higherrecord()"需要新的问题!"
