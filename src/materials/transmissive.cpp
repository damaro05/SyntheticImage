#include "transmissive.h"
#include <cmath>
#include <iostream>

Transmissive::Transmissive(double _eta, Vector3D vec)
{
	eta = _eta;
}

Vector3D Transmissive::getReflectance(const Vector3D &n, const Vector3D &wo,
	const Vector3D &wi) const {

	//Vector3D diffuse = _kd * (dot(wi, n));
	//double cosA = dot(wi, n);
	//Vector3D wt = wi*-1*eta + n*(eta*cosA-cosB);
	//Vector3D specular = _ks * pow(dot(wo, wr), _shininess);
	
	//return wt;
	return Vector3D(1);
}

double Transmissive::getIndexOfRefraction() const
{
//    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
//              << std::endl;

    return eta;
}

bool Transmissive::hasSpecular() const
{
	return false;
}

bool Transmissive::hasTransmission() const
{
	return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
	return false;
}
