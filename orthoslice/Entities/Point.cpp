
//! Point Class contains x, y, z co-ordinates representing real point in 3D space
/*!
 * Created on	: 10-Jun-2015
 * Author		: Prasad Shetkar
 */

#include "Point.h"
namespace std {
Point::Point() {
	// TODO Auto-generated constructor stub

}


Point::Point(double x,double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;

}
void Point::addPoint(Point P){
	this->x += P.x;
	this->y += P.y;
	this->z += P.z;
}
//! Gets the distance of current point from point p passed as argument
double Point::getDistance(Point p){
	//printf("%lf %lf %lf \n",center->x,center->y,center->z);
	return sqrt((this->x-p.x)*(this->x-p.x) +
			(this->y-p.y)*(this->y-p.y)+
			(this->z-p.z)*(this->z-p.z)
	);
}
//! Checks if current point is equal to point p passed as argument(while we are dealing with 2D space in the orthogonal slicing we ingnore y dimesion to reuse same structure in 2D space)
bool Point::equals(Point P){
	if(abs(this->x-P.x) <= EPSILON && abs(this->z-P.z)<=EPSILON)
		return true;
	return false;
}

//! Compare current point with paased point p along Y co-ordinates
bool Point::compareAlongY(Point p){
	if(this->y < p.y )
		return true;
	else
		return false;

}
//! Compare current point with paased point p along Z co-ordinates
bool Point::compareAlongZ(Point p){
	if(this->z < p.z)
			return true;
		else
			return false;

}
//! Compare current point with paased point p along X co-ordinates
bool Point::compareAlongX(Point p){
	if(this->x < p.x)
			return true;
		else
			return false;

}

} /* namespace std */
