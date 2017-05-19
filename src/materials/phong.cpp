#include "phong.h"

#include <iostream>

Phong::Phong(const Vector3D &kd, const Vector3D &ks, const double& shininess)
{
	_kd = kd;
	_ks = ks;
	_shininess = shininess;
}

Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo,
	const Vector3D &wi) const {
	Vector3D diffuse = _kd * (dot(wi, n));
	Vector3D wr = n * 2 * dot(n, wi) - wi;
	Vector3D specular = _ks * pow(dot(wo, wr), _shininess);
	
	return (diffuse + specular);
	//return diffuse;
}

double Phong::getIndexOfRefraction() const
{
//    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
//              << std::endl;

    return -1;
}

bool Phong::hasSpecular() const
{
	return false;
}

bool Phong::hasTransmission() const
{
	return false;
}

bool Phong::hasDiffuseOrGlossy() const
{
	return true;
}

Vector3D Phong::getDiffuseCoefficient() const
{
	return _kd;
}