#pragma once
enum ErrorCode{
	noError=0,
	
	objectNotFound,
	typeError,
	
	permissionDenied,
	needRoot,
	
	objectExists,
	userExists,
	invalidInfo,
	invalidKey,
	
	loginFailed,
	loginAgain,
	
	bookAlreadyBorrowed,
	bookAlreadyReturned,
	
	requestAlreadyProcessed,
	
	objectNotFrozen,
	objectAlreadyFrozen,
	
	previewFileDoesNotFound,
	
	notYetOpen,
	
	unknownError
};