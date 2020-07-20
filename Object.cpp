
#include "Object.h"


Object::~Object()
{
}

void Object::update()
{
	center.x += speed.x * vectorX;
	center.y += speed.y * vectorY;
}