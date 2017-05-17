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
		//double dist = sqrt(dot(pointToCamera, pointToCamera));

		Vector3D color;

		if (its.shape->getMaterial().hasSpecular()) {
			Vector3D wr = Utils::computeReflectionDirection(r.d, normal);
			Ray reflectionRay(its.itsPoint, wr, r.depth + 1);
			color = computeColor(reflectionRay, objList, lsList);
		}

		if (its.shape->getMaterial().hasTransmission()) {

			double cosA = dot(-r.d.normalized(), normal.normalized());
			double cosB;
			double eta = its.shape->getMaterial().getIndexOfRefraction();

			if (cosA < 0) {
				eta = 1 / eta;
				normal = -normal;
				cosA = dot(-r.d.normalized(), normal.normalized());
			}

			if (!Utils::isTotalInternalReflection(eta, cosA, cosB)) {
				//return Vector3D(1, 0, 0);

				Vector3D wt = Utils::computeTransmissionDirection(r, normal, eta, cosA, cosB);
				Ray refractRay(its.itsPoint, wt, r.depth + 1);
				color = computeColor(refractRay, objList, lsList);
			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(r.d, normal);
				Ray reflectionRay(its.itsPoint, wr, r.depth + 1);
				color = computeColor(reflectionRay, objList, lsList);
			}
		}

		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
			for (int i = 0; i < lsList.size(); i++) {
				Vector3D pointToLight = (lsList[i].getPosition() - its.itsPoint).normalized();
				double distanceToLight = sqrt(dot(lsList[i].getPosition() - its.itsPoint, lsList[i].getPosition() - its.itsPoint));

				if (dot(pointToLight, normal) <= 0) continue;
				Ray shadowRay(its.itsPoint, pointToLight, 0, 0.2, distanceToLight);
				if (Utils::hasIntersection(shadowRay, objList)) continue;

				Vector3D intensity = lsList[i].getIntensity(its.itsPoint);
				Vector3D reflectance = its.shape->getMaterial().getReflectance(normal, -r.d, pointToLight);
				color += Utils::multiplyPerCanal(intensity, reflectance);
			}
		}

		return color;
	}

	return bgColor;
}