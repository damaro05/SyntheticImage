#ifndef TRANSMISSIVE
#define TRANSMISSIVE
#include "material.h"
#include "../core/vector3d.h"

class Transmissive : public Material
{
public:
	Transmissive(double, Vector3D);

    virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
                                    const Vector3D &wi) const;
    virtual bool hasSpecular() const;
    virtual bool hasTransmission() const;
    virtual bool hasDiffuseOrGlossy() const;
    virtual double getIndexOfRefraction() const;

private:
	double eta;
};


#endif // TRANSMISSIVE
