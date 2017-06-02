#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>
#include <ctime>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplane.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/directshader.h"
#include "shaders/globalshader.h"

#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/transmissive.h"

#define IMAGE_WIDTH 300
#define IMAGE_HEIGHT 300

struct rayInfo {
	bool thrown = false;
	Vector3D color = Vector3D(0.0, 0.0, 0.0);
};

rayInfo rays[IMAGE_WIDTH+1][IMAGE_HEIGHT+1];


void buildSceneCornellBox(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
	Material *transmissive = new Transmissive(1.1, Vector3D(1));
	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	//Material *transmissive = new Phong(Vector3D(1, 1, 0.2), Vector3D(1, 1, 0.2), 20);
	//Material *mirror = new Mirror(Vector3D(0, 0.9, 0.9), Vector3D(0.1, 0.9, 0.9), 50);
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

	/* ******* */
	/* Objects */
	/* ******* */
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;
	// Construct the Cornell Box
	Shape *leftPlan = new InfinitePlane(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), redDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), blueDiffuse);
	objectsList->push_back(leftPlan);
	objectsList->push_back(rightPlan);
	objectsList->push_back(topPlan);
	objectsList->push_back(bottomPlan);
	objectsList->push_back(backPlan);

	// Place the Spheres inside the Cornell Box
	Matrix4x4 sphereTransform1;
	double radius = 1;
	sphereTransform1 = Matrix4x4::translate(Vector3D(-offset + radius, -offset + radius, 3.5));
	Shape *s1 = new Sphere(1.5, sphereTransform1, mirror);
	Matrix4x4 sphereTransform2;
	sphereTransform2 = Matrix4x4::translate(Vector3D(1.0, 0.0, 2));
	Shape *s2 = new Sphere(1, sphereTransform2, transmissive);
	Matrix4x4 sphereTransform3;
	radius = 1;
	sphereTransform3 = Matrix4x4::translate(Vector3D(0.3, -offset + radius, 5));
	Shape *s3 = new Sphere(radius, sphereTransform3, red_100);
	objectsList->push_back(s1);
	objectsList->push_back(s2);
	objectsList->push_back(s3);

	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
	Vector3D intensity = Vector3D(5.5, 5.5, 5.5); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
}

void buildSceneSphere(Camera* &cam, Film* &film,
                      std::vector<Shape*>* &objectsList,
                      std::vector<PointLightSource>* &lightSourceList)
{
    /* **************************** */
    /* Declare and place the camera */
    /* **************************** */
    // By default, this gives an ID transform
    //  which means that the camera is located at (0, 0, 0)
    //  and looking at the "+z" direction
    Matrix4x4 cameraToWorld;
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ************************** */
    /* DEFINE YOUR MATERIALS HERE */
    /* ************************** */
    // (...)
    //  EXAMPLE:  Material *green_50 = new Phong (Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);
	Material *green_100 = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 100);
	Material *red_150 = new Phong(Vector3D(1, 0.3, 0.5), Vector3D(1, 0.3, 0.5), 150);
	Material *blue_100 = new Phong(Vector3D(0.2, 0.1, 1), Vector3D(0.2, 0.1, 1), 100);
	Material *white_100 = new Phong(Vector3D(0.0, 0.0, 0.0), Vector3D(1.0, 1.0, 1.0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);

    /* ******* */
    /* Objects */
    /* ******* */
    // Create a heterogeneous list of objects of type shape
    // (some might be triangles, other spheres, plans, etc)
    objectsList = new std::vector<Shape*>;

    // Define and place a sphere
    Matrix4x4 sphereTransform1;
    sphereTransform1 = sphereTransform1.translate(Vector3D(-1.0, -0.5, 2*std::sqrt(2.0)));
    //Shape *s1 = new Sphere (0.25, sphereTransform1, NULL);
	Shape *s1 = new Sphere(0.25, sphereTransform1, blue_100);

    // Define and place a sphere
    Matrix4x4 sphereTransform2;
    sphereTransform2 = sphereTransform2.translate(Vector3D(1.0, 0.0, 6));
    //Shape *s2 = new Sphere (1, sphereTransform2, NULL);
	Shape *s2 = new Sphere(1, sphereTransform2, green_100);

    // Define and place a sphere
    Matrix4x4 sphereTransform3;
    sphereTransform3 = sphereTransform3.translate(Vector3D(0.3, -0.75, 3.5));
    //Shape *s3 = new Sphere (0.25, sphereTransform3, NULL);
	Shape *s3 = new Sphere(0.25, sphereTransform3, red_150);


	//Infinite plane
	double offset = 2;
	Shape *bottomPlane = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);

    // Store the objects in the object list
    objectsList->push_back(s1);
    objectsList->push_back(s2);
    objectsList->push_back(s3);
	objectsList->push_back(bottomPlane);

    /* ****** */
    /* Lights */
    /* ****** */
    //
    // ADD YOUR LIGHT SOURCES HERE
	PointLightSource light1(Vector3D(1, 2, 1), Vector3D(10, 10, 10));
	PointLightSource light2(Vector3D(-2.5, 0.5, 0), Vector3D(6, 6, 6));
	PointLightSource light3(Vector3D(2, -1.5, 2), Vector3D(7, 7, 7));
	PointLightSource light4(Vector3D(1, 5, 5), Vector3D(10, 10, 10));


	lightSourceList = new std::vector<PointLightSource>;
	lightSourceList->push_back(light1);
	lightSourceList->push_back(light2);
	//lightSourceList->push_back(light3);
	lightSourceList->push_back(light4);

    // DO NOT FORGET TO STORE THE LIGHT SOURCES IN THE "lightSourceList"
    // (...)
    //
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor( cameraRay, *objectsList, *lightSourceList );

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}

void buildSceneProject(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList) {
	
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
	Material *transmissive = new Transmissive(1.1, Vector3D(1));
	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

	/* ******* */
	/* Objects */
	/* ******* */
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;
	// Construct the Cornell Box
	Shape *leftPlan = new InfinitePlane(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), redDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), blueDiffuse);
	objectsList->push_back(leftPlan);
	objectsList->push_back(rightPlan);
	objectsList->push_back(topPlan);
	objectsList->push_back(bottomPlan);
	objectsList->push_back(backPlan);

	// Place the Spheres inside the Cornell Box
	Matrix4x4 sphereTransform1;
	double radius = 1;
	sphereTransform1 = Matrix4x4::translate(Vector3D(-offset + radius, -offset + radius, 3.5));
	Shape *s1 = new Sphere(1.5, sphereTransform1, mirror);
	Matrix4x4 sphereTransform2;
	sphereTransform2 = Matrix4x4::translate(Vector3D(1.0, 0.0, 2));
	Shape *s2 = new Sphere(1, sphereTransform2, transmissive);
	Matrix4x4 sphereTransform3;
	radius = 1;
	sphereTransform3 = Matrix4x4::translate(Vector3D(0.3, -offset + radius, 5));
	Shape *s3 = new Sphere(radius, sphereTransform3, red_100);
	
	Matrix4x4 sphereTransform4;
	radius = 1;
	sphereTransform4 = Matrix4x4::translate(Vector3D(0.0, 0.0, 2));
	Shape *s4 = new Sphere(radius, sphereTransform4, red_100);

	//objectsList->push_back(s1);
	//objectsList->push_back(s2);
	//objectsList->push_back(s3);
	objectsList->push_back(s4);

	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
	Vector3D intensity = Vector3D(5.5, 5.5, 5.5); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
}


/* ************ */
/* Project code */
/* ************ */


bool isDifferentColor(Vector3D actual, Vector3D nextToActual, float tolerance = 1.0) {
	return (tolerance < sqrt( pow(actual.x-nextToActual.x,2) + pow(actual.y - nextToActual.y, 2) + pow(actual.z - nextToActual.z, 2)));
}


void imageFilter(Camera* &cam, Shader* &shader, Film* &firstImage, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList, const std::string imageName, int mode)
{
	
	unsigned int sizeBar = 40;

	size_t resX = firstImage->getWidth();
	size_t resY = firstImage->getHeight();

	film = new Film(resX, resY);

	double nFilterRays = 10.0;
	int filterRadius = 1;
	float tolerance = 0.01; // Max value ~= 0.81 - Min value ~= 0.032
	

	for (size_t lin = 0; lin < resY; lin++)
	{
		// Show progression
		if (lin % (resY / sizeBar) == 0)
			std::cout << ".";

		for (size_t col = 0; col < resX; col++)
		{
			Vector3D averageColor = Vector3D(0.0, 0.0, 0.0);
			if (mode > 0) {
				//For para recorrer los pixeles colindantes
				double numPixels = 0.0;
				
				for (int x = -filterRadius; x <= filterRadius; x++) {
					for (int y = -filterRadius; y <= filterRadius; y++) {

						
						if ((col + x)>= 0 && (col + x) < resX && (y + lin) >= 0 && (y + lin) < resY) {
							if (x == y) continue;
							Vector3D actualColor;
							if (firstImage->getPixelValue(x + col, y + lin).length() > sqrt(3))
								actualColor = Vector3D(1.0, 1.0, 1.0);
							else actualColor = firstImage->getPixelValue(x + col, y + lin);
							averageColor += actualColor;
							numPixels++;


						}
					}
				}
				
				averageColor /= numPixels;
				// differentPixels += 3;
				
			}
			Vector3D finalColor(0, 0, 0);
			Vector3D myPixelColor;
			if (firstImage->getPixelValue(col,lin).length() > sqrt(3))
				myPixelColor = Vector3D(1.0, 1.0, 1.0);
			else myPixelColor = firstImage->getPixelValue(col, lin);

			if (mode == 1) {
				if (isDifferentColor(myPixelColor, averageColor, tolerance)) finalColor = Vector3D(0.0, 1.0, 1.0);
				else finalColor = firstImage->getPixelValue(col, lin);
			}
			else {
				if (isDifferentColor(myPixelColor, averageColor, tolerance) || mode == 0) {

					for (double y = 1 / (nFilterRays + 1); y < 1; y += 1 / (nFilterRays + 1)) {
						for (double x = 1 / (nFilterRays + 1); x < 1; x += 1 / (nFilterRays + 1)) {
							Ray cameraRay = cam->generateRay((col + x) / resX, (lin + y) / resY);

							// Compute ray color according to the used shader
							finalColor += shader->computeColor(cameraRay, *objectsList, *lightSourceList);
						}
					}
					finalColor /= pow(nFilterRays,2); //number of rays
				}
				else finalColor = firstImage->getPixelValue(col, lin);
			}
			film->setPixelValue(col, lin, finalColor);
		}
	}

	film->save(("./" + imageName + ".bmp").c_str());
}


void ourImageFilter(Camera* &cam, Shader* &shader, Film* &firstImage, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList, const std::string imageName )
{
	unsigned int sizeBar = 40;
	bool recomputePixel[IMAGE_WIDTH + 1][IMAGE_HEIGHT + 1];

	size_t resX = firstImage->getWidth();
	size_t resY = firstImage->getHeight();

	film = new Film(resX, resY);

	int filterRadius = 1;
	int differentPixels = 0;
	int filterStrictness = 2;
	float tolerance = 0.08; // Max value ~= 1.73
	
	for (size_t lin = 0; lin < resY; lin++)
	{
		// Show progression
		if (lin % (resY / sizeBar) == 0)
			std::cout << ".";

		for (size_t col = 0; col < resX; col++)
		{
			Vector3D averageColor = Vector3D(0.0, 0.0, 0.0);
			double numPixels = 0.0;
			//For para recorrer los pixeles colindantes
			for (int x = -filterRadius; x <= filterRadius; x++) {
				for (int y = -filterRadius; y <= filterRadius; y++) {


					if ((col + x) >= 0 && (col + x) < resX && (y + lin) >= 0 && (y + lin) < resY) {
						if (x == y) continue;
						Vector3D actualColor;
						if (firstImage->getPixelValue(x + col, y + lin).length() > sqrt(3))
							actualColor = Vector3D(1.0, 1.0, 1.0);
						else actualColor = firstImage->getPixelValue(x + col, y + lin);
						averageColor += actualColor;
						numPixels++;


					}
				}
			}

			averageColor /= numPixels;
			Vector3D myPixelColor;
			if (firstImage->getPixelValue(col, lin).length() > sqrt(3))
				myPixelColor = Vector3D(1.0, 1.0, 1.0);
			else myPixelColor = firstImage->getPixelValue(col, lin);

			if (isDifferentColor(myPixelColor, averageColor, tolerance)) {
				recomputePixel[col][lin] = true;
				for (double y = 0; y <= 1; y++) {
					for (double x = 0; x <= 1; x++) {
						if (rays[(int)(col + x)][(int)(lin + y)].thrown) continue;
						Ray cameraRay = cam->generateRay((col + x) / resX, (lin + y) / resY);

						// Compute ray color according to the used shader
						rays[(int)(col + x)][(int)(lin + y)].color = shader->computeColor(cameraRay, *objectsList, *lightSourceList);
						rays[(int)(col + x)][(int)(lin + y)].thrown = true;
					}
				}
			}
			else recomputePixel[col][lin] = false;
			//else finalColor = firstImage->getPixelValue(col, lin);
			
			//film->setPixelValue(col, lin, finalColor);
		}
	}

	float percentage = 0;
	int numRays = 0;
	for (size_t lin = 0; lin < resY; lin++)
	{
		// Show progression
		if (lin % (resY / sizeBar) == 0)
			std::cout << ".";

		for (size_t col = 0; col < resX; col++)
		{
			Vector3D finalColor(0, 0, 0);
			if (recomputePixel[col][lin]) {
				percentage = 0.0;
				

				for (double y = 0; y <= 1; y++) {
					for (double x = 0; x <= 1; x++) {
						//if (!rays[(int)(col + x)][(int)(lin + y)].thrown) continue;

						finalColor += rays[(int)(col + x)][(int)(lin + y)].color;
						percentage += 0.04;
						numRays++;
					}
				}
				if (numRays > 0)
					finalColor /= numRays;

				percentage *= numRays;
				finalColor = (finalColor*percentage) + firstImage->getPixelValue(col, lin)*(1 - percentage);
				percentage = 0;
				numRays = 0;
			}
			else finalColor = firstImage->getPixelValue(col, lin);

			film->setPixelValue(col, lin, finalColor);
		}
	}

	film->save(("./" + imageName + ".bmp").c_str());
}


void createBaseImage(Camera* &cam, Shader* &shader, Film* &firstImage,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList) {
	
	unsigned int sizeBar = 40;

	size_t resX = firstImage->getWidth();
	size_t resY = firstImage->getHeight();

	// Main raytracing loop
	// Out-most loop invariant: we have rendered lin lines
	std::cout << "Computing first image" << std::endl;
	for (size_t lin = 0; lin<resY; lin++)
	{
		// Show progression
		if (lin % (resY / sizeBar) == 0)
			std::cout << ".";

		// Inner loop invariant: we have rendered col columns
		for (size_t col = 0; col<resX; col++)
		{
			// Compute the pixel position in NDC
			double x = (double)(col + 0.5) / resX;
			double y = (double)(lin + 0.5) / resY;

			// Generate the camera ray
			Ray cameraRay = cam->generateRay(x, y);

			// Compute ray color according to the used shader
			Vector3D pixelColor = shader->computeColor(cameraRay, *objectsList, *lightSourceList);

			// Store the pixel color
			//film->setPixelValue(col, lin, pixelColor);
			firstImage->setPixelValue(col, lin, pixelColor);
		}
	}
	firstImage->save("./0-Original.bmp");
	std::cout << "\nFirst image complete. Computing anti-aliasing filters..." << std::endl;
}

double computePixelDifference(Film* &image1, Film* &image2) {
	double error = 0.0;

	size_t resX = image1->getWidth();
	size_t resY = image1->getHeight();

	for (size_t lin = 0; lin < resY; lin++)
	{
		for (size_t col = 0; col < resX; col++)
		{
			Vector3D pixelColor1 = image1->getPixelValue(col, lin);
			Vector3D pixelColor2 = image2->getPixelValue(col, lin);

			pixelColor1.x = (std::min(pixelColor1.x, 1.0));
			pixelColor1.y = (std::min(pixelColor1.y, 1.0));
			pixelColor1.z = (std::min(pixelColor1.z, 1.0));
			pixelColor2.x = (std::min(pixelColor2.x, 1.0));
			pixelColor2.y = (std::min(pixelColor2.y, 1.0));
			pixelColor2.z = (std::min(pixelColor2.z, 1.0));

			double difference = pixelColor1.lengthSq() - pixelColor2.lengthSq();
			error += difference;

		}
	}
	error /= (resX*resY);
	return sqrt(error)*100;
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // Create an empty film
    Film *originalImage;
    //film = new Film(720, 576);
	originalImage = new Film(IMAGE_WIDTH, IMAGE_HEIGHT);

    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);
	Vector3D at(0.2, 0.2, 0.2);

	//Shader *shader = new IntersectionShader(intersectionColor, bgColor);
	//Shader *shader = new DepthShader(Vector3D(0.4, 1, 0.4), 8, bgColor);
	Shader *shader = new DirectShader(bgColor, 100);
	
	//Shader *shader = new GlobalShader(bgColor, at);

    // Declare pointers to all the variables which describe the scene
    Camera *cam;
    std::vector<Shape*> *objectsList;
    std::vector<PointLightSource> *lightSourceList;

    // Build the scene
    //buildSceneSphere(cam, film, objectsList, lightSourceList);
	//buildSceneCornellBox(cam, film, objectsList, lightSourceList);
	buildSceneProject(cam, originalImage, objectsList, lightSourceList);

    // Launch some rays!
    //raytrace(cam, shader, film, objectsList, lightSourceList);
	//Film* originalImage = new Film(film->getWidth(), film->getHeight());
	
	clock_t begin_time = clock();
	createBaseImage(cam, shader, originalImage, objectsList, lightSourceList);
	float baseImageTime = float(clock() - begin_time) / CLOCKS_PER_SEC;
	
	std::cout << "\nOriginal image compute time: " << baseImageTime << std::endl;

	std::cout << Ray::rayCounter << std::endl;
	Ray::resetCounter();

	Film* allFiltered;
	begin_time = clock();
	imageFilter(cam, shader, originalImage, allFiltered, objectsList, lightSourceList, "1-AllFiltered", 0);
	std::cout << "\nAllFiltered image compute time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;
	std::cout << "Number of thrown rays: " << Ray::rayCounter << std::endl;

	Film* pixelsToFilter;
	begin_time = clock();
	imageFilter(cam, shader, originalImage, pixelsToFilter, objectsList, lightSourceList, "2-PixelsToFilter", 1);
	std::cout << "\nPixelsToFilter image compute time: " << (baseImageTime + float(clock() - begin_time) / CLOCKS_PER_SEC) << std::endl;

	Film* defaultFilter;
	Ray::resetCounter();
	begin_time = clock();
	imageFilter(cam, shader, originalImage, defaultFilter,objectsList, lightSourceList, "3-DefaultFilter", 2);
	std::cout << "\nDefaultFilter image compute time: " << (baseImageTime + float(clock() - begin_time) / CLOCKS_PER_SEC) << std::endl;
	std::cout << "Number of thrown rays: " << Ray::rayCounter << std::endl;

	Film* ourFilter;
	Ray::resetCounter();
	begin_time = clock();
	ourImageFilter(cam, shader, originalImage, ourFilter, objectsList, lightSourceList, "4-OurFilter");
	std::cout << "\OurFilter image compute time: " << (baseImageTime + float(clock() - begin_time) / CLOCKS_PER_SEC) << std::endl;
	std::cout << "Number of thrown rays: " << Ray::rayCounter << std::endl;

	std::cout << "Difference between AllFiltered and DefaultFilter " << computePixelDifference(allFiltered, defaultFilter) << "%" << std::endl;
	std::cout << "Difference between AllFiltered and OurFilter " << computePixelDifference(allFiltered, ourFilter) << "%" << std::endl;

    // Save the final result to file
    //std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
	//originalImage->save("./Project.bmp");

    std::cout << "\n\n" << std::endl;
    return 0;
}
