/*
 * Grid.h
 *
 *  Created on: 26-Jun-2015
 *      Author: liveprasad
 */

#ifndef ENTITIES_GRID_H_
#define ENTITIES_GRID_H_
#include "Constants.h"
#include "../CommonInclude.h"
#include "IsoPolygon.h"
#include "Point.h"
using namespace std;

namespace std{

class Grid {
public:
	
	Point 		*v;
	int 		totalPoints;
	int 		totalPolygons;
	int 		xgridDim;
	int 		zgridDim;
	int 		gridStartX;
	int 		gridStartZ;
	int 		gridEndX;
	int 		gridEndZ;
	int 		first_dimesion;
	int 		second_dimesion;
	int 		sliceNum;
	double 		canvas_x_len ;
	double 		canvas_z_len ;
	//! Name of the corresponding svg file associated with this slice.
	string 		filename;
	//! List of points of the polygon in anticlockwise manner.
	GridPoint* 	polyPoints;
	//! List of all isothetic polygons in this grid object.
	IsoPolygon*	gridPolygons;
	IsoPolygon*	outerPolygon;
	vector<IsoPolygon*> outerPolygonsList;
	double 		gridmin;
	double 		MinIsoDist;
	int    		gridsize;
	double 		minX,minZ;
	double 		maxX,maxZ;
	Point 		pair1,pair2;
	double 		Grid_x_sym, Grid_z_sym;
	double 		slicing_coordinate;
	//! Class to store the information of the perticlar square cell of this 2D grid.
	class GridCell{
	public:
		GridCell(){}
		bool isvisited;
		bool isFilled;
		int x;
		int z;

	};
	GridCell** 	filledGrid;
	vector<GridPoint> DoorPoints;


	Grid();
	Grid(int);
	~Grid();

	void 		setGridDimesions();
	void 		find_min_max();
	void 		find_Min();
	void 		change_Min_Max(Point P);
	void 		find_Min(Point *points);

	void 		getMinof_25_Dist(int i,int j,Point P,map<string,Point> &gridMap,double &min,bool iscal);
	double 		getEcludianDist(Point P1,Point P2);
	double 		getIsotheticDist(Point P1,Point P2);
	void 		changeMin(string index, double &min, map<string,Point> &gridMap,Point P);
	void 		rehash(Point* poinmismatchts,map<string,Point> &gridMap,double hashLength,int size);
	string 		getIndex(Point P,double hashLength);

	void 		findGridSize(double f);
	void 		fillGrid(string sliceAxis);


	void 		printGrid();
	void 		printSliceSymmetry();
	void 		printPoints();
	void 		printStart();

	void 		TraversingAlgorithm(string sliceAxis,int gridMaxX,int gridMaxZ );
	
	void 		reportPolyGon(string filename,IsoPolygon &s);
	static bool compareForPerimeter(IsoPolygon , IsoPolygon );
	void 		findSliceSymmetry(int comp_par);
	/*void fillgridColor(string filename ,int xgridDim,int zgridDim );
	void fillgridIntermediate(string filename,int xgridDim,int zgridDim );*/
	
	void 		setAllPointsType();
	void 		DeleteGrid();
	int 		Push(int polyPointsCount,GridPoint &g , int );
	void 		printDoorPoints(string  );
	void 		printEnd();



};

} /* namespace std */

#endif /* ENTITIES_GRID_H_ */
