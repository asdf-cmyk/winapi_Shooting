#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet(Cannon& cannon, RECT& rV)
	: spawnDist(2*cannon.getLineG().x + 20), radBullet(cannon.getRad()), radius(20)
{
	center.x = long(double(cannon.getCenterC().x) +
		(cannon.getLineG().y + spawnDist)*cos(2 * acos(0.0) / 180 * (90-radBullet)));
	center.y = long(double(cannon.getCenterC().y) -
		(cannon.getLineG().y + spawnDist)*sin(2 * acos(0.0) / 180 * (90-radBullet)));
	
	rectView = rV;
	speed = { 50, 50 };
	vectorX = cos(2 * acos(0.0) / 180 * (90 - radBullet));
	vectorY = -sin(2 * acos(0.0) / 180 * (90 - radBullet));
}


Bullet::~Bullet()
{
}


//void Bullet::update()
//{
//
//}

void Bullet::show(HDC hdc)
{
	Ellipse(hdc, center.x - radius, center.y - radius,
		center.x + radius, center.y + radius);
}

void Bullet::collision(std::vector<Block*>& blockCont, int& score)
{
	if (center.x + radius < rectView.left || center.x - radius > rectView.right ||
		center.y + radius < rectView.top || center.y - radius > rectView.bottom)
		isExist = 0;

	for (unsigned int i = 0; i < blockCont.size(); i++)
	{
		//int blcokColliRad = min(blockCont[i]->getHeight(), blockCont[i]->getWidth());
		int colliCnt = 0;
		if (abs((center.y - (blockCont[i]->getCenter().y + blockCont[i]->getHeight() / 2))) <= radius)
			colliCnt++;
		if (abs((center.y - (blockCont[i]->getCenter().y - blockCont[i]->getHeight() / 2))) <= radius)
			colliCnt++;
		if (abs((center.x - (blockCont[i]->getCenter().x + blockCont[i]->getWidth() / 2))) <= radius)
			colliCnt++;
		if (abs((center.x - (blockCont[i]->getCenter().x - blockCont[i]->getWidth() / 2))) <= radius)
			colliCnt++;
		
		if (colliCnt > 1)
		{
			isExist = 0;
			blockCont[i]->setIsExist(0);
			score += 100;
		}
		else if (colliCnt == 1 && (((center.x - radius >= (blockCont[i]->getCenter().x - blockCont[i]->getWidth() / 2)) &&
			(center.x + radius <= (blockCont[i]->getCenter().x + blockCont[i]->getWidth() / 2))) ||
			((center.y - radius >= (blockCont[i]->getCenter().y - blockCont[i]->getHeight() / 2)) &&
			(center.y + radius <= (blockCont[i]->getCenter().y + blockCont[i]->getHeight() / 2)))))
		{
			isExist = 0;
			blockCont[i]->setIsExist(0);
			score += 100;
		}

		/*if (abs((center.y - (blockCont[i]->getCenter().y + blockCont[i]->getHeight() / 2))) <= radius)
		{
			isExist = 0;
			blockCont[i]->setIsExist(0);
		}*/
	}
}