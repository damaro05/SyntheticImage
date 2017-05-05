#include "phong.h"

#include <iostream>

Phong::Phong()
{ 

}

double Phong::getIndexOfRefraction() const
{
//    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
//              << std::endl;

    return -1;
}

bool Phong::hasSpecular() const
{
	return -1;

}

bool Phong::hasTransmission() const
{
	return -1;

}

bool Phong::hasDiffuseOrGlossy() const
{
	return -1;
}
