#pragma once
#include "Object.h"
//#include <vector>
class Block : public Object
{
	int width, height;
public:
	Block(POINT&, int, int);
	~Block();
	void collision(Object&) {};
	//void update();
	void show(HDC);
};

//std::vector<Block*> blockCont;