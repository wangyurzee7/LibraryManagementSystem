#pragma once
enum ErrorCode{
	noError=0,
	
	objectNotFound,
	typeError,
	
	permissionDenied,
	needRoot,
	
	userExists,
	InvalidInfo,
	
	noSuchUser,
	wrongPassowrd,
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