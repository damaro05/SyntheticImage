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
		Vector3D pointToCamera = (its.itsPoint - r.o).normalized();
		//double dist = sqrt(dot(pointToCamera, pointToCamera));

		Vector3D color;
		for (int i = 0; i < lsList.size(); i++) {
			Vector3D pointToLight = (lsList[i].getPosition() - its.itsPoint).normalized();
			double distanceToLight = sqrt(dot(lsList[i].getPosition() - its.itsPoint, lsList[i].getPosition() - its.itsPoint));

			if (its.shape->getMaterial().hasSpecular()) {
				Vector3D wr = its.shape->getMaterial().getReflectance(normal, pointToCamera, pointToLight);
				Ray reflectionRay(its.itsPoint, wr*-1, r.depth+1);
				color = computeColor(reflectionRay, objList, lsList);
			}

			if (its.shape->getMaterial().hasTransmission()) {

				double cosA = dot(-r.d.normalized(), normal.normalized());
				double etaT = 1/its.shape->getMaterial().getIndexOfRefraction();
				double radicand = 1 + pow(etaT, 2)*(pow(cosA, 2) - 1);

				
				if (radicand > 0){
					return Vector3D(1, 0, 0);
					double cosB = sqrt(radicand);
					//Vector3D wt = its.shape->getMaterial().getReflectance(normal, pointToCamera, pointToLight);
					
					Vector3D wt = pointToLight*-1 * etaT + normal*(etaT*cosA - cosB);
					Ray refractRay(its.itsPoint, wt, r.depth + 1);
					color = computeColor(refractRay, objList, lsList);
					return Vector3D(1,0,0);
				}
				return Vector3D(0, 0, 1);
				//Vector3D wr = its.shape->getMaterial().getReflectance(normal, pointToCamera, pointToLight);
				Vector3D wr = normal * 2 * dot(normal, pointToCamera) - pointToCamera;
				Ray reflectionRay(its.itsPoint, wr*-1, r.depth + 1);
				color = computeColor(reflectionRay, objList, lsList);
				
			}

			if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
				if (dot(pointToLight, normal) <= 0) continue;
				Ray shadowRay(its.itsPoint, pointToLight, 0, 0.2, distanceToLight);
				if (Utils::hasIntersection(shadowRay, objList)) continue;

				Vector3D intensity = lsList[i].getIntensity(its.itsPoint);
				Vector3D reflectance = its.shape->getMaterial().getReflectance(normal, pointToCamera, pointToLight);
				color += Utils::multiplyPerCanal(intensity, reflectance);
			}
		}

		return color;
	}

	return bgColor;
}