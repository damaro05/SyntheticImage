#include "ray.h"
int Ray::rayCounter = 0;
Ray::Ray() : minT(0), maxT(INFINITY), depth(0)
{
	rayCounter++;
}

Ray::Ray(const Vector3D &ori, const Vector3D &dir, size_t dep, double start,
         double end)
         : o(ori), d(dir), minT(start), maxT(end), depth(dep)
{
	rayCounter++;
}

std::string Ray::toString() const
{
    std::ostringstream out;
    out << "origin: " << o << std::endl;
    out << "direction: " << d << std::endl;
    out << "minT = " << minT << ", maxT = " << maxT << std::endl;
    return out.str();
}

std::ostream &operator<<(std::ostream &out, const Ray &r)
{
    out << r.toString();
    return out;
}

void Ray::resetCounter() {
	rayCounter = 0;
}

