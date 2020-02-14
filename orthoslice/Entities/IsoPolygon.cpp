/*
 * IsoPolygon.cpp
 *
 *  Created on: 12-Jun-2015
 *      Author: liveprasad
 */

#include "IsoPolygon.h"

using namespace std;
namespace std {

IsoPolygon::IsoPolygon(){
	this->isHole 		= false;
}

IsoPolygon::IsoPolygon(int size, bool _isHole,int slice_num, int _num) {
	// TODO Auto-generated constructor stub
	this->isHole 		= _isHole;
	this->x_max 		= -10000;
	this->x_min 		=  10000;
	this->z_max 		= -10000;
	this->z_min 		=  10000;
	z_depth 			= -5000;
	x_depth 			= -5000;
	Perimeter   		= 0;
	this->size 			= size;
	num 				= _num;
	this->sliceNum 	= slice_num;


}

IsoPolygon::~IsoPolygon(){
	
}

//!Prints the polygon points in the respective svg file of the slice to which this polygon belong.
/*!
 *\param filename 	name of the svg file in which the polygon should be printed
 *\param num 		It is the number of the polygon in the polygon list of the corresponding grid. 
 */

void IsoPolygon::reportPolyGon(string filename){

	fstream fw;
	
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app );
	//This line prints the number of the polygon in the corresponding slice.
	//fw  <<"<text x=\""<<polyPoints[0].x<<"\" y=\""<<polyPoints[0].z<<"\" text-anchor=\"middle\" font-size=\"20px\" fill=\"#aaaaaa\">"<< this->num<<"</text>"<<endl;
	
	//It is used to print H for the hole in the corresponding file.
	/*if( isHole)
		fw  <<"<text x=\""<<polyPoints[0].x<<"\" y=\""<<polyPoints[0].z<<"\" text-anchor=\"middle\" font-size=\"20px\" fill=\"#aaaaaa\">H</text>"<<endl;*/
	
	fw<< "<polygon points=\"";
	bool flag = true;
	GridPoint gg;
	for(int i = 0; i< size; i++){
		GridPoint g = polyPoints[i];
		fw<<g.x<<","<<g.z<<" ";
		if(this->x_min  > g.x)
			this->x_min = g.x;
		if(this->z_min  > g.z)
			this->z_min = g.z;
		if(this->x_max  < g.x)
			this->x_max = g.x;
		if (this->z_max < g.z)
			this->z_max = g.z;

		if (flag && g.type == 3){
			gg 		= g;
			flag 	= false;
			 //s =  "<circle cx=\"" +  to_string(g.x,10) +"\" cy=\"" + to_string(g.z,10) +"\" r=\"4\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" />" ;
		}

	}
	fw<<"\" style=\"fill:none;stroke:black;stroke-width:0.7\" />" << endl;
	//fw<<"<circle cx=\"" <<  gg.x <<"\" cy=\"" << gg.z <<"\" r=\"1\" stroke=\"black\" stroke-width=\"0\" fill=\"red\" />"  << endl;
	//fw << s << endl;
	fw.close();
}

//!It detects whether current isothetic polygon intersects with the isothetic polygon p passed as an argument. 
/*!
* \param p 			It is a pointer to the isothetic polygon with which we have to check the intersection of the current polygon
* \param gridsize 	It is the size of underlying grid.
* \return 			true if two polygon intersect, otherwise false.
*/
bool IsoPolygon::checkIntersection(IsoPolygon *p, const int g){

	int gridsize = g;

	int bound_box_x_min = min(this->x_min , p->x_min);
	int bound_box_z_min = min(this->z_min , p->z_min);
	int dim_X 	= (max(this->x_max, p->x_max) - bound_box_x_min)/ gridsize ;
	int dim_Z 	= (max(this->z_max, p->z_max) - bound_box_z_min)/gridsize ;
	int mat[dim_X+1][dim_Z +1];

	//cout << "Dimesions:" <<dim_X  << " " << dim_Z << endl;
	memset(mat, 0,sizeof(mat));

	for (int j = 1; j <= this->size ; ++j){
	 			
		GridPoint 	g1 		= this->polyPoints[(j-1)];
		GridPoint 	g2 		= this->polyPoints[j%this->size];		
		int 		row_in 	= (g1.x - bound_box_x_min)/ gridsize ;
		int 		col_in 	= (g1.z - bound_box_z_min)/ gridsize ;
		
		if(g1.x == g2.x ){

			int z1 		= g1.z;
			int z2      = g2.z;
			int sign	= (z2-z1)/ abs(z2-z1);
			while(z1 != z2){				
				z1 		=  z1 + (sign * gridsize);
				col_in 	= (z1 - bound_box_z_min)/ gridsize ;
				mat[row_in][col_in]++;
			}

		}
		else {

			int x1 		= g1.x;
			int x2      = g2.x;
			int sign	= (x2-x1)/ abs(x2-x1);

			while(x1 != x2){

				x1 		+= (sign * gridsize);
				row_in 	= (x1 - bound_box_x_min)/ gridsize;
				mat[row_in][col_in]++ ;
			}

		}
		
	}

	for (int j = 1; j <= p->size ; ++j){
	 			
		GridPoint 	g1 		= p->polyPoints[(j-1)];
		GridPoint 	g2 		= p->polyPoints[j%p->size];		
		int 		row_in 	= (g1.x - bound_box_x_min)/ gridsize ;
		int 		col_in 	= (g1.z - bound_box_z_min)/ gridsize ;
		
		if(g1.x == g2.x ){

			int z1 		= g1.z;
			int z2      = g2.z;
			int sign	= (z2-z1)/ abs(z2-z1);
			while(z1 != z2){				
				z1 		=  z1 + (sign * gridsize);
				col_in 	= (z1 - bound_box_z_min)/ gridsize ;				
				//printf("row : %d\t col : %d\n", row_in, col_in);
				if( mat[row_in][col_in])
					return 1;
				mat[row_in][col_in]++;
			}

		}
		else {

			int x1 		= g1.x;
			int x2      = g2.x;
			int sign	= (x2-x1)/ abs(x2-x1);
			while(x1 != x2){

				x1 		+= (sign * gridsize);
				row_in 	= (x1 - bound_box_x_min)/ gridsize ;				
				//printf("row : %d\t col : %d\n", row_in, col_in);
				if( mat[row_in][col_in])
					return 1;
				mat[row_in][col_in]++ ;
			}
		}		
	}
	return 0;
}




 //Calculates the perimeter of the polygon
 /*!
 * \return	Interger value that depicts the polygon perimeter
 */
int IsoPolygon::getPerimeter(){

	if(Perimeter != 0)
		return Perimeter;
	
	Perimeter = 0;
	for(int i = 0; i< size; i++){
		GridPoint g = polyPoints[i]; 
		Perimeter = Perimeter + g.getIsotheticDist(polyPoints[(i+1)%size]);

	}
	return Perimeter;
}

void IsoPolygon::findSymmetryAxis(){

	this->sym_x = (this->x_max + this->x_min)/2;
	this->sym_z = (this->z_max + this->z_min)/2;
}

double IsoPolygon::calAngle(GridPoint P1,GridPoint P0,GridPoint P2){

	//define P0P1 = {P1.x-P0.x,P1.y-P0.y}
	//define P0P2 = {P2.x-P0.x,}
	//cross product =

	double  x1 = P1.x-P0.x,
			y1 = P1.z-P0.z,
			x2 = P2.x-P0.x,
			y2 = P2.z-P0.z;
	double dot = (x1*x2) + (y1 * y2); // dot product
	double cross = (x1 * y2 - x2 * y1); // cross product

	double alpha = atan2(cross, dot);

	return (alpha * 180. / M_PI );

}

//! It finds all the door points in this polygon.
/*! 
* \param grid_z_sym It depocts the symmetry of grid along z axis 
* \param grid_x_sym It depicts the symmetry of grid along x axis
*/
void IsoPolygon::findDoorPoints( vector<GridPoint> & DoorPoints ){

	findDoorPointsWithX(DoorPoints);
	findDoorPointsWithZ(DoorPoints);

	
}

void IsoPolygon::findDoorPointsWithX(vector<GridPoint> &DoorPoints){

	pair<int , int> pp;
	int dist = INT_MAX;
	for (int i = 0; i < size; ++i){		
		// find out the points of the opening where it is on the left side of the opening.
		if( this->polyPoints[i].type == 1 && this->polyPoints[ (i+1) % size].type == 1 && 
			this->polyPoints[i].x == this->polyPoints[ (i+1) % size].x && this->polyPoints[ (i+2) % size].x < this->polyPoints[i].x  ){

			for (int j = 0; j < size; ++j){

				if( this->polyPoints[j].type == 1 && this->polyPoints[ (j+1) % size].type == 1 && 
					this->polyPoints[j].x == this->polyPoints[ (j+1) % size].x && this->polyPoints[(j+2) % size].x > this->polyPoints[j].x  ){

					int ndist = getIsotheticDist(this->polyPoints[i],this->polyPoints[j]);

					if(  this->polyPoints[i].x < this->polyPoints[j].x && ndist < dist){

						pp = make_pair(i,j);
						dist = ndist;

					}
				}
			}
		}
	}	

	if(dist != INT_MAX ){			
			DoorPoints.push_back( this->polyPoints[pp.first]);
			DoorPoints.push_back(this->polyPoints[(pp.first+1) %size]);
			DoorPoints.push_back( this->polyPoints[pp.second]);
			DoorPoints.push_back( this->polyPoints[(pp.second+1) %size]);
	}

}


void IsoPolygon::findDoorPointsWithZ(vector<GridPoint> &DoorPoints){

	pair<int , int> pp;
	int dist = INT_MAX;
	for (int i = 0; i < size; ++i){		
		// find out the points of the opening where it is on the left side of the opening.
		if( this->polyPoints[i].type == 1 && this->polyPoints[ (i+1) % size].type == 1 && 
			this->polyPoints[i].z == this->polyPoints[ (i+1) % size].z && this->polyPoints[ (i+2) % size].z < this->polyPoints[i].z  ){

			for (int j = 0; j < size; ++j){

				if( this->polyPoints[j].type == 1 && this->polyPoints[ (j+1) % size].type == 1 && 
					this->polyPoints[j].z == this->polyPoints[ (j+1) % size].z && this->polyPoints[(j+2) % size].z > this->polyPoints[j].z  ){

					int ndist = getIsotheticDist(this->polyPoints[i],this->polyPoints[j]);

					if(  this->polyPoints[i].z < this->polyPoints[j].z && ndist < dist){

						pp = make_pair(i,j);
						dist = ndist;

					}
				}
			}
		}
	}

	

	if(dist != INT_MAX){			
			DoorPoints.push_back( this->polyPoints[pp.first]);
			DoorPoints.push_back(this->polyPoints[(pp.first+1) %size]);
			DoorPoints.push_back( this->polyPoints[pp.second]);
			DoorPoints.push_back( this->polyPoints[(pp.second+1) %size]);
	}

}



void IsoPolygon::setAllPointsType(){

	

	for (int i = 1; i <= size; ++i) {
		GridPoint g =  polyPoints[i%size];
		if((int)calAngle(polyPoints[(i+1)%size],polyPoints[i%size],polyPoints[(i-1)%size]) == 90){

			/*cout<< "type 1" << endl;
			cout<< polyPoints[(i+1)%size].x <<" " << polyPoints[(i+1)%size].z <<  endl;
			cout<< polyPoints[i%size].x << polyPoints[i%size].z <<  endl;
			cout<< polyPoints[(i-1)%size].x << polyPoints[(i-1)%size].z <<  endl;*/
			// inside angle is 90
			//Points are stored in anticlockwise manner in polyPoints data structure.
			g.type = 1;
		}
		else{
			//inside angle is 270 
			g.type = 3;
		}

		polyPoints[i%size] = g;
	}
}

void IsoPolygon::setPolygon(GridPoint *Polypoints){
	
	polyPoints = new GridPoint[size];
	memcpy(polyPoints,Polypoints ,size * sizeof(GridPoint));
}

void IsoPolygon::calculateDepth(){	

	if(x_depth >=0 || z_depth >= 0)
		return;
	for(int i = 1; i <=size; ++i) {
        GridPoint g =  polyPoints[i%size];
        GridPoint g_1 = polyPoints[i-1];
		if(g_1.type == 3 && g.type == 3){
			GridPoint prev,next;
			if(i == 1){
				prev = polyPoints[size-1];
			}
			else
				prev = polyPoints[i-2];
			next = polyPoints[(i+1)%size];
			if(g_1.x == g.x){

				int depth = 0;
				if(next.x < g.x ){

					depth = g.x - x_min;
				}
				else
					depth = x_max -  g.x;
				if(x_depth < depth){
					x_depth = depth;
				}

			}
			else{
				int depth = 0;
				if(next.z < g.z ){

					depth = g.z - z_min;
				}
				else
					depth = z_max -  g.z;
				if(z_depth < depth){
					z_depth = depth;
				}
			}
		}
	}

	if(x_depth < 0)
		x_depth = 0;
	if(z_depth < 0)
		z_depth = 0;
}


void IsoPolygon::determineOpeningPoints(IsoPolygon p ,  vector<GridPoint>  &DoorPoints){

	int dist = 100000;
	cout << "between " << this->num << ":" << p.num << endl;
	pair<int, int> pp, finalPP;
	bool found = false;
	for (int i = 0; i < p.size; ++i){

		if( p.polyPoints[i].type == 1 &&  p.polyPoints[(i+1) %p.size].type == 1 )
			for ( int j = 0; j < this->size; ++j){

				if(this->polyPoints[j].type == 1 && this->polyPoints[(j+1) %this->size].type == 1 ){

					if( (p.polyPoints[i].x ==   p.polyPoints[(i+1) %p.size].x  &&
						this->polyPoints[j].x == this->polyPoints[(j+1) %this->size].x 
						) 	 ||

						(p.polyPoints[i].z ==   p.polyPoints[(i+1) %p.size].z  &&
						this->polyPoints[j].z == this->polyPoints[(j+1) %this->size].z 
						) 
						){
						int ndist1 = getIsotheticDist(this->polyPoints[j],p.polyPoints[i]);
						int ndist2 = getIsotheticDist(this->polyPoints[(j+1)%size],p.polyPoints[i]);
						int ndist3 = getIsotheticDist(this->polyPoints[j],p.polyPoints[(i+1)%p.size]);
						int ndist4 = getIsotheticDist(this->polyPoints[(j+1)% size],p.polyPoints[(i+1)% p.size]);
						
						int ndist = min(ndist1, ndist2);
						ndist = min(ndist, ndist3);
						ndist = min(ndist, ndist4);

						cout <<p.polyPoints[i].x << ":"<< p.polyPoints[i].z << "  ";
						cout <<p.polyPoints[(i+1) %p.size].x <<":" <<p.polyPoints[(i+1) %p.size].z << "  " ;
						cout <<this->polyPoints[j].x << ":" <<this->polyPoints[j].z << "  ";
						cout <<this->polyPoints[(j+1) % size].x << ":" << this->polyPoints[(j +1)% this->size].z << endl ;
						cout << ndist1 << " " << ndist2 << " " <<ndist3 <<" " <<ndist4 << " "  << ndist << " "<< dist << endl; 
						pp = make_pair(i, j);
						if( dist > ndist  && checkDoorPointValidation(pp,p)){
							dist   	= ndist;
							finalPP = pp;							
							found 	= true;
							cout << "changed" << endl;
							
						}
					}
				}
			}
	}


	

	if(found){
		cout <<"final ";
		cout <<p.polyPoints[finalPP.first].x << ":"<< p.polyPoints[finalPP.first].z << "  ";
		cout <<p.polyPoints[(finalPP.first+1) %p.size].x <<":" <<p.polyPoints[(finalPP.first+1) %p.size].z << "  " ;
		cout <<this->polyPoints[finalPP.second].x << ":" <<this->polyPoints[finalPP.second].z << "  ";
		cout <<this->polyPoints[(finalPP.second+1) % size].x << ":" << this->polyPoints[(finalPP.second +1)% this->size].z << endl ;


		DoorPoints.push_back( p.polyPoints[finalPP.first]);
		DoorPoints.push_back( p.polyPoints[(finalPP.first+1) %p.size]);
		DoorPoints.push_back( this->polyPoints[finalPP.second]);
		DoorPoints.push_back( this->polyPoints[(finalPP.second+1) %size]);
	}
		
	
	//return dist;
}

bool IsoPolygon:: checkDoorPointValidation(pair<int , int> pp, IsoPolygon p){


	if( p.polyPoints[ (pp.first + 1)% p.size].x == p.polyPoints[pp.first].x &&
	
		checkValidationInCross(p.polyPoints[pp.first].z,p.polyPoints[ (pp.first + 1)% p.size].z,this->polyPoints[pp.second].z , this->polyPoints[(pp.second+1)%size].z)
	){
		cout << "found opening first case(x similar) in " << this->sliceNum << " between " << this->num << ":" << p.num<< endl;
		return true;
	}
		
	else if( p.polyPoints[ (pp.first + 1)% p.size].z == p.polyPoints[pp.first].z  &&
				
		checkValidationInCross(p.polyPoints[pp.first].x,p.polyPoints[ (pp.first + 1)% p.size].x,this->polyPoints[pp.second].x , this->polyPoints[(pp.second+1)%size].x)
				
	){
			
		cout << "found opening second case(z similar) in " << this->sliceNum << " between " << this->num << ":" << p.num<< endl;
		return true;
	}

		return false;
}


bool IsoPolygon:: checkValidationInCross(int x1, int x2, int x3 ,int x4){

	if( x1 > x2  &&  ((x1 >= x3 && x3 >= x2) || (x1 >= x4 && x4 >= x2 )))
		return 1;
	else if( x2 > x1 && ((x2 >= x3 && x3 >= x1) || (x2 >= x4 && x4 >= x1 )) )
		return 1;
	else if( x3 > x4 && (( x3 >= x1 && x1 >= x4) || ( x3 >= x2 && x2 >= x4)) )
		return 1;
	else if( x4 > x3 && (( x4 >= x1 && x1 >= x3) || ( x4 >= x2 && x2 >= x3)))
		return 1;
	return 0;
}

int IsoPolygon:: getIsotheticDist(GridPoint P1,GridPoint P2){

	int val = abs( P1.x-P2.x);
	int val1 = abs( P1.z-P2.z);
	
	return max (val,val1);
}

} /* namespace std */
