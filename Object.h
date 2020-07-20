#pragma once
#include "stdafx.h"
class Object
{
protected:
	POINT center, speed;
	RECT rectView;
	bool isExist;
	double vectorX, vectorY;
public:
	Object(): isExist(1) {};
	~Object();
	//virtual void collision(Object&) {};
	void update();
	//virtual void show(HDC) {};

	POINT getCenter() { return center; }
	bool getIsExist() { return isExist; }
	void setIsExist(bool set) { isExist = set; }
};