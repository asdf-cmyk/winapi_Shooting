#include "stdafx.h"
#include "Life.h"


Life::Life(POINT& cPt, int w, int h)
{
	center.x = cPt.x;
	center.y = cPt.y;
	//width = 70;
	//height = 60;
	width = w;
	height = h;
	speed.x = speed.y = vectorX = vectorY = 0;
}


Life::~Life()
{
}

void Life::show(HDC hdc)
{
	Rectangle(hdc, center.x - width / 2 + 2, center.y - height / 2,
		center.x + width / 2, center.y + height / 2);
}

void Life::collision(std::vector<Block*>& blockCont)
{
	for (unsigned int i = 0; i < blockCont.size(); i++)
	{
		if ((blockCont[i]->getCenter().y + blockCont[i]->getHeight() / 2 >=
			center.y - height / 2) &&
			(blockCont[i]->getCenter().x - blockCont[i]->getWidth() / 2 >=
			center.x - width/2) &&
			(blockCont[i]->getCenter().x + blockCont[i]->getWidth() / 2 <=
			center.x + width / 2))
		{
			isExist = 0;
			blockCont[i]->setIsExist(0);
		}
	}
		
}