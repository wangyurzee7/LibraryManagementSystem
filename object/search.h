#include <bits/stdc++.h>

#include "abstractobject.h"

using namespace std;


#pragma once
class Search:public AbstractObject<multiset<Field>>{
	static const int maxSize;
	static const int maxLength;
public:
	using AbstractObject::AbstractObject;
	virtual string typeName() const override final;
	virtual string strategyName() const=0;
	virtual FormatInfo invalidFields() const;
};

class CompleteMatchingSearch:public Search{
	// virtual string strategyName() const override;
};
class FuzzyMatchingSearch:public Search{
	// virtual string strategyName() const override;
};
class ReSearch:public Search{
	// virtual string strategyName() const override;
};