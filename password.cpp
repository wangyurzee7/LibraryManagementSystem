#include <bits/stdc++.h>

#include "encryptor.h"

using namespace std;

#include "password.h"

Encryptor* Password::encryptor=new EncryptorMD5(); // MD5 encryption is used by default.

Password::Password():ciphertext(encryptor->encrypt("")){}
Password::Password(const string &pwd):ciphertext(encryptor->encrypt(pwd)){}

// Return the ciphertext of password.
string Password::toString(){
	return ciphertext;
}

istream& operator >>(istream &in,Password &dst){
	string pwd;
	istream& ret=(in>>pwd);
	dst.ciphertext=dst.encryptor->encrypt(pwd);
	return ret;
}
ostream& operator <<(istream &out,const Password &src){
	ostream& ret=(out<<src.ciphertext);
	return ret;
}