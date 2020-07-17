#include "stdafx.h"
#include "Cannon.h"


Cannon::Cannon()
	: lineCannon(100), radCannon(0)
{
	POINT centerCannon = { 300, 750 };
	POINT lineGun = { 20, 200 };
}

Cannon::~Cannon()
{
}

void Cannon::update(int rad)
{
	radCannon = rad;
	POINT gunP3 = { long(double(centerCannon.x) + lineGun.x*cos(2 * acos(0.0) / 180 * (radCannon))),
		long(double(centerCannon.y) + lineGun.x*sin(2 * acos(0.0) / 180 * (radCannon))) };
	POINT gunP4 = { long(double(centerCannon.x) - lineGun.x*cos(2 * acos(0.0) / 180 * (radCannon))),
		long(double(centerCannon.y) - lineGun.x*sin(2 * acos(0.0) / 180 * (radCannon))) };
	POINT gun[4] =
	{ { long(double(gunP4.x) + lineGun.y*cos(2 * acos(0.0) / 180 * (90 - radCannon))),
		long(double(gunP4.y) - lineGun.y*sin(2 * acos(0.0) / 180 * (90 - radCannon))) },
		{ long(double(gunP3.x) + lineGun.y*cos(2 * acos(0.0) / 180 * (90 - radCannon))),
		long(double(gunP3.y) - lineGun.y*sin(2 * acos(0.0) / 180 * (90 - radCannon))) },
		gunP3, gunP4 };
}

void Cannon::show(HDC hdc)
{
	Polygon(hdc, gun, 4);
	//Arc(hdc, 200, 600, 400, 800, 400, 800, 200, 800);
	/*Arc(hdc, centerCannon.x - lineCannon, centerCannon.y - lineCannon,
	centerCannon.x + lineCannon, centerCannon.y + lineCannon,
	centerCannon.x + lineCannon, centerCannon.y + lineCannon,
	centerCannon.x - lineCannon, centerCannon.y + lineCannon);*/
	Ellipse(hdc, centerCannon.x - lineCannon,
		centerCannon.y - lineCannon,
		centerCannon.x + lineCannon,
		centerCannon.y + lineCannon);
}