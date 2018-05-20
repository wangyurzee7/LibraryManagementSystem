#pragma once
#include <bits/stdc++.h>

#include "encryptor.h"

using namespace std;

class Password{
	static Encryptor* encryptor;
	string ciphertext;
public:
	Password();
	Password(const string&);
	string toString(); // Caution!!! (Don't XJB use it! )
	friend istream& operator >>(istream&,Password&);
	friend ostream& operator <<(istream&,const Password&);
};