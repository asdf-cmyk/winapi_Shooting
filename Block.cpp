
#include "Block.h"


Block::Block(POINT& cPt, int w, int h)
{
	center.x = cPt.x;
	center.y = cPt.y;
	//width = 70;
	//height = 60;
	width = w;
	height = h;
	speed.x = vector.x = 0;
	speed.y = height + 2;
	vector.y = 1;
}

Block::~Block()
{
}

//void Block::update()
//{
//}

void Block::show(HDC hdc)
{
	Rectangle(hdc, center.x - width/2 + 2, center.y - height/2,
		center.x + width/2, center.y + height/2);
}