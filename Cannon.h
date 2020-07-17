#pragma once
#include "Object.h"
class Cannon: public Object
{
	POINT centerCannon, lineGun, gun[4];
	int lineCannon, radCannon;
public:
	Cannon();
	~Cannon();
	void update(int);
	void show(HDC);
};

