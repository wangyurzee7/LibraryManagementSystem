#include <bits/stdc++.h>

#include "database.h"
#include "object/object.h"
#include "object/user.h"
#include "object/book.h"
#include "object/practicalbook.h"
#include "object/record.h"

#include "object/search.h"

#include "content/content.h"
#include "content/txtcontent.h"

#include "errorcode.h"


using namespace std;

#pragma once
class Server{
private:
	Database* db;
	string getTime();
public:
	#ifdef TEST
	void test(){cout<<"Server address = "<<this<<endl;db->test();}
	#endif
	Server(Database* ptr);
	template<typename ObjType>
	ErrorCode search(const User &currentUser,const Search& key,vector<ObjType> &ret);

	ErrorCode searchInContent(const User &currentUser,const Book &book,const string &keyWord);
	
	template<typename ObjType>
	ErrorCode add(const User &currentUser,ObjType obj);
	
	template<typename ObjType>
	ErrorCode update(const User &currentUser,ObjType obj);
	template<typename ObjType>
	ErrorCode remove(const User &currentUser,const ObjType &obj);
	
	// ErrorCode userRegister(User user);
	ErrorCode userLogin(User &user);
	ErrorCode modifyPassword(User user,Password newPwd);

	// ErrorCode bookToPracticalBook(const User &currentUser,const Book &book);

	ErrorCode borrowBook(const User& currentUser,const PracticalBook& book);
	ErrorCode returnBook(const User& currentUser,const PracticalBook& book);
	
	ErrorCode acceptRequest(const User& currentUser,const Record& record);
	ErrorCode rejectRequest(const User& currentUser,const Record& record);
	
	// ErrorCode countBook(const User& currentUser,const Book& book,int& cnt);
	// ErrorCode countAccessableBook(const User& currentUser,const Book& book,int& cnt);
	
	// Only for User/Book/PracticalBook
	template<typename ObjType>
	ErrorCode freeze(const User &currentUser,ObjType obj);
	template<typename ObjType>
	ErrorCode unfreeze(const User &currentUser,ObjType obj);
	
	ErrorCode browseBook(const User& currentUser,Book& book);
	
	ErrorCode previewBookContent(const User& currentUser,Book book,Content *ret);
};


template<typename ObjType>
ErrorCode Server::search(const User &currentUser,const Search& key,vector<ObjType> &ret){
	if (!key.invalidFields().empty()) return invalidKey;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	string objType=ObjType().typeName();
	if (objType=="User"){
		authority=db->isAdmin(currentUser)||currentUser["username"]==key["username"];
	}
	else if (objType=="Book"){
		authority=1;
	}
	else if (objType=="PracticalBook"){
		authority=1;
	}
	else if (objType=="Record"){
		authority=db->isAdmin(currentUser)||currentUser["username"]==key["username"];
	}
	
	if (authority){
		ret.clear();
		ErrorCode errorCode=db->search(key,ret);
		if (key["Status"]==""){
			for (auto it=ret.begin();it!=ret.end();++it)
				if ((*it)["Status"]=="Frozen"){
					swap(*it,ret.back());
					ret.pop_back();
				}
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
	string objType=ObjType().typeName();
	if (objType=="User"){
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
	else if (objType=="Book"){
		authority=db->isAdmin(currentUser);
	}
	else if (objType=="PracticalBook"){
		authority=db->isAdmin(currentUser);
	}
	else if (objType=="Record"){
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
	string objType=ObjType().typeName();
	if (obj["Status"]!="") authority=db->isRoot(currentUser);
	else if (objType=="User"){
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
	if (objType=="Book"){
		authority=db->isAdmin(currentUser);
	}
	if (objType=="PracticalBook"){
		authority=db->isAdmin(currentUser);
	}
	if (objType=="Record"){
	
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


template<typename ObjType>
ErrorCode Server::freeze(const User &currentUser,ObjType obj){
	if (!db->findOne(obj)) return objectNotFound;
	if (obj["Status"]!="Accessible") return objectNotAccessible;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	string objType=ObjType().typeName();
	if (objType=="User"){
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
	else if (objType=="Book"){
		authority=db->isAdmin(currentUser);
	}
	else if (objType=="ParcticalBook"){
		authority=db->isAdmin(currentUser);
	}
	else if (objType=="Record"){
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
	string objType=ObjType().typeName();
	if (objType=="User"){
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
	else if (objType=="Book"){
		authority=db->isAdmin(currentUser);
	}
	else if (objType=="ParcticalBook"){
		authority=db->isAdmin(currentUser);
	}
	else if (objType=="Record"){
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
