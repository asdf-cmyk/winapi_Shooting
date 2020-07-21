#pragma once
#include "Object.h"
#include "Cannon.h"
#include "Block.h"
#include <vector>

class Bullet: public Object
{
	int spawnDist, radBullet, radius;
public:
	Bullet(Cannon&, RECT&);
	~Bullet();
	//void update();
	void show(HDC);
	void collision(std::vector<Block*>&, int&);
};

