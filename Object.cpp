
#include "Object.h"


Object::~Object()
{
}

void Object::update()
{
	center.x += speed.x * vector.x;
	center.y += speed.y * vector.y;
}