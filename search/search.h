#include <bits/stdc++.h>

#include "../object/abstractobject.h"

using namespace std;

#pragma once
class Search:public AbstractObject{
public:
	using AbstractObject::AbstractObject;
	virtual string typeName() const override final;
};