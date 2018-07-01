#include <bits/stdc++.h>

using namespace std;

#include "txtcontent.h"

TxtContent::TxtContent():content(""){}
TxtContent::TxtContent(const string &_content):content(_content){}
TxtContent::TxtContent(const string &url,bool &found){
	ifstream in(url);
	content="";
	if (in){
		found=1;
		string buffer;
		while (getline(in,buffer))
			content+=buffer+'\n';
	}
	else{
		found=0;
	}
}
string TxtContent::show() const{
	return content;
}