#include <bits/stdc++.h>

#include "../object/abstractobject.h"

using namespace std;


#pragma once
class Search:public AbstractObject<multiset<Field>>{
public:
	using AbstractObject::AbstractObject;
	virtual string typeName() const override final;
	virtual string strategyName() const=0;
};