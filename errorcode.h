#pragma once
enum ErrorCode{
	noError=0,
	
	objectNotFound,
	bookNotFound,
	typeError,
	
	permissionDenied,
	needRoot,
	
	objectExists,
	userExists,
	invalidInfo,
	invalidKey,
	
	loginFailed,
	noSuchUser,
	wrongPassword,
	loginAgain,
	
	bookInaccessible,
	bookNotBorrowed,
	bookNotBorrowedByCurrentUser,
	
	requestAlreadySubmitted,
	requestAlreadyProcessed,
	requestNotFound,
	
	objectNotFrozen,
	objectNotAccessible,
	
	bookContentMissing,
	unknownContentSuffix,
	
	guGuGu,
	
	unknownError
};