#ifndef MIRROR
#define MIRROR
#include "material.h"
#include "../core/vector3d.h"

class Mirror : public Material
{
public:
	Vector3D _color;
	Mirror(const Vector3D &color);

    virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
                                    const Vector3D &wi) const;
    virtual bool hasSpecular() const;
    virtual bool hasTransmission() const;
    virtual bool hasDiffuseOrGlossy() const;
    virtual double getIndexOfRefraction() const;

private:
	
};


#endif // MIRROR
