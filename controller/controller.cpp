#include <bits/stdc++.h>
#include "controller.h"
using namespace std;

User AbstractController::getSelf()
{
	return user;
}

vector<string> LoginController::login(const string &userName,const string &password)
{
	user=User(userName,password);
	ErrorCode errorCode=server->userLogin(user);
	if(errorCode==noError)
	{	
		vector<string> cmds;
		cmds.push_back("登录成功,您的角色是"+user["Role"]);
		cmds.push_back(user["Role"]);
		return cmds;
	}
	else
	{
		vector<string> cmds;
		cmds.push_back("错误");
		switch(errorCode){
		case noSuchUser:
			cmds.push_back("并没有这个用户");
		case wrongPassword:
			cmds.push_back("密码错误");
		case loginFailed:
			cmds.push_back("用户被冻结");
			}
		return cmds;
		}
	}
	
string ReaderController::type()
{
	return "Reader";
}

void ReaderController::infoBook()
{
	for(Book j:books)
		{
			info.push_back(j["Name"]);
			info.push_back(j["Author"]);
			info.push_back(j["Publisher"]);
		}
}

void ReaderController::infoPracticalBook()
{
	for(PracticalBook j:practicalBooks)
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

string ReaderController::searchBook()
{
	info.clear();
	if(commands.size()==1)
	{	ReSearch key=ReSearch(multiset<Field>{Field("Name",commands[0])});
		server->search(user,key,books);}
	else //你可以给出一张页面(输入不同方法检索串的bar)
	{
		vector<string>s=Book().explicitKey();
		multiset<Field> searchKey;
		for(int i=0;i<6;i++)
		{
			if(commands[i]!="-")
				searchKey.insert(searchKey.end(),Field(s[i],commands[i]));
			}
			ReSearch key=ReSearch(searchKey);
			server->search(user,key,books);
			commands.clear();
		}
		infoBook();
		int i=books.size();
		stringstream ss;ss<<i;
		return "共发现"+ss.str()+"种书";
}

string ReaderController::browseBook(Book &book)
{
	info.clear();
	ErrorCode err=server->browseBook(user,book);
	switch(err)
	{
		case bookNotFound:
			return "错误:未找到书籍";
		case bookInaccessible:
			return "错误:不可查看该书籍";
		case noError:
			{
				Content *content;
				ErrorCode errorCode=server->previewBookContent(user,book,content);
				switch(errorCode)
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
	return practicalBooks[number-1];
}

void ReaderController::bookToPractical(const Book &book)
{
	info.clear();
	CompleteMatchingSearch key=CompleteMatchingSearch(multiset<Field>{Field("No",book["No"])});
	server->search(user,key,practicalBooks);
	infoPracticalBook();
}

string ReaderController::borrowBook(const Book &book)
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
	return records[number-1];
}

template<class ObjType>
void ReaderController::show(const ObjType &object)
{
	deepInfo.clear();
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
	CompleteMatchingSearch key=CompleteMatchingSearch(multiset<Field>{Field("Username",(_user["Username"])),Field("Status","Accepted")});
	server->search(_user,key,records);
	for(auto i:records)
	{	CompleteMatchingSearch practicalBookKey=CompleteMatchingSearch(multiset<Field>{Field("No",i["BookNo"]),Field("BookIndex",i["BookIndex"])});
		server->search(_user,practicalBookKey,s);
		practicalBooks.push_back(s[0]);
	}
	infoRecord();
	int j=practicalBooks.size();
	stringstream ss;ss<<j;
	return "共借阅"+ss.str()+"本书";
}

string ReaderController::returnBook(PracticalBook book)
{
	ErrorCode errorcode=server->returnBook(user,book);
	switch(errorcode)
	{
		
		case bookNotBorrowed:
			return "错误,该书籍未被借阅";
		default:
			return "已发出归还请求";
	}
}

string ReaderController::modifyPassword(string password1,string password2)
{
	if(password1!=password2)
		return "两次密码不一致请重输";
	server->modifyPassword(user,Password(password1));
		return "成功修改密码";
}

string ReaderController::readRecord(User _user)
{
	info.clear();
	CompleteMatchingSearch key=CompleteMatchingSearch(multiset<Field>{Field("Username",_user["Username"])});
	ErrorCode errorCode=server->search(_user,key,records);
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

string AdminController::findUser(const string &username)
{
	info.clear();
	CompleteMatchingSearch key=CompleteMatchingSearch(multiset<Field>{Field("Username",username)});
	server->search(user,key,users);
	infoUser();
	int j=users.size();
	stringstream ss;
	ss<<j;
	return "共发现"+ss.str()+"位用户";
}

string AdminController::registerUser(const string &username,const string &password,string identity="Reader")
{
	User _user=User(username,password);
	if(identity!="Root"||identity!="Administrator")
		identity="Reader";
	_user.update("Role",identity);//commands[2]里存放身份,在client操作(前端衔接)的时候直接补(如果command2没有操作或输入错误的话,那么默认是reader)
	ErrorCode errorcode=server->add(user,_user);
	switch(errorcode){
		case invalidInfo:
			return "信息无效";
		case permissionDenied:
			return "您无权创建此用户";
		case noError:
			return "创建用户成功";
	}
}

string AdminController::addBook(Book &book)
{
	bookToPractical(book);
	int j=practicalBooks.size();
	stringstream ss; ss<<j+2;
	PracticalBook practicalBook=PracticalBook(book["No"],ss.str());
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
	server->add(user,book);
	addBook(book);
}

string AdminController::showPendingBook()
{	
	info.clear();
	ErrorCode err=server->search(user,CompleteMatchingSearch(multiset<Field>{Field("Status","Pending")}),records);
	infoRecord();
	int i=records.size();
	stringstream ss;ss<<i;
	return "共发现"+ss.str()+"本需要处理的书";
}

string AdminController::deal(Record record,bool accept)
{
	ErrorCode errorCode;
	if(accept==1)
		errorCode=server->acceptRequest(user,record);
	else
		errorCode=server->rejectRequest(user,record);
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

string AdminController::editBook(Book book)
{
	vector<string> s=book.explicitKey();
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
	server->search(user,CompleteMatchingSearch(multiset<Field>{Field("Status","Frozen")}),practicalBooks);
	infoPracticalBook();
	int j=practicalBooks.size();
	stringstream ss;ss<<j;
		return "共发现"+ss.str()+"本被冻结的书";
}

string AdminController::showFreezeUser()
{
	server->search(user,CompleteMatchingSearch(multiset<Field>{Field("Status","Frozen")}),users);
	infoUser();
	int j=users.size();
	stringstream ss;ss<<j;
		return "共发现"+ss.str()+"个被冻结的用户";
}

template<class ObjType>
string AdminController::freeze(ObjType obj)
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

string AdminController::readBookRecord(const PracticalBook &practicalBook)
{
	info.clear();
	ErrorCode err=server->search(user,CompleteMatchingSearch(multiset<Field>{Field("BookNo",practicalBook["No"]),Field("BookIndex",practicalBook["Index"])}),records);
	switch (err)
	{
	case permissionDenied:
		return "您无权读取此书的历史记录";
	case noError:
		infoRecord();
		int i=records.size();
		stringstream ss;ss<<i;
		return "共发现"+ss.str()+"条历史记录";
	}
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
	ErrorCode errorCode=server->remove(user,obj);
	switch(errorCode)
	{
		case permissionDenied:
			return "删除失败,您没有root权限";
		case loginAgain:
			return "您已掉线,请重新登录";
		case noError:
			return "成功删除";
	}
}

string RootController::removeUser(User user)
{
	return removeObject(user);
	}

string RootController::removePracticalBook(PracticalBook practicalBook)
{
	string result=removeObject(practicalBook);
	if(result!="成功删除")
		return result;
	else
	{
	server->search(user,CompleteMatchingSearch(multiset<Field>{Field("No",practicalBook["No"])}),practicalBooks);
	if(practicalBooks.empty())
		server->search(user,CompleteMatchingSearch(multiset<Field>{Field("No",practicalBook["No"])}),books);
	return removeObject(books[0]);
	}
}

