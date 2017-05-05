#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_) {
	color = color_;
	maxDist = maxDist_;
	bgColor = bgColor_;
}

Vector3D DirectShader::computeColor(const Ray &r,
	const std::vector<Shape*> &objList,
	const std::vector<PointLightSource> &lsList) const {

	/*Intersection its = Intersection();
	if (Utils::getClosestIntersection(r, objList, its)) {
		Vector3D vDist = its.itsPoint - r.o;
		double dist = sqrt(dot(vDist, vDist));
		
		double color = 0.0;

		for (int i = 0; i < lsList.size(); i++) {
			Ray r2 = Ray();
			color += lsList[i].getIntensity(its);
		}

		return color;
	}*/

	return bgColor;
}