#ifndef PHONG
#define PHONG
#include "material.h"
#include "../core/vector3d.h"

class Phong : public Material
{
public:
	Phong();

    virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
                                    const Vector3D &wi) const = 0;
    virtual bool hasSpecular() const;
    virtual bool hasTransmission() const;
    virtual bool hasDiffuseOrGlossy() const;
    virtual double getIndexOfRefraction() const;
};


#endif // PHONG
