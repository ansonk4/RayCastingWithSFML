#include "ray.h"

bool ray::operator< (const ray& r) const
{
	return this->angle < r.angle;
}

void ray::printRay()
{
	std::cout << this->angle << ' ' << this->x << ' ' << this->y << '\n';
}

ray::ray() 
{

}

ray::ray(float ang, float X, float Y)
{
	this->angle = ang;
	this->x = X;
	this->y = Y;
}

ray::~ray() {

}

