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
	loginAgain,
	
	bookInaccessible,
	bookNotBorrowed,
	bookNotBorrowedByCurrentUser,
	
	requestAlreadySubmitted,
	requestAlreadyProcessed,
	
	objectNotFrozen,
	objectAlreadyFrozen,
	
	previewFileDoesNotFound,
	
	notYetOpen,
	
	unknownError
};