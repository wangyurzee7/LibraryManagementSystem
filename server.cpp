#include <bits/stdc++.h>

using namespace std;

#include "server.h"


Server::Server(Database* ptr):db(ptr){}

template<typename ObjType>
ErrorCode Server::search(const User &currentUser,const Search& key,vector<ObjType> &ret){
	if (!key.invalidFields().empty()) return invalidKey;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	switch (currentUser.typeName()){
		case "User":
			authority=db->isAdmin(currentUser)||currentUser["username"]==key["username"];
		case "Book":
			authority=1;
		case "ParcticalBook":
			authority=1;
		case "Record":
			authority=db->isAdmin(currentUser);
	}
	
	if (authority){
		ret.clear();
		return db->search(key,ret);
	}
	else{
		return permissionDenied;
	}
}



template<typename ObjType>
ErrorCode Server::add(const User &currentUser,const ObjType &obj){
	if (!obj.invalidFields().empty()) return invalidInfo;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	switch (currentUser.typeName()){
		case "User":
			switch(obj["Role"]){
				case "Reader":
					authority=db->isAdmin(currentUser);
				case "Admin":
					authority=db->isRoot(currentUser);
				case "Root":
					authority=0;
			}
		case "Book":
			authority=db->isAdmin(currentUser);
		case "ParcticalBook":
			authority=db->isAdmin(currentUser);
		case "Record":
			authority=db->isRoot(currentUser);
	}
	
	if (authority){
		return db->add(obj);
	}
	else{
		return permissionDenied;
	}
}



template<typename ObjType>
ErrorCode Server::update(const User &currentUser,const ObjType &obj){
	if (!obj.invalidFields().empty()) return invalidInfo;
	
	if (!db->userExist(currentUser)) return loginAgain;
	
	bool authority=0;
	switch (currentUser.typeName()){
		case "User":
			if (currentUser["username"]==obj["username"])
				authority=1;
			else switch(obj["Role"]){
				case "Reader":
					authority=db->isAdmin(currentUser);
				case "Admin":
					authority=db->isRoot(currentUser);
				case "Root":
					authority=0;
			}
		case "Book":
			authority=db->isAdmin(currentUser);
		case "ParcticalBook":
			authority=db->isAdmin(currentUser);
		case "Record":
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


ErrorCode Server::userRegister(const User& user){
	if (!user.invalidFields().empty()) return invalidInfo;
	
	if (user["Role"]!="Reader") return permissionDenied;
	
	return db->add(user);
}


ErrorCode Server::userLogin(const User& user){
	if (!db->userExist(user)) return loginFailed;
	
	return noError;
}

ErrorCode Server::borrowBook(const User& currentUser,const PracticalBook& book){
	
}
// ErrorCode returnBook(const User& currentUser,const PracticalBook& book);

/*
ErrorCode acceptRequest(const User& currentUser,const Record& record);
ErrorCode rejectRequest(const User& currentUser,const Record& record);

template<typename ObjType>
ErrorCode freeze(const User &currentUser,const ObjType &obj);
template<typename ObjType>
ErrorCode unfreeze(const User &currentUser,const ObjType &obj);

ErrorCode browseBook(const User& currentUser,const PracticalBook& book);

ErrorCode previewBookContent(const User& currentUser,const PracticalBook& book,Content &ret);

*/


//==============Gu Gu Gu!====================
ErrorCode searchInContent(const User &currentUser,const Book &book,const string &keyWord){
	return notYetOpen;
}