#include "normalshader.h"
#include "../core/utils.h"
#include "../materials/phong.h"

NormalShader::NormalShader()
{}

NormalShader::NormalShader(Vector3D bgColor_) :
    Shader(bgColor_)
{}

Vector3D NormalShader::computeColor(const Ray &r, const std::vector<Shape *> &objList, const std::vector<PointLightSource> &lsList) const
{
    Intersection its;
	

    if (Utils::getClosestIntersection(r, objList, its))
    {
		Vector3D res;
		for (int i = 0; i < lsList.size(); i++) {
			
			Vector3D intensity = lsList[i].getIntensity(its.itsPoint);
			//Vector3D reflectance = its.shape->getMaterial().getReflectance(its.shape-> )
			
			//res = (its.normal + Vector3D(1.0)) / 2.0;

		}

		return res;

    } else
    {
        return bgColor;
    }
}
