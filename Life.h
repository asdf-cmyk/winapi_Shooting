#pragma once
#include "Object.h"
#include "Block.h"
class Life: public Object
{
	int width, height;
public:
	Life(POINT&, int, int);
	~Life();
	void show(HDC);
	void collision(std::vector<Block*>&);
};

