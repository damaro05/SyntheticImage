#include "mirror.h"

#include <iostream>

Mirror::Mirror(const Vector3D &color)
{
	_color = color;
}

Vector3D Mirror::getReflectance(const Vector3D &n, const Vector3D &wo,
	const Vector3D &wi) const {

	//Vector3D diffuse = _kd * (dot(wi, n));
	Vector3D wr = n * 2 * dot(n, wo) - wo;
	//Vector3D specular = _ks * pow(dot(wo, wr), _shininess);
	
	return wr;
	//return diffuse;
}

double Mirror::getIndexOfRefraction() const
{
//    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
//              << std::endl;

    return -1;
}

bool Mirror::hasSpecular() const
{
	return true;
}

bool Mirror::hasTransmission() const
{
	return false;
}

bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}
