#include "depthshader.h"
#include "../core/utils.h"

DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_) {
	color = color_;
	maxDist = maxDist_;
	bgColor = bgColor_;
}

Vector3D DepthShader::computeColor(const Ray &r,
								   const std::vector<Shape*> &objList,
								   const std::vector<PointLightSource> &lsList) const{

	Intersection its = Intersection();
	if (Utils::getClosestIntersection(r, objList, its)) {
		Vector3D vDist = its.itsPoint - r.o;
		double dist = sqrt(dot(vDist, vDist));
		if (dist < maxDist) {
			return color *(1.0 - dist / maxDist);
		}
	}
	
	return bgColor;
}