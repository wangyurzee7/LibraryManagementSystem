#include <bits/stdc++.h>

#include "object/object.h"
#include "object/user.h"
#include "object/book.h"
#include "object/practicalbook.h"
#include "object/record.h"

#include "search/search.h"
#include "search/completematching.h"
#include "search/fuzzymatching.h"
#include "search/re.h"

#include "content/content.h"
#include "content/txtcontent.h"

#include "database.h"

#include "errorcode.h"


using namespace std;

#pragma once
class Server{
private:
	Database* db;
public:
	Server(Database* ptr);
	template<typename ObjType>
	ErrorCode search(const User &currentUser,multiset<Field> fields,vector<ObjType> &ret,SearchStrategy strategy=completeMatching);
	template<typename ObjType>
	ErrorCode search(const User &currentUser,Field field,vector<ObjType> &ret,SearchStrategy strategy=completeMatching);

	ErrorCode searchInContent(const User &currentUser,const Book &book,const string &keyWord);
	
	template<typename ObjType>
	ErrorCode add(const User &currentUser,const ObjType &obj);
	
	template<typename ObjType>
	ErrorCode update(const User &currentUser,const ObjType &obj);
	template<typename ObjType>
	ErrorCode remove(const User &currentUser,const ObjType &obj);
	
	ErrorCode userRegister(const User& user);
	ErrorCode userLogin(const User& user);

	ErrorCode borrowBook(const User& currentUser,const PracticalBook& book);
	ErrorCode returnBook(const User& currentUser,const PracticalBook& book);
	
	ErrorCode acceptRequest(const User& currentUser,const Record& record);
	ErrorCode rejectRequest(const User& currentUser,const Record& record);
	
	// ErrorCode countBook(const User& currentUser,const Book& book,int& cnt);
	// ErrorCode countAccessableBook(const User& currentUser,const Book& book,int& cnt);
	
	// Only for User/Book/PracticalBook
	template<typename ObjType>
	ErrorCode freeze(const User &currentUser,const ObjType &obj);
	template<typename ObjType>
	ErrorCode unfreeze(const User &currentUser,const ObjType &obj);
	
	ErrorCode browseBook(const User& currentUser,const PracticalBook& book);
	
	ErrorCode previewBookContent(const User& currentUser,const PracticalBook& book,Content &ret);
};