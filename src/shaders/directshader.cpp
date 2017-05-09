#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader( Vector3D bgColor_, double maxDist_) {
	maxDist = maxDist_;
	bgColor = bgColor_;
	color = Vector3D(0.0, 0.0, 0.0);
}

Vector3D DirectShader::computeColor(const Ray &r,
	const std::vector<Shape*> &objList,
	const std::vector<PointLightSource> &lsList) const {

	Intersection its = Intersection();
	if (Utils::getClosestIntersection(r, objList, its)) {
		Vector3D normal = its.normal;
		Vector3D pointToCamera = (r.o - its.itsPoint).normalized();
		//double dist = sqrt(dot(pointToCamera, pointToCamera));
		Vector3D color;
		for (int i = 0; i < lsList.size(); i++) {
			Vector3D pointToLight = (r.o - its.itsPoint).normalized();
		
			Vector3D intensity = lsList[i].getIntensity(its.itsPoint);
			Vector3D reflectance = its.shape->getMaterial().getReflectance(normal, pointToCamera, pointToLight);
			color += Vector3D(intensity.x * reflectance.x, intensity.y * reflectance.y, intensity.z * reflectance.z);
		}

		return color;
	}

	return bgColor;
}