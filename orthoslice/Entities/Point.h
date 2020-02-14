

/*
 * Point.h
 *
 *  Created on: 10-Jun-2015
 *      Author: liveprasad
 */

#ifndef ENTITIES_POINT_H_
#define ENTITIES_POINT_H_
 #include "../CommonInclude.h"
#include "Constants.h"
#include <stdlib.h>
#include <cmath>
namespace std{

class Point {
public:
	double x;
	double y;
	double z;
	Point();
	Point(double x,double y, double z);
	void addPoint(Point P);
	double getDistance(Point p);
	bool equals(Point P);
	bool compareAlongY(Point p);
	bool compareAlongZ(Point);
	bool compareAlongX(Point);


};

} /* namespace std */
#endif /* ENTITIES_POINT_H_ */
