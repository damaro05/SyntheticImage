#ifndef MIRROR
#define MIRROR
#include "material.h"
#include "../core/vector3d.h"

class Mirror : public Material
{
public:
	Mirror(const Vector3D &kd, const Vector3D &ks, const double& shininess);

    virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
                                    const Vector3D &wi) const;
    virtual bool hasSpecular() const;
    virtual bool hasTransmission() const;
    virtual bool hasDiffuseOrGlossy() const;
    virtual double getIndexOfRefraction() const;

private:
	double _shininess;
	Vector3D _kd;
	Vector3D _ks;
};


#endif // MIRROR
