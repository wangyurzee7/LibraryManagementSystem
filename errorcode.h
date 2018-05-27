#pragma once
enum ErrorCode{
	succeeded=0,
	
	objectNotFound,
	typeError,
	
	permissionDenied,
	needRoot,
	
	userExists,
	InvalidInfo,
	
	noSuchUser,
	wrongPassowrd,
	needToConfirmPassword,
	
	bookAlreadyBorrowed,
	bookAlreadyReturned,
	
	requestAlreadyProcessed,
	
	objectNotFrozen,
	objectAlreadyFrozen,
	
	previewFileDoesNotFound,
	
	unknownError
};