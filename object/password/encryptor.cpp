#include <bits/stdc++.h>

#include "md5.h"

using namespace std;

#include "encryptor.h"


string EncryptorMD5::encrypt(const string& st){
	return MD5(st).toString();
}