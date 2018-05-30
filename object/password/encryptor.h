#pragma once
#include <bits/stdc++.h>

using namespace std;


class Encryptor{
public:
	virtual string encrypt(const string&)=0;
};

class EncryptorMD5: public Encryptor{
public:
	string encrypt(const string&) override;
};

