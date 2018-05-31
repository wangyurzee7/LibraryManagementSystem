#include <bits/stdc++.h>

using namespace std;

#include "server.h"

string Server::getTime(){
	const int bufferLen=50;
	
	char buffer[bufferLen];
	time_t __t(time(0));
	strftime(buffer,sizeof(buffer),"%Y/%02m/%d %H:%M:%S",localtime(&__t));
	return buffer;
}

Server::Server(Database* ptr):db(ptr){}

template<typename ObjType>
ErrorCode Server::search(const User &currentUser,const Search& key,vector<ObjType> &ret){
	if (!key.invalidFields().empty()) return invalidKey;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	if (currentUser.typeName()=="User"){
		authority=db->isAdmin(currentUser)||currentUser["username"]==key["username"];
	}
	else if (currentUser.typeName()=="Book"){
		authority=1;
	}
	else if (currentUser.typeName()=="ParcticalBook"){
		authority=1;
	}
	else if (currentUser.typeName()=="Record"){
		authority=db->isAdmin(currentUser)||currentUser["username"]==key["username"];
	}
	
	if (authority){
		ret.clear();
		ErrorCode errorCode=db->search(key,ret);
		for (auto it=ret.begin();it!=ret.end();++it)
			if ((*it)["Status"]=="Frozen"){
				swap(*it,ret.back());
				ret.pop_back();
			}
		return errorCode;
	}
	else{
		return permissionDenied;
	}
}



template<typename ObjType>
ErrorCode Server::add(const User &currentUser,ObjType obj){
	if (!obj.invalidFields().empty()) return invalidInfo;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	if (currentUser.typeName()=="User"){
		string role=obj["Role"];
		if (role=="Reader"){
			authority=db->isAdmin(currentUser);
		}
		else if (role=="Admin"){
			authority=db->isRoot(currentUser);
		}
		else if (role=="Root"){
			authority=0;
		}
	}
	else if (currentUser.typeName()=="Book"){
		authority=db->isAdmin(currentUser);
	}
	else if (currentUser.typeName()=="ParcticalBook"){
		authority=db->isAdmin(currentUser);
	}
	else if (currentUser.typeName()=="Record"){
		authority=0;
	}
	
	if (authority){
		obj.update("Status","Accessible");
		return db->add(obj);
	}
	else{
		return permissionDenied;
	}
}



template<typename ObjType>
ErrorCode Server::update(const User &currentUser,ObjType obj){
	if (!obj.invalidFields().empty()) return invalidInfo;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	if (obj["Status"]!="") authority=db->isRoot(currentUser);
	else if (currentUser.typeName()=="User"){
		if (currentUser["username"]==obj["username"])
			authority=1;
		else{
			string role=obj["Role"];
			if (role=="Reader"){
				authority=db->isAdmin(currentUser);
			}
			else if (role=="Admin"){
				authority=db->isRoot(currentUser);
			}
			else if (role=="Root"){
				authority=0;
			}
		}
	}
	if (currentUser.typeName()=="Book"){
		authority=db->isAdmin(currentUser);
	}
	if (currentUser.typeName()=="ParcticalBook"){
		authority=db->isAdmin(currentUser);
	}
	if (currentUser.typeName()=="Record"){
		authority=db->isRoot(currentUser);
	}
	
	if (authority){
		return db->update(obj);
	}
	else{
		return permissionDenied;
	}
}

template<typename ObjType>
ErrorCode Server::remove(const User &currentUser,const ObjType &obj){
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=db->isRoot(currentUser);
	
	if (authority){
		return db->update(obj);
	}
	else{
		return permissionDenied;
	}
}


ErrorCode Server::userRegister(User user){
	if (!user.invalidFields().empty()) return invalidInfo;
	
	if (user["Role"]!="Reader") return permissionDenied;
	
	user.update("Status","Accessible");
	
	return db->add(user);
}


ErrorCode Server::userLogin(User user){
	if (!db->findOne(user)) return noSuchUser;
	if (!db->userExist(user)) return wrongPassword;
	if (user["Status"]!="Accessible") return loginFailed;
	
	return noError;
}

ErrorCode Server::modifyPassword(User user,Password newPwd){
	if (!db->findOne(user)) return userNotFound;
	if (!db->userExist(user)) return wrongPassword;
	if (user["Status"]!="Accessible") return loginFailed;
	
	return db->modifyPassword(user,newPwd);
}


ErrorCode Server::borrowBook(const User& currentUser,const PracticalBook& book){
	PracticalBook res=book;
	if (!db->findOne(res)) return bookNotFound;
	
	if (res["Status"]!="Accessible") return bookInaccessible;
	
	return db->update(Record({
		Field("Id",db->newRecordId()),
		Field("Username",currentUser["Username"]),
		Field("BookNo",res["No"]),
		Field("BookIndex",res["Index"]),
		Field("Status","Pending"),
		Field("Time",getTime()),
		Field("Type","Borrowing")
	}));
}
ErrorCode Server::returnBook(const User& currentUser,const PracticalBook& book){
	PracticalBook res=book;
	if (!db->findOne(res)) return bookNotFound;
	
	if (res["Status"]!="Borrowed") return bookNotBorrowed;
	
	if (!db->objectExist(Record({
		Field("Username",currentUser["Username"]),
		Field("BookNo",book["No"]),
		Field("BookIndex",book["Index"]),
		Field("Status","Pending"),
		Field("Type","Returning")
	})))
		return requestAlreadySubmitted;
	
	if (!db->objectExist(Record({
		Field("Username",currentUser["Username"]),
		Field("BookNo",book["No"]),
		Field("BookIndex",book["Index"]),
		Field("Status","Accepted"),
		Field("Type","Borrowing")
	})))
		return bookNotBorrowedByCurrentUser;
	
	return db->update(Record({
		Field("Id",db->newRecordId()),
		Field("Username",currentUser["Username"]),
		Field("BookNo",res["No"]),
		Field("BookIndex",res["Index"]),
		Field("Status","Pending"),
		Field("Time",getTime()),
		Field("Type","Returning")
	}));
}


ErrorCode Server::acceptRequest(const User& currentUser,const Record& record){
	Record res=record;
	if (!db->findOne(res)) return requestNotFound;
	if (res["Type"]!="Borrowing"&&res["Type"]!="Returning") return requestNotFound;
	
	if (res["Status"]!="Pending") return requestAlreadyProcessed;
	
	bool flag=res["Type"]=="Borrowing";
	
	if (!db->objectExist(PracticalBook({
		Field("No",res["BookNo"]),
		Field("Index",res["BookIndex"]),
		Field("Status",flag?"Accessible":"Borrowed")
	})))
		return flag?bookInaccessible:bookNotBorrowed;
	
	ErrorCode ret=db->update(PracticalBook({
		Field("No",res["BookNo"]),
		Field("Index",res["BookIndex"]),
		Field("Status",flag?"Borrowed":"Accessible")
	}));
	if (ret!=0) return ret;
	if (!flag){
		vector<Record> recs;
		ret=db->search<Record>(CompleteMatchingSearch({
			Field("Username",res["Username"]),
			Field("BookNo",res["BookNo"]),
			Field("BookIndex",res["BookIndex"]),
			Field("Type","Borrowing"),
			Field("Status","Accepted")
		}),recs);
		if (ret!=0) return ret;
		if (int(recs.size())!=1) return unknownError;
		string tmpId=(*recs.begin())["Id"];
		ret=db->update(Record(set<Field>({
			Field("Id",tmpId),
			Field("Status","Returned")
		})));
		if (ret!=0) return ret;
	}
	return db->update(Record(set<Field>({
		Field("Id",res["Id"]),
		Field("Status","Accepted"),
		Field("Remarks","Processing Time : "+getTime())
	})));
}

ErrorCode Server::rejectRequest(const User& currentUser,const Record& record){
	Record res=record;
	if (!db->findOne(res)) return requestNotFound;
	if (res["Type"]!="Borrowing"&&res["Type"]!="Returning") return requestNotFound;
	
	if (res["Status"]!="Pending") return requestAlreadyProcessed;
	
	bool flag=res["Type"]=="Borrowing";
	
	return db->update(Record(set<Field>({
		Field("Id",res["Id"]),
		Field("Status","Rejected"),
		Field("Remarks","Processing Time : "+getTime())
	})));
}


template<typename ObjType>
ErrorCode Server::freeze(const User &currentUser,ObjType obj){
	if (!db->findOne(obj)) return objectNotFound;
	if (obj["Status"]!="Accessible") return objectNotAccessible;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	if (currentUser.typeName()=="User"){
		string role=obj["Role"];
		if (role=="Reader"){
			authority=db->isAdmin(currentUser);
		}
		else if (role=="Admin"){
			authority=db->isRoot(currentUser);
		}
		else if (role=="Root"){
			authority=0;
		}
	}
	else if (currentUser.typeName()=="Book"){
		authority=db->isAdmin(currentUser);
	}
	else if (currentUser.typeName()=="ParcticalBook"){
		authority=db->isAdmin(currentUser);
	}
	else if (currentUser.typeName()=="Record"){
		authority=0;
	}
	
	if (authority){
		obj.update("Status","Frozen");
		return db->update(obj);
	}
	else{
		return permissionDenied;
	}
}

template<typename ObjType>
ErrorCode Server::unfreeze(const User &currentUser,ObjType obj){
	if (!db->findOne(obj)) return objectNotFound;
	if (obj["Status"]!="Frozen") return objectNotAccessible;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	if (currentUser.typeName()=="User"){
		string role=obj["Role"];
		if (role=="Reader"){
			authority=db->isAdmin(currentUser);
		}
		else if (role=="Admin"){
			authority=db->isRoot(currentUser);
		}
		else if (role=="Root"){
			authority=0;
		}
	}
	else if (currentUser.typeName()=="Book"){
		authority=db->isAdmin(currentUser);
	}
	else if (currentUser.typeName()=="ParcticalBook"){
		authority=db->isAdmin(currentUser);
	}
	else if (currentUser.typeName()=="Record"){
		authority=0;
	}
	
	if (authority){
		obj.update("Status","Accessible");
		return db->update(obj);
	}
	else{
		return permissionDenied;
	}
}



ErrorCode Server::browseBook(const User& currentUser,Book& book){
	if (!db->findOne(book)) return bookNotFound;
	
	if (book["Status"]!="Accessible") return bookInaccessible;
	
	return db->update(Record({
		Field("Id",db->newRecordId()),
		Field("Username",currentUser["Username"]),
		Field("BookNo",book["No"]),
		Field("Status","Meowed"),
		Field("Time",getTime()),
		Field("Type","Browsing")
	}));
}

ErrorCode Server::previewBookContent(const User& currentUser,Book book,Content *ret){
	if (!db->findOne(book)) return bookNotFound;
	
	if (book["Status"]!="Accessible") return bookInaccessible;
	
	db->update(Record({
		Field("Id",db->newRecordId()),
		Field("Username",currentUser["Username"]),
		Field("BookNo",book["No"]),
		Field("Status","Meowed"),
		Field("Time",getTime()),
		Field("Type","Browsing")
	}));
	
	string url=book["Url"];
	string suffix=url.substr(url.find_last_of(".")+1);
	if (suffix==url) suffix="";
	bool found=0;
	if (suffix=="txt"){
		ret=new TxtContent(url,found);
	}
	else{
		return unknownContentSuffix;
	}
	return found?noError:bookContentMissing;
}


//==============Gu Gu Gu!====================
ErrorCode searchInContent(const User &currentUser,const Book &book,const string &keyWord){
	return guGuGu;
}