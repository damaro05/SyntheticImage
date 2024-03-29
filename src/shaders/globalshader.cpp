#include "globalshader.h"
#include "../core/utils.h"
#include "../samplers/hemisphericalsampler.h"

GlobalShader::GlobalShader(Vector3D bgColor_, Vector3D at_) {
	bgColor = bgColor_;
	at = at_;
}

Vector3D GlobalShader::computeColor(const Ray &r,
	const std::vector<Shape*> &objList,
	const std::vector<PointLightSource> &lsList) const {

	Intersection its = Intersection();
	if (Utils::getClosestIntersection(r, objList, its)) {
		Vector3D normal = its.normal;
		//double dist = sqrt(dot(pointToCamera, pointToCamera));

		Vector3D Idir(0.0, 0.0, 0.0);
		Vector3D Iind(0.0, 0.0, 0.0);

		if (its.shape->getMaterial().hasSpecular()) {
			double cosA = dot(-r.d.normalized(), normal.normalized());
			if (cosA < 0)
				return bgColor;
			Vector3D wr = Utils::computeReflectionDirection(r.d, normal);
			Ray reflectionRay(its.itsPoint, wr, r.depth, 0.05);
			Idir = computeColor(reflectionRay, objList, lsList);
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
				Ray refractRay(its.itsPoint, wt, r.depth);
				Idir = computeColor(refractRay, objList, lsList);
			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(r.d, normal);
				Ray reflectionRay(its.itsPoint, wr, r.depth);
				Idir = computeColor(reflectionRay, objList, lsList);
			}
		}

		if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
			//Iind = Utils::multiplyPerCanal(its.shape->getMaterial().getDiffuseCoefficient(), at);
			

			for (int i = 0; i < lsList.size(); i++) {
				Vector3D pointToLight = (lsList[i].getPosition() - its.itsPoint).normalized();
				double distanceToLight = sqrt(dot(lsList[i].getPosition() - its.itsPoint, lsList[i].getPosition() - its.itsPoint));

				if (dot(pointToLight, normal) <= 0) continue;
				Ray shadowRay(its.itsPoint, pointToLight, r.depth + 1, 0.05, distanceToLight);
				if (Utils::hasIntersection(shadowRay, objList)) continue;

				Vector3D intensity = lsList[i].getIntensity(its.itsPoint);
				Vector3D reflectance = its.shape->getMaterial().getReflectance(normal, -r.d, pointToLight);
				Idir += Utils::multiplyPerCanal(intensity, reflectance);
			}

			if (r.depth == 0) {
				HemisphericalSampler sampler;
				int nSamples = 10;
				Vector3D toRandomDir;
				for (int i = 0; i < nSamples; i++) {
					toRandomDir = sampler.getSample(normal);
					
					//Vector3D pointToLight = (lsList[i].getPosition() - toRandomDir).normalized();
					//double distanceToLight = sqrt(dot(lsList[i].getPosition() - its.itsPoint, lsList[i].getPosition() - toRandomDir));

					//if (dot(pointToLight, normal) <= 0) continue;
					Ray samplerRay(its.itsPoint, toRandomDir, r.depth + 1, 0.05);

					Vector3D rayLightInt = computeColor(samplerRay, objList, lsList);
					Vector3D reflectance = its.shape->getMaterial().getReflectance(normal, -r.d, toRandomDir);
					Iind += Utils::multiplyPerCanal(rayLightInt, reflectance);
				}
				Iind /= nSamples;
			}
			else if (r.depth >= MAX_DEPTH) {
				Iind = Utils::multiplyPerCanal(its.shape->getMaterial().getDiffuseCoefficient(), at);
			}
			else {
				Ray normalRay(its.itsPoint, normal, r.depth+1, 0.05);
				Vector3D wr = Utils::computeReflectionDirection(r.d, normal);
				Ray reflectionRay(its.itsPoint, wr, r.depth+1, 0.05);

				Vector3D normalColor = computeColor(normalRay, objList, lsList);
				Vector3D reflectionColor = computeColor(reflectionRay, objList, lsList);

				Vector3D normalLightInt = its.shape->getMaterial().getReflectance(normal, -r.d, normal);
				normalColor = Utils::multiplyPerCanal(normalColor, normalLightInt);

				Vector3D wrLightInt = its.shape->getMaterial().getReflectance(normal, -r.d, wr);
				reflectionColor = Utils::multiplyPerCanal(reflectionColor, wrLightInt);

				Iind = (normalColor + reflectionColor) / 2.0;
			}
		}

		return Iind + Idir;
	}

	return bgColor;
}