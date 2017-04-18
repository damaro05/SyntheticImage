#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>
#include <cmath>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "shapes/sphere.h"
#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#define PI 3.14159265359

void transformationsExercise()
{
    /* *************** */
    /* TRANSFORMATIONS */
    /* *************** */
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";

    std::cout << separatorStar << "Construct and visualize transforms" << separatorStar << std::endl;

    // Identity Matrix
    Matrix4x4 idMat; // By default, a matrix4x4 is initialized with the identity matrix
    std::cout << "The content of matrix idMat is: \n" << std::endl;
    std::cout << idMat << separator << std::endl;

    // Translation Matrix
    double deltaX, deltaY, deltaZ;
    deltaX = 1; deltaY = -2; deltaZ = 3;
    Vector3D delta(deltaX, deltaY, deltaZ);
    Matrix4x4 translationMat = Matrix4x4::translate(delta);
    std::cout << "The content of matrix translationMat is: \n" << std::endl;
    std::cout << translationMat << separator << std::endl;

    // Scale Matrix
	double scaleX, scaleY, scaleZ;
	scaleX = 2; scaleY = 1; scaleZ = -1;
	Vector3D scale(scaleX, scaleY, scaleZ);
	Matrix4x4 scaleMatrix = Matrix4x4::scale(scale);
    std::cout << "The content of matrix scaleMatrix is: \n" << std::endl;
    std::cout << scaleMatrix << separator << std::endl;

    // Rotate around X Matrix
    double angleInDegrees = 60;
    double thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	double rotateX, rotateY, rotateZ;
	rotateX = 1; rotateY = 0; rotateZ = 0;
	Vector3D rotate(rotateX, rotateY, rotateZ);
	Matrix4x4 rotateXMatrix = Matrix4x4::rotate(thetaInRadians, rotate);
    std::cout << "The content of matrix rotateXMatrix is: \n" << std::endl;
    std::cout << rotateXMatrix << separator << std::endl;

    // Rotate around an arbitrary axis Matrix
	double angleInDegrees2 = 30;
	double thetaInRadians2 = Utils::degreesToRadians(angleInDegrees2);
	double rotateArbitraryX, rotateArbitraryY, rotateArbitraryZ;
	rotateArbitraryX = 1; rotateArbitraryY = 1; rotateArbitraryZ = 1;
	Vector3D rotateArbitrary(rotateArbitraryX, rotateArbitraryY, rotateArbitraryZ);
	Matrix4x4 rotateArtitraryAxisMatrix = Matrix4x4::rotate(thetaInRadians2, rotateArbitrary);
    std::cout << "The content of matrix rotateArtitraryAxisMatrix is: \n" << std::endl;
    std::cout << rotateArtitraryAxisMatrix << separator << std::endl;

    // Transposed and Inversion
    std::cout << separatorStar << "Inverting and Transposing a Matrix" << separatorStar << std::endl;
    Matrix4x4 translationMatTransposed;
    translationMat.transpose(translationMatTransposed);
    std::cout << "The transpose of matrix \n\n" << translationMat << "\n is \n\n"
              << translationMatTransposed << separator << std::endl;

    Matrix4x4 inverseTranslationMat;
	translationMat.inverse(inverseTranslationMat);
    std::cout << "The inverse of matrix \n\n" << translationMat << "\n is \n\n" << inverseTranslationMat << std::endl;
    std::cout << "And their multiplication should thus give the identity matrix:\n\n";
	std::cout << translationMat * inverseTranslationMat << separator << std::endl;

    // Combine here some transforms, and visualize the result
    std::cout << separatorStar << "Combine transforms and visualize the result" << separatorStar << std::endl;

    Matrix4x4 scaleTranslate = scaleMatrix * translationMat;
    std::cout << "The content of matrix scaleTranslate is: \n" << std::endl;
    std::cout << scaleTranslate << separator << std::endl;

    Matrix4x4 translateScale = translationMat * scaleMatrix;
    std::cout << "The content of matrix translateScale is: \n" << std::endl;
    std::cout << translateScale << separator << std::endl;
}

void normalTransformExercise()
{
    std::string separator = "\n----------------------------------------------\n";

    // NOTE: points, vectors and normals are represented, in this framework, using
    //  the Vector3D class. It is up to the programmer to take care of whether the
    //  Vector3D is representing a point, a vector or a normal. For example, when
    //  applying a transform using a Matrix4x4, the programmer must choose which
    //  "applyTransform" method of the namespace Matrix4x4 should be used.

    Vector3D v(1, 1, 0);
    Vector3D n(-1.0/sqrt(2), 1.0/sqrt(2), 0);
    Matrix4x4 S = Matrix4x4::scale(Vector3D(2, 1, 1));

    std::cout << "Vector v = " << v << "\n" << std::endl;
    std::cout << "Normal n = " << n << "\n" << std::endl;
    std::cout << "Scale Matrix: \n" << std::endl;
    std::cout << S << std::endl;

    Vector3D vTransformed = S.transformVector(v);
    std::cout << "Vector v\' = " << vTransformed << "\n" << std::endl;

	Vector3D nTransformed = S.transformVector(n);
	std::cout << "Vector n\' = " << nTransformed << "\n" << std::endl;

	double dotproduct = dot(nTransformed, vTransformed);
	std::cout << "dot(n\', v\') = " << dotproduct << "\n" << std::endl;

	Matrix4x4 STranspose;
	S.transpose(STranspose);
	Matrix4x4 STInverse;
	STranspose.inverse(STInverse);
	Vector3D nTransformedCorrect = STInverse.transformVector(n);
	std::cout << "Vector n\' = " << nTransformedCorrect << "\n" << std::endl;

	double dotproductCorrect = dot(nTransformedCorrect, vTransformed);
	std::cout << "dot(n\', v\') = " << dotproductCorrect << "\n" << std::endl;
}

void paintingAnImageExercise()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

    for(unsigned int col = 0; col < resX; col++)
        {
            for(unsigned int row = 0; row < resY; row++)
            {
                Vector3D color(((col+0.5)/resX), ((row+0.5)/resY), 0);
                film.setPixelValue(col, row, color);
            }
        }

    // Save the final result to file
    film.save("./paintingAnImage.bmp");
}


Vector3D blurFilter(Film &image, int lin, int col, int radius, bool isGaussian) {
	Vector3D currentPixel = (0, 0, 0);
	int resX, resY, numPixels = 0;
	float sumWeight = 0;
	resX = (int)image.getWidth();
	resY = (int)image.getHeight();
	
	for (int x = lin - radius; x <= lin + radius; x++) {
		for (int y = col - 1; y < col + 2; y++) {

			if (x >= 0 && x < resX && y >= 0 && y < resY) {
				if (isGaussian) {
					float dist = (x - lin)*(x - lin) + (y - col)*(y - col);
					float weight = exp( -dist / (2*radius*radius)) / (PI*2*radius*radius);
					currentPixel += image.getPixelValue(x, y) * weight;
					sumWeight += weight;
				}
				else {
					currentPixel += image.getPixelValue(x, y);
					numPixels++;
				}
			}
		}
	}
	if (isGaussian) return (currentPixel / (int)sumWeight);
	else return (currentPixel / numPixels);
}

void filteringAnImageExercise()
{
	// Create two instances of the film class with the same resolution
	int resX, resY;
	resX = 512;
	resY = 512;
	Film f1(resX, resY);
	Film f2(resX, resY);

	Film *image1 = &f1;
	Film *image2 = &f2;

	Film *image1bis = NULL;

	// Create the original image
	//  Draw a circle centered at centerX, centerY (in pixels, image space)
	//   and with ray r (also in pixels)
	int centerX = resX / 2;
	int centerY = resY / 2;
	int r = std::min(centerX, centerY) / 2;
	for (int lin = 0; lin < resX; lin++)
	{
		for (int col = 0; col < resY; col++)
		{
			// Use the equation of the sphere to determine the pixel color
			if ((lin - centerX)*(lin - centerX) + (col - centerY)*(col - centerY) < r*r)
				f1.setPixelValue(col, lin, Vector3D(1, 1, 0));


			//TESTING CORNERS AND BORDERS
			if ((lin - 100)*(lin - 100) + (col - 100)*(col - 100) < r*r)
				f1.setPixelValue(col, lin, Vector3D(1, 0, 0));

			if (lin >= 0 && lin < 15 && col >= 0 && col < 15) f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
			if (lin >= 0 && lin < 15 && col >= resY - 16 && col < resY) f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
			if (lin >= resX - 16 && lin < resX && col >= 0 && col < 15) f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
			if (lin >= resX - 16 && lin < resX && col >= resY - 16 && col < resY) f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
			
		}
	}

	f1.save("./blurred_original.bmp");
	image1bis = image1;

	// Filter-related variables
	// Declare here your filter-related variables
	// (e.g., FILTER SIZE)
	int iteraciones = 25;
	int fSize = 3;
	int radius = (fSize - 1) / 2;
	int numPixels = 0; //Numero de pixeles afectados por el filtro segun la posicion del centro
	bool isGaussian = true;
	Vector3D currentPixel = (0,0,0);
	
	// Implement here your image filtering algorithm
	for (int it = 0; it < iteraciones; it++) {
		for (int lin = 0; lin < resX; lin++)
		{
			for (int col = 0; col < resY; col++)
			{
				currentPixel = blurFilter(*image1, lin, col, radius, isGaussian);
				image2->setPixelValue(col, lin, currentPixel);
			}
		}
		Film *aux;
		aux = image1;
		image1 = image2;
		image2 = aux;
	}
    
    // DO NOT FORGET TO SAVE YOUR IMAGE!
	image1->save("./blurred_filter.bmp");
	
	/*
	image1 = image1bis;
	isGaussian = true;

	// Implement here your image filtering algorithm (Gaussian filter)
	for (int it = 0; it < iteraciones; it++) {
		for (int lin = 0; lin < resX; lin++)
		{
			for (int col = 0; col < resY; col++)
			{
				currentPixel = blurFilter(*image1, lin, col, radius, isGaussian);
				image2->setPixelValue(col, lin, currentPixel);
			}
		}
		Film *aux;
		aux = image1;
		image1 = image2;
		image2 = aux;
	}

	// DO NOT FORGET TO SAVE YOUR IMAGE!
	image1->save("./blurred_gaussian_filter.bmp");*/
}

void completeSphereClassExercise()
{
    // Make your intersection tests here
    // (....)
}

void eqSolverExercise()
{
    EqSolver solver;
    rootValues roots;

    double A, B, C;

    // (...)

	bool hasRoots = true;
    //bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        std::cout << "Equation has no real roots!" << std::endl;
    }
    else
    {
        // SHOW THE SOLUTIONS OF THE EQUATION
        // (...)
    }
}

void raytrace()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

    /* ******************* */
    /* Orthographic Camera */
    /* ******************* */
    Matrix4x4 cameraToWorld; // By default, this gives an ID transform
                             // meaning that the camera space = world space
    OrtographicCamera camOrtho(cameraToWorld, film);

    /* ******************* */
    /* Perspective Camera */
    /* ******************* */
    double fovRadians = Utils::degreesToRadians(60);
    PerspectiveCamera camPersp(cameraToWorld, fovRadians, film);

    // Save the final result to file
    film.save();
}

int main()
{
    std::string separator = "\n----------------------------------------------\n";

    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // ASSIGNMENT 1
    transformationsExercise();
    normalTransformExercise();
    paintingAnImageExercise();
    filteringAnImageExercise();

    // ASSIGNMENT 2
    //eqSolverExercise();
    //completeSphereClassExercise();
    //raytrace();

    std::cout << "\n\n" << std::endl;
    return 0;
}
