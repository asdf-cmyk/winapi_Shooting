#pragma once
#include "stdafx.h"
class Object
{
protected:
	POINT center, speed, vector;
	bool isExist;
public:
	Object(): isExist(1) {};
	~Object();
	//virtual void collision(Object&) {};
	void update();
	//virtual void show(HDC) {};
};