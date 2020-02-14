/*
 * GridPoint.cpp
 *
 *  Created on	: 12-Jun-2015
 *      Author	: liveprasad
 *  Description	:
 */

#include "GridPoint.h"
namespace std {

GridPoint::GridPoint() {
	// TODO Auto-generated constructor stub

}

GridPoint::GridPoint(int x, int z){

	this->x = x;
	this->z = z;

}

bool GridPoint::equals(GridPoint g){


	if(this->x == g.x && this->z == g.z)
		return true;

	return false;
}

int GridPoint::getIsotheticDist(GridPoint g){

	int diff_x = abs(this->x - g.x);
	int diff_z = abs(this->z - g.z);

	return (diff_x > diff_z) ? diff_x : diff_z;
}

} /* namespace std */
