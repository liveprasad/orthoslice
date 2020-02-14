/*
 * IsoPolygon.h
 *
 *  Created on: 12-Jun-2015
 *      Author: liveprasad
 */

#ifndef ENTITIES_ISOPOLYGON_H_
#define ENTITIES_ISOPOLYGON_H_
#include "../CommonInclude.h"
#include "GridPoint.h"
namespace std {
using namespace std;
class IsoPolygon {
public:
	GridPoint 	*polyPoints;
	int 		size;
	double 		x_min;
	double 		x_max;
	double 		z_min;
	double 		z_max;
	int 		x_depth;
	int 		z_depth;
	int 		slice_x_dim;
	int 		slice_z_dim;
	int 		Perimeter;
	bool 		isHole;
	double 		sym_x;
	double 		sym_z;
	double 		slicing_coordinate;
	int 		sliceNum;
	int 		num;
	//! List of the polygons which are adjacent to this polygon in the reeb graph
	vector<IsoPolygon *>  adjPolygons;

	IsoPolygon();
	IsoPolygon(int , bool , int, int);
	~IsoPolygon();

	void 	reportPolyGon(string  );
    void 	setPolygon(GridPoint *);

	int 	getPerimeter();
	void 	findSymmetryAxis();
	double 	calAngle(GridPoint , GridPoint , GridPoint );
	void 	setAllPointsType();
	void 	calculateDepth();
	void 	findDoorPoints( vector<GridPoint> & );
	
	bool	checkIntersection(IsoPolygon*,const int  );
	void 	determineOpeningPoints(IsoPolygon, vector<GridPoint>  &);
	int 	getIsotheticDist(GridPoint ,GridPoint );
	void 	findDoorPointsWithX(vector<GridPoint> &);
	void 	findDoorPointsWithZ(vector<GridPoint> &);
	bool 	checkDoorPointValidation(pair<int , int> , IsoPolygon );
	bool 	checkValidationInCross(int , int, int ,int );


};



} /* namespace std */

#endif /* ENTITIES_ISOPOLYGON_H_ */
