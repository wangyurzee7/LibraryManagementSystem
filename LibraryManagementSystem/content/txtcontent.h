#include <bits/stdc++.h>

using namespace std;

#include "content.h"

#pragma once
class TxtContent:public Content{
private:
	string content;
public:
	TxtContent();
	TxtContent(const string &_content);
	TxtContent(const string &url,bool &found);
	virtual string show() const override;
};