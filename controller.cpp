#include <bits/stdc++.h>
#include "controller.h"
#include "object/user.h"
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
	return user["Role"];
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
	int i=books.size();
	if(number<=i&&number>0)
		return books[number-1];
	else
		return Book();
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
			break;
		case bookInaccessible:
			return "错误:不可查看该书籍";
			break;
		case noError:
			{
				Content *content;
				ErrorCode errorCode=server->previewBookContent(user,book,content);
				switch(errorCode)
				{
					case unknownContentSuffix:
						return "错误:格式不支持";
						break;
					case bookContentMissing:
						return "错误:内容丢失";
						break;
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
	int i=practicalBooks.size();
	if(number<=i&&number>0)
		return practicalBooks[number-1];
	else
		return PracticalBook();
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
	if(!practicalBooks.empty())
	{
		for(PracticalBook i:practicalBooks)
		{
			ErrorCode errorCode=server->borrowBook(user,i);
			if(errorCode==noError||errorCode==requestAlreadyProcessed)
			{
				practicalBooks.clear();
				return "已发出借阅请求";
			}
		}
	}
	return "借阅失败";
}


Record ReaderController::getRecord(int number)
{
	int i=records.size();
	if(number<=i&&number>0)
		return records[number-1];
	else
		return Record();
}

string ReaderController::listBorrowingBooks(const User &_user)
{
	//cout<<"in"<<endl;
	info.clear();
	practicalBooks.clear();
	vector<PracticalBook> s;
	CompleteMatchingSearch key=CompleteMatchingSearch(multiset<Field>{Field("Username",(_user["Username"])),Field("Status","Accepted"),Field("Type","Borrowing")});
	//cout<<"that"<<endl;
	server->search(_user,key,records);
	//cout<<"aftersearch"<<records.size()<<endl;
	for(auto i:records)
	{	CompleteMatchingSearch practicalBookKey=CompleteMatchingSearch(multiset<Field>{Field("No",i["BookNo"]),Field("Index",i["BookIndex"])});
		server->search(_user,practicalBookKey,s);
		{
			if(s[0]["Status"]!="Borrowed")
				practicalBooks.push_back(s[0]);
			}
	}
	//cout<<"afterpush"<<endl;
	infoPracticalBook();
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
			break;
		default:
			return "已发出归还请求";
	}
}

string ReaderController::modifyPassword(string password1,string password2)
{
	ErrorCode errorCode=server->modifyPassword(user,Password(password2));
	switch(errorCode)
	{
		case noError:
			return "成功修改密码";
			break;
		case wrongPassword:
			return "密码输入错误";
			break;
		default:
			return "密码修改失败";
	}
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
		break;
	case noError:
		infoRecord();
		int i=records.size();
		stringstream ss;
		ss<<i;
		return "共发现"+ss.str()+"条历史记录";
	}
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
	int i=users.size();
	if(number<=i&&number>0)
		return users[number-1];
	else
		return User();
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
	if(identity!="Root"&&identity!="Admin")
		identity="Reader";
	_user.update("Role",identity);
	ErrorCode errorcode=server->add(user,_user);
	switch(errorcode){
		case objectExists:
			return "用户名已被占用";
			break;
		case invalidInfo:
			return "信息无效";
			break;
		case permissionDenied:
			return "您无权创建此用户";
			break;
		case noError:
			return "创建用户成功";
	}
}

string AdminController::addBook(Book &book)
{
	bookToPractical(book);
	int j=0;
	if(!practicalBooks.empty())
	{
	for (PracticalBook i :practicalBooks)
		j=max(stoi(i["Index"]),j);
		}
//	cout<<j<<endl;
	stringstream ss; ss<<j+1;
	PracticalBook practicalBook=PracticalBook(book["No"],ss.str());
	practicalBook.update("Status","Accessible");
	practicalBook.update("Remarks",book["Remarks"]);
	ErrorCode errorcode=server->add(user,practicalBook);
	switch(errorcode)
	{
		case invalidInfo:
			return "信息无效";
		case permissionDenied:
			return "您无权添加书籍";
		case noError:
			return "添加书籍成功";

		case objectExists:
			return "该书籍已经存在";

		default:
			return "添加书籍失败";
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
	book.update("Status","Accessible");
	ErrorCode errorCode=server->add(user,book);
	switch(errorCode){
		case invalidInfo:
			return "信息错误";
		case loginAgain:
			return "您已掉线";
		case noError:
			return addBook(book);
		default:
			return "添加失败";
	}
	
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
			break;
		case requestAlreadyProcessed:
			return "错误:已经处理过该记录了";
			break;
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
		{	
			//cout<< s[i]<<" "<<commands[i-1]<<endl;
			book.update(s[i],commands[i-1]);
			//cout << book[s[i]]<<endl;
			}
	}
	ErrorCode errorCode=server->update(user,book);
	commands.clear();
	switch(errorCode)
	{
		case permissionDenied:
			return "您无权限修改此书籍信息";
			break;
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
	info.clear();
	server->search(user,CompleteMatchingSearch(multiset<Field>{Field("Status","Frozen")}),users);
	infoUser();
	int j=users.size();
	stringstream ss;ss<<j;
		return "共发现"+ss.str()+"个被冻结的用户";
}


string AdminController::readBookRecord(const PracticalBook &practicalBook)
{
	info.clear();
	ErrorCode err=server->search(user,CompleteMatchingSearch(multiset<Field>{Field("BookNo",practicalBook["No"]),Field("BookIndex",practicalBook["Index"])}),records);
	switch (err)
	{
	case permissionDenied:
		return "您无权读取此书的历史记录";
		break;
	case noError:
		infoRecord();
		int i=records.size();
		stringstream ss;ss<<i;
		return "共发现"+ss.str()+"条历史记录";
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
	{
		server->search(user,CompleteMatchingSearch(multiset<Field>{Field("No",practicalBook["No"])}),books);
		return removeObject(books[0]);
		}
	return result;
	}
}

