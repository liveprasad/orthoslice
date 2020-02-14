
#include "CommonInclude.h"
#include "Entities/Point.h"
#include "Entities/Constants.h"
#include "Entities/GridPoint.h"
#include "Entities/IsoPolygon.h"
#include "Entities/Grid.h"
#include "Entities/Segment.h"
#include "Entities/ReebGraph.h"
using namespace std;
double canvas_x_len ;
double canvas_z_len ;
static int sliceNum;
static int scaleFac;
static int first_dimesion, second_dimesion;

static int s;


vector<Segment> segments;
vector<Point> Readed_points;



// functions prototypes

ReebGraph* buildReebGraph( vector<Grid*>  , int );
void findMultipleOpeningsByReebGraph(vector<Grid*>  grids,ReebGraph *r);

struct pointCmpY {
    bool operator()( Point& p1,  Point& p2) {
     return p1.compareAlongY(p2);
  }
};

/*struct pairHash
{
	public:
		template<typename T, typename U>
		size_t operator()(const pair<T,U> &x) const{

			return hash<T>()(x.first) ^ hash<U>()( x.second);
		}
};*/

static bool compareAlongY(Point p1, Point p2){
	return p1.compareAlongY(p2);
}

static bool compareAlongZ(Point p1, Point p2){
	return p1.compareAlongZ(p2);
}

static bool compareAlongX(Point p1, Point p2){
	return p1.compareAlongX(p2);
}

/*!

*/
void meshlab_points_faces(FILE *fp,int i,int j,int k, int g)
{	
	//printf("%d %d %d %d\n",i, j, k, g);
	fprintf(fp,"v %d %d %d\n",i,j,k-g);
	fprintf(fp,"v %d %d %d\n",i,j,k);
	fprintf(fp,"v %d %d %d\n",i,j+g,k-g);
	fprintf(fp,"v %d %d %d\n",i,j+g,k);
	fprintf(fp,"v %d %d %d\n",i+g,j,k-g);
	fprintf(fp,"v %d %d %d\n",i+g,j,k);
	fprintf(fp,"v %d %d %d\n",i+g,j+g,k-g);
	fprintf(fp,"v %d %d %d\n",i+g,j+g,k);

	fprintf(fp,"f %d %d %d %d\n",s+8,s+4,s+2,s+6);
	fprintf(fp,"f %d %d %d %d\n",s+8,s+6,s+5,s+7);
	fprintf(fp,"f %d %d %d %d\n",s+8,s+7,s+3,s+4);
	fprintf(fp,"f %d %d %d %d\n",s+4,s+3,s+1,s+2);
	fprintf(fp,"f %d %d %d %d\n",s+1,s+3,s+7,s+5);
	fprintf(fp,"f %d %d %d %d\n",s+2,s+1,s+5,s+6);
	s+=8;
	return;
}
/*!
It generates the cube by making P(i,j,k) as center point
*/
void meshlab_points_faces(FILE *fp,double i,double j,double k)
{
	fprintf(fp,"v %f %f %f\n",i-0.5,j-0.5,k-0.5);
	fprintf(fp,"v %f %f %f\n",i-0.5,j-0.5,k+0.5);
	fprintf(fp,"v %f %f %f\n",i-0.5,j+0.5,k-0.5);
	fprintf(fp,"v %f %f %f\n",i-0.5,j+0.5,k+0.5);
	fprintf(fp,"v %f %f %f\n",i+0.5,j-0.5,k-0.5);
	fprintf(fp,"v %f %f %f\n",i+0.5,j-0.5,k+0.5);
	fprintf(fp,"v %f %f %f\n",i+0.5,j+0.5,k-0.5);
	fprintf(fp,"v %f %f %f\n",i+0.5,j+0.5,k+0.5);

	fprintf(fp,"f %d %d %d %d\n",s+8,s+4,s+2,s+6);
	fprintf(fp,"f %d %d %d %d\n",s+8,s+6,s+5,s+7);
	fprintf(fp,"f %d %d %d %d\n",s+8,s+7,s+3,s+4);
	fprintf(fp,"f %d %d %d %d\n",s+4,s+3,s+1,s+2);
	fprintf(fp,"f %d %d %d %d\n",s+1,s+3,s+7,s+5);
	fprintf(fp,"f %d %d %d %d\n",s+2,s+1,s+5,s+6);
	s+=8;
	return;
}


//! It creats the obj file for the output for slicing along Y axis( i.e slice of zx plane).This method is called on readed points as we rescale the scaled object to original state to show entire output of the segmentation before calling this method.
/*!
* \param outFilename It is the name of the output obj file 
*/
void create_obje_file_Y( string outFileName){


	FILE *fp_output = fopen(outFileName.c_str(),"w");
	fclose(fp_output);
	fp_output = fopen(outFileName.c_str(),"a");
	sort(Readed_points.begin(),Readed_points.end(),compareAlongY);
	int size = Readed_points.size();
	fprintf(fp_output,"mtllib temple.mtl\n");
	int material_number = 0 ;
	//int number_value = 0;
	int segement_number = 0;
	int end_slice_num = segments[segement_number].slices.size() - 1;
	double y_co = segments[segement_number].slices[end_slice_num]->slicing_coordinate;
	fprintf(fp_output,"usemtl symmetryPlane%d\n",material_number+1);
	for(int i = 0 ; i< size; i++){

		if(y_co <= Readed_points[i].y){
			material_number++;
			material_number = material_number%5;
			segement_number ++;
			end_slice_num = segments[segement_number].slices.size() - 1;
			y_co = segments[segement_number].slices[end_slice_num]->slicing_coordinate;
			fprintf(fp_output,"usemtl symmetryPlane%d\n",(material_number+1));
		}

		meshlab_points_faces(fp_output,Readed_points[i].x,Readed_points[i].y, Readed_points[i].z );
	}
	fclose(fp_output);

}

//! It creats the obj file for the output for slicing along Z axis( i.e slice of xy plane). This method is called on readed points as we rescale the scaled object to original state to show entire output of the segmentation before calling this method.
/*!
* \param outFilename It is the name of the output obj file 
*/



void create_obje_file_Z( string outFileName){

	FILE *fp_output = fopen(outFileName.c_str(),"w");
	fclose(fp_output);
	fp_output = fopen(outFileName.c_str(),"a");
	sort(Readed_points.begin(),Readed_points.end(),compareAlongZ);
	int size = Readed_points.size();
	fprintf(fp_output,"mtllib temple.mtl\n");
	int material_number = 0 ;
	//int number_value = 0;
	int segement_number = 0;
	int end_slice_num = segments[segement_number].slices.size() - 1;
	double z_co = segments[segement_number].slices[end_slice_num]->slicing_coordinate;
	fprintf(fp_output,"usemtl symmetryPlane%d\n",material_number+1);
	for(int i = 0 ; i< size; i++){

		if(z_co <= Readed_points[i].z){
			material_number++;
			material_number = material_number%5;
			segement_number ++;
			end_slice_num = segments[segement_number].slices.size() - 1;
			z_co = segments[segement_number].slices[end_slice_num]->slicing_coordinate;
			fprintf(fp_output,"usemtl symmetryPlane%d\n",(material_number+1));
		}

		meshlab_points_faces(fp_output,Readed_points[i].x,Readed_points[i].y, Readed_points[i].z );
	}
	fclose(fp_output);

}


//! It creats the obj file for the output for slicing along X axis( i.e slice of zy plane).This method is called on readed points as we rescale the scaled object to original state to show entire output of the segmentation before calling this method.
/*!
* \param outFilename It is the name of the output obj file 
*/

void create_obje_file_X(string outFileName){

	FILE *fp_output = fopen(outFileName.c_str(),"w");
	fclose(fp_output);
	fp_output = fopen(outFileName.c_str(),"a");
	sort(Readed_points.begin(),Readed_points.end(),compareAlongX);
	int size = Readed_points.size();
	fprintf(fp_output,"mtllib temple.mtl\n");
	int material_number = 0 ;
	//int number_value = 0;
	int segement_number = 0;
	int end_slice_num = segments[segement_number].slices.size() - 1;
	double x_co = segments[segement_number].slices[end_slice_num]->slicing_coordinate;
	fprintf(fp_output,"usemtl symmetryPlane%d\n",material_number+1);
	for(int i = 0 ; i< size; i++){

		if(x_co <= Readed_points[i].x){
			material_number++;
			material_number = material_number%5;
			segement_number ++;
			end_slice_num = segments[segement_number].slices.size() - 1;
			x_co = segments[segement_number].slices[end_slice_num]->slicing_coordinate;
			fprintf(fp_output,"usemtl symmetryPlane%d\n",(material_number+1));
		}

		meshlab_points_faces(fp_output,Readed_points[i].x,Readed_points[i].y, Readed_points[i].z );
	}
	fclose(fp_output);

}

//! It creates the obj file which shows the symmetry of the object along xz plane.
/*!
* \param outFileName the name of the output obj file 
*/
void create_obje_file_symmetry(string outFileName){

	FILE *fp_output = fopen(outFileName.c_str(),"w");
	fclose(fp_output);
	fp_output = fopen(outFileName.c_str(),"a");
	int size = Readed_points.size();
	fprintf(fp_output,"mtllib temple.mtl\n");
	for(int i = 0 ; i< size; i++){
		meshlab_points_faces(fp_output,Readed_points[i].x,Readed_points[i].y, Readed_points[i].z );
	}
	fclose(fp_output);
}

//! Creates voxelization of object by constructing one voxel per filled unit grid block in each slice and shows the result of the door.
/*!
* 	\param	outFileName  	Filename of the output obj file to visualize voxelization.
*	\param	grids  			Vector of grid objects which stores information of all the slices along perticular plane.
* 	\param 	axis 			Name of the slicing plane 
*/ 

void createVoxelsByGrid_door(string outFileName , vector<Grid*> grids, string axis){

	FILE *fp_output = fopen(outFileName.c_str(),"w");
	fclose(fp_output);
	fp_output = fopen(outFileName.c_str(),"a");
	fprintf(fp_output,"mtllib temple.mtl\n");
	fprintf(fp_output,"usemtl symmetryPlane\n");

	
	for (int k = 0; k < grids.size(); ++k){
		
		Grid *g = grids[k];
	
	
		for (int i = 0; i < g->xgridDim; ++i){
			for (int j = 0; j < g->zgridDim; ++j){
				if(g->filledGrid[i][j].isFilled){
					bool f = false;					
					//printf("%d %d\n",g->filledGrid[i][j].x,g->filledGrid[i][j].z );
					for (int l = 0; l < g->DoorPoints.size(); ++l){
						if( (g->filledGrid[i][j].x == g->DoorPoints[l].x || g->filledGrid[i][j].x+g->gridsize == g->DoorPoints[l].x) &&
						    (g->filledGrid[i][j].z == g->DoorPoints[l].z ||  g->filledGrid[i][j].z+g->gridsize == g->DoorPoints[l].z) ){
							f = true;
							break;
						}
					}
					
					// f is the flag used to determine if current cell is forming the door boundary or not.
					if(f){
						//In case the cell represent the door boundary we color is by using differet material
						fprintf(fp_output,"usemtl symmetryPlane%d\n",5);
					}

					// Reconstruct the object in originally readed axis order
					if(axis.compare("zy") == 0){
						meshlab_points_faces(fp_output, k*g->gridsize, g->filledGrid[i][j].z, g->filledGrid[i][j].x,g->gridsize );
					}
					else if(axis.compare("yx") == 0){
						meshlab_points_faces(fp_output,g->filledGrid[i][j].z ,g->filledGrid[i][j].x, k*g->gridsize, g->gridsize );
					}
					else if(axis.compare("xz") == 0){
						meshlab_points_faces(fp_output, g->filledGrid[i][j].x, k*g->gridsize ,g->filledGrid[i][j].z, g->gridsize );
					}
					//as we have used differet material to show door we reset it to original material being used for the temple. 
					if(f){
						fprintf(fp_output,"usemtl symmetryPlane\n");
					}
				}
			}			
		}
	}

	
	fclose(fp_output);
}

//! Creates voxelization of object by constructing one voxel per filled unit grid block in each slice with segmentaion details.
/*!
* \param	outFileName  	Filename of the output obj file to visualize voxelization.
* \param 	axis 			Name of the slicing plane 
*/ 

void createVoxelsByGrid_segments(string outFileName , string axis){

	FILE *fp_output = fopen(outFileName.c_str(),"w");
	fclose(fp_output);
	fp_output = fopen(outFileName.c_str(),"a");
	fprintf(fp_output,"mtllib temple.mtl\n");
	
	
	int material_number = 0;
	int k = 0;
	for (int i = 0; i < segments.size(); ++i){

		for (int j = 0; j < segments[i].slices.size(); ++j){
			fprintf(fp_output,"usemtl symmetryPlane%d\n",(material_number+1));
			Grid *g = segments[i].slices[j];
			for (int i = 0; i < g->xgridDim; ++i){
				for (int j = 0; j < g->zgridDim; ++j){


					if(g->filledGrid[i][j].isFilled){

						// Reconstruct the object in originally readed axis order
						if(axis.compare("zy") == 0){

							meshlab_points_faces(fp_output, k*g->gridsize, g->filledGrid[i][j].z, g->filledGrid[i][j].x,g->gridsize );
						}
						else if(axis.compare("yx") == 0){
							meshlab_points_faces(fp_output,g->filledGrid[i][j].z ,g->filledGrid[i][j].x, k*g->gridsize, g->gridsize );
						}
						else if(axis.compare("xz") == 0){
							meshlab_points_faces(fp_output, g->filledGrid[i][j].x, k*g->gridsize ,g->filledGrid[i][j].z, g->gridsize );
						}
						//meshlab_points_faces(fp_output, g->filledGrid[i][j].x, k*g->gridsize ,g->filledGrid[i][j].z, g->gridsize );
					}
				}
			}
			k++;
		}
		material_number++;
		material_number = material_number%5;
	}
	fclose(fp_output);
}


//! It generate the representation of any polygon in respective obj file along with some reeb graph details.
/*!
* \param outFileName 	Name of the obj file 
* \param p 				Pointer to the isothetic polygon which needs to be printed in obj file.
* \param g 				It represents grid size.
* \param factor 		It is used to compare two adjacent polygon in the reeb graph to assign the weight.
*/
void printIsoPolygonInObj( string outFileName, IsoPolygon *p, int g , int factor){


	FILE *fp_output = fopen(outFileName.c_str(),"a");
	//If the polygon is hole then use the hole_col material
	if(p->isHole){

		fprintf(fp_output,"\nusemtl hole_col\n");

	}
	// If polygon has more than 2 adjacent polygons then it is articulation point.
	else if(p->adjPolygons.size() > 2){


		fprintf(fp_output,"\nusemtl articulation_point\n");
	}

	//else it is the normal polygon
	else{
		fprintf(fp_output,"\nusemtl white_col\n");
	}
	for (int i = 0; i < p->size; ++i){
		fprintf(fp_output,"v %d %d %d\n",p->polyPoints[i].x ,p->sliceNum * g ,p->polyPoints[i].z);
	}

	fprintf(fp_output, "f ");
	for (int i = 0; i < p->size; ++i){
		s++;
		fprintf(fp_output, "%d ", s); 
	}
	
	fclose(fp_output);	
}

//! It generates the obj file which shows all slices along with details of the reeb graph
/*!
* \param outFilename 	Name of the obj file. 
* \param graph 			It is the object of the reeb graph whose details are to be printed to the obj file.
* \param g 				It represents grid size.
*/
void printReegGraphInObj(string outFileName, ReebGraph *graph, int g){

	FILE *fp_output = fopen(outFileName.c_str(),"w");
	fprintf(fp_output,"mtllib temple.mtl\n");
	fclose(fp_output);
	for (int i = 0; i < graph->vertices.size(); ++i){
		//If the degree of the vetex is greater than 2 then it is articulation point.

			//printf("polygon %d====\n", i );
			printIsoPolygonInObj(outFileName ,graph->vertices[i] , g , 1.08);
	
	}


}

// Generates slices along the given plane and then create output files for visualization.
/*!
* \param 	axis 	It specififies the plane along which we are slicing the object.
* \param	factor  It is the factor used to compare the depth and perimeter of outer polygon's to classify the slice into appropriate segment.
*/


void calculate_slices(string axis,double factor){
	cout<< time(NULL) << endl;
	segments.clear();
	first_dimesion = 0;
	second_dimesion = 0;
	
	int size = Readed_points.size();

	vector<Point> hPoints(size);
	double aa,bb,cc;
	int p =0;
	double minX = 10000,minZ = 10000;
	double maxX = -10000  ,maxZ = -10000 ;
	for(int i = 0 ; i< size; i++){
		//According to plane we are slicing ,  we exchange the the co-ordinates of original readed points to always have xz as slicng plane. 
		if(axis.compare("zy") == 0){

			bb = Readed_points[i].x;
			cc = Readed_points[i].y;
			aa = Readed_points[i].z;
		}
		else if(axis.compare("yx") == 0){
			//fp>>c>>a>>b;
			cc = Readed_points[i].x;
			aa = Readed_points[i].y;
			bb = Readed_points[i].z;
		}
		else if(axis.compare("xz") == 0){
			//fp>>a>>b>>c;
			aa = Readed_points[i].x;
			bb = Readed_points[i].y;
			cc = Readed_points[i].z;
		}

		// we scale the real object by using the scale factor.
		aa = aa * scaleFac;
		cc = cc * scaleFac;
		bb = bb * scaleFac;
		hPoints[i] = Point(aa,bb,cc);
		if (minX > aa)
			minX = aa;
		if (minZ > cc)
			minZ = cc;

		if (maxX < aa)
			maxX = aa;
		if (maxZ < cc)
			maxZ = cc;
		p++;

	}
	
	double margin = 20;
	for (int i = 0; i < size; ++i) {
	
		hPoints[i].x = hPoints[i].x + margin - minX;
		hPoints[i].z = hPoints[i].z + margin - minZ;
	}

	//Detemine the length of the canvas by finding the bounding box along the slicing plane for the entire object and adding extra margin(margin) from all side to this box;
	canvas_x_len = abs(maxX - minX);
	canvas_z_len = abs(maxZ - minZ);
	canvas_x_len = canvas_x_len + 2*margin;
	canvas_z_len = canvas_z_len + 2*margin;
 

	//sort the point along current point.(Note that every time we exchange the co-ordinates , 
	//so that whenever we are slicng real object its slicing plane become xz. Hence we always sort along y axis.)
	//For slicing we are actually using scaled object.
	sort(hPoints.begin(),hPoints.end(),pointCmpY());
	Point p1 = hPoints[0];
	double y = p1.y,slice;

	cout<< time(NULL) << endl;
	cout << "Enter slice size ( "<< axis<<"):"<<endl;
	cin >> slice;
	double y1 = y + slice;

	double f;
	cout << "Enter grid size factor:"<<endl;
	cin>> f;
	cout<< time(NULL) << endl;
	vector<vector<Point> > slicedPoints;
	vector<double> slicedYco;
	for(int i = 0 ; i < size; i++){
		vector<Point> v;
		Point p = hPoints[i];
		while( p.y < y1 ){

		
			v.push_back(p);				
			i++;
			if(i == size){
				i++;
				break;
			}
			p = hPoints[i];
		}
		i--;
		if(v.size() != 0){
			slicedPoints.push_back(v);
			slicedYco.push_back(y1);
		}
		else{
			//cout<<"this slice is empty: "<< y1<<endl;
		}
		y1 = y1+slice;
		v.clear();
		
	}
	
	vector<Grid*> grids;
	double globalIsotheticDist = 1;
	double globalMinX = minX - minX + margin;
	double globalMaxX = maxX - minX + margin;
	double globalMinZ = minZ - minZ + margin;
	double globalMaxZ = maxZ - minZ + margin;
	int totalGrids = 0;
    
    cout<<"find globalIsotheticDist: "<<endl <<time(NULL) << endl;
	size = slicedPoints.size();
	for(int i = 0 ; i < size; i++){
		Grid *g = new Grid(slicedPoints[i].size());

		copy(slicedPoints[i].begin(), slicedPoints[i].end(), g->v);
		//g->find_min_max();
		
		/*g->find_Min();

		if(i == 0)
			globalIsotheticDist = g->MinIsoDist;

		else{
			if(g->MinIsoDist < globalIsotheticDist ){
				globalIsotheticDist = g->MinIsoDist;
			}
		}*/
		totalGrids++;
		grids.push_back(g);


	}

	cout<<"globalIsotheticDist: " << globalIsotheticDist << endl;
	cout<<"Algorithm will start here:" <<time(NULL) << endl;
	
	Segment segment;
	
	for(int i = 0; i< totalGrids; i++){
		Grid *g = grids[i];
		g->canvas_x_len = canvas_x_len;
		g->canvas_z_len = canvas_z_len;
		g->maxZ = globalMaxZ;
		g->maxX = globalMaxX;
		g->minZ = globalMinZ;
		g->minX = globalMinX;
		g->sliceNum = sliceNum;
		g->MinIsoDist = globalIsotheticDist;

		g->findGridSize(f);
		//cout << "grid size " << g->gridsize << endl;
		g->slicing_coordinate = slicedYco[i];
		g->fillGrid(axis);
		g->printStart();
		g->printGrid();
		for (int j = 0; j < g->totalPolygons; ++j)
		{
			g->gridPolygons[j].reportPolyGon(g->filename);

			g->gridPolygons[j].findSymmetryAxis();

			if(! g->gridPolygons[j].isHole){
				g->outerPolygonsList.push_back( &g->gridPolygons[j] );
			}
		}
		//The method used to print all original points in the perticular slice in associated svg file
		//g->printPoints();
		//The parameter 2 is customizable. Read the defination of method in grid.cpp for more details.
		g->findSliceSymmetry(2);


		//for each of the outer polygon find out door points by using the algorithm for single polygon
		for (int j = 0; j < g->outerPolygonsList.size(); ++j){
			g->outerPolygonsList[j]->findDoorPoints(g->DoorPoints);
		}		
		//g->outerPolygon->findDoorPoints(g->DoorPoints);
		if(g->DoorPoints.size()){
			//cout << "Door Ponts found in " << sliceNum << endl; 
			g->printDoorPoints(g->filename);
		}
		g->printSliceSymmetry();
		g->outerPolygon->calculateDepth();
		//cout <<g->outerPolygon->x_depth << " " << g->outerPolygon->z_depth <<" " << g->slicing_coordinate <<"  " << g->outerPolygon->getPerimeter() <<endl;

		//perimeter based slicing
		int periNow = g->outerPolygon->getPerimeter();
		int periPrev = 0;
		if(segment.slices.size() > 0 && i > 0)
			periPrev = grids[i-1]->outerPolygon->getPerimeter();

		//By using only perimeter comparision for segmetation 
		/*if( segment.slices.size() > 0 &&(  periNow > 1.2 * periPrev || periPrev > 1.2 * periNow)){

				segments.push_back(segment);
				segment.slices.clear();
		}*/

		//By using only depth comparision for segmentation
		/*if( segment.slices.size() > 0 &&((  g->outerPolygon->x_depth > 1.2 * grids[i-1]->outerPolygon->x_depth || grids[i-1]->outerPolygon->x_depth > 1.2 * g->outerPolygon->x_depth) ||
					(  g->outerPolygon->z_depth > 1.2 * grids[i-1]->outerPolygon->z_depth || grids[i-1]->outerPolygon->z_depth > 1.2 * g->outerPolygon->z_depth)	)){

							segments.push_back(segment);
							segment.slices.clear();
		}*/


		//by using depth and perimeter comparision for segmentation.
		if( segment.slices.size() > 0 &&(  periNow > factor * periPrev || periPrev > factor * periNow) &&
				( segment.slices.size() > 0 &&((  g->outerPolygon->x_depth > factor * grids[i-1]->outerPolygon->x_depth || grids[i-1]->outerPolygon->x_depth > factor * g->outerPolygon->x_depth) ||
						(  g->outerPolygon->z_depth > factor * grids[i-1]->outerPolygon->z_depth || grids[i-1]->outerPolygon->z_depth > factor * g->outerPolygon->z_depth)	))){

			//printf("polygon diversion : %d %d :: %d %d :: %d %d\n",periNow, periPrev, g->outerPolygon->x_depth, grids[i-1]->outerPolygon->x_depth ,  g->outerPolygon->z_depth, grids[i-1]->outerPolygon->z_depth );
			segments.push_back(segment);
			segment.slices.clear();
		}


		segment.slices.push_back(g);
		//cout << g->Grid_x_sym << " " << g->Grid_z_sym << " "<< g->slicing_coordinate << endl;
		sliceNum++;
	}

	segments.push_back(segment);


	cout<< time(NULL) << endl << "writing outPut" << endl;
	string outFileName = axis + "_Segmentation.obj";

	/*if(axis.compare("xz") == 0){

		create_obje_file_Y(outFileName);
		outFileName = axis +"_symmetry.obj";
		s = 0;
		//Create the obje file which shows symmetry planes.
		create_obje_file_symmetry( outFileName);
		double sym_value_X = 0,sym_value_Z = 0, start_slice_value = slicedYco[0] ,end_slice_value = slicedYco[totalGrids-1];

		for (int i = 0; i < totalGrids; ++i)
		{
			Grid *g = grids[i];
			sym_value_X = sym_value_X + g->Grid_x_sym;
			sym_value_Z = sym_value_Z + g->Grid_z_sym;

			if( first_dimesion < g->first_dimesion )
				first_dimesion = g->first_dimesion;
			if(second_dimesion < g->second_dimesion )
				second_dimesion = g->second_dimesion;

		}

		sym_value_X = sym_value_X/totalGrids;
		sym_value_Z = sym_value_Z/totalGrids;

		fstream fw;
		fw.open(const_cast<char *> (outFileName.c_str()), ios::out | ios::app);
		fw<<"usemtl symmetryPlane"<<endl;
		fw<<"v "<<(sym_value_X + minX - margin)/scaleFac<<" "<< start_slice_value <<" " << (minZ - margin)/scaleFac <<endl;
		fw<<"v "<<(sym_value_X + minX - margin)/scaleFac<<" "<< start_slice_value <<" " << (second_dimesion + minZ -margin)/scaleFac <<endl;
		fw<<"v "<<(sym_value_X + minX - margin)/scaleFac<<" "<< end_slice_value 	 <<" " << (second_dimesion + minZ-margin)/scaleFac<<endl;
		fw<<"v "<<(sym_value_X + minX - margin)/scaleFac<<" "<< end_slice_value   <<" " << (minZ - margin)/scaleFac<<endl;

		fw<< "f "<< ++s<<" " << ++s<< " " << ++s<<" " << ++s <<endl;

		fw<<"usemtl symmetryPlane"<<endl;
		fw<<"v "<< (minX - margin)/scaleFac<<" "<< start_slice_value <<" " << (sym_value_Z + minZ - margin)/scaleFac <<endl;
		fw<<"v "<<(first_dimesion + minX -margin)/scaleFac<<" "<< start_slice_value <<" " << (sym_value_Z + minZ - margin)/scaleFac <<endl;
		fw<<"v "<< (first_dimesion + minX-margin)/scaleFac<<" "<< end_slice_value   <<" " << (sym_value_Z + minZ - margin)/scaleFac <<endl;
		fw<<"v "<<(minX - margin)/scaleFac<<" "<< end_slice_value  <<" " << (sym_value_Z + minZ - margin) /scaleFac<<endl;

		fw<< "f "<< ++s<<" " << ++s<< " " << ++s<<" " << ++s <<endl;
	}

	else if(axis.compare("yx") == 0){
		create_obje_file_Z(outFileName);

	}
	else{
		create_obje_file_X(outFileName);
	}*/



	
	//Method call to create voxelization of the object to show segmentation. 
	createVoxelsByGrid_segments(outFileName , axis);
	cout<< time(NULL) << endl;

	ReebGraph *r =  buildReebGraph(grids, grids[0]->gridsize);
	//reset the vertex number to 0 so that we can restart from 1 to print reeb graph details.
	s = 0;
	printReegGraphInObj("ReebGraph_"+ axis + ".obj" , r , grids[0]->gridsize);
	//Print end of the file for all svg files.
	
	
	findMultipleOpeningsByReebGraph(grids,r);

	for(int i = 0; i< totalGrids; i++)
		grids[i]->printEnd();
	string outFileName_door = axis + "_Grid_oriented_temple_door.obj";
	//if(axis.compare("xz") == 0 ){
	//method call to create voxelization of the object to show the door which is only detected along xz plane
	s = 0;
	createVoxelsByGrid_door(outFileName_door,grids , axis);
	//s=0;
	//}
}



//! It determines the staircases in the temple.

void findStairCases(vector<Grid*>  grids, int halfZ, int halfX){

	for (int i = 0; i < grids.size(); ++i){
		
		for (int j = 0; j < grids[i]->outerPolygon->size; ++j){
			
			if( grids[i]->outerPolygon->polyPoints[j].z < halfZ && grids[i]->outerPolygon->polyPoints[j].x > halfX ){



			}
		}

	}
}

//! It uses reeb graph construction to explore the mutiple opening like multiple doors etc. 
/*!
* \param grid 		array of pointers of the grids cooresponding to slices along any perticular plane.
* \param gridsize 	pointer to the reeb graph.
*/

void findMultipleOpeningsByReebGraph(vector<Grid*>  grids,ReebGraph *r){


	for (int i = 0; i < r->articulationPoints.size()-1; ++i){		
		bool found = false;
		if(r->articulationPoints[i]->sliceNum + 4 < r->articulationPoints[i+1]->sliceNum){

			for (int sl = r->articulationPoints[i]->sliceNum +1; sl <  r->articulationPoints[i+1]->sliceNum -1; ++sl){
				if(grids[sl]->outerPolygonsList.size() != grids[sl]->outerPolygonsList.size() && grids[sl]->outerPolygonsList.size() > 1 ){
					found = true;
					break;
				}
			}
			//If found flag is not set, It implies we have pair of articulation points in between which there are mutiple openings.
			if(!found){
				cout << "slices between " << r->articulationPoints[i]->sliceNum + 1 << "  to " << r->articulationPoints[i+1]->sliceNum << endl;
				for (int sl = r->articulationPoints[i]->sliceNum +1; sl <  r->articulationPoints[i+1]->sliceNum  ; ++sl){
					
					/// find the relation between the polygons.
					
					for (int i = 0 ; i < grids[sl]->outerPolygonsList.size() -1; ++i){

						for (int j = i+1; j < grids[sl]->outerPolygonsList.size(); ++j){

							grids[sl]->outerPolygonsList[i]->determineOpeningPoints(*grids[sl]->outerPolygonsList[j],grids[sl]->DoorPoints  );
							
						}
						
					}


					grids[sl]->printDoorPoints(grids[sl]->filename);

				}
			}
		}
	}

}

//! It reads the input points from the input file provided.
/*!
* \param filename 	It is the name of the file from which input points are being read
* \param reads 		it depicts the order in which we read the point( order of the axes).
*/

void readPoints(string filename, string read){

	fstream fp;
	//cout << filename << endl;
	fp.open(filename.c_str(),ios_base::in);
	double a, b,c;
	cout<< "Reading point" << endl;
	if(read == "zyx")
	while(!fp.eof()){
		fp >> c >> b >> a;
		//cout << a << " " << " " << " " << c;
		Readed_points.push_back(Point(a,b,c));
		cout << "reaad " << endl;
	}

	else if(read == "xyz")
	while(!fp.eof()){
		fp >> a >> b >> c;
		//cout << a << " " << " " << " " << c;
		Readed_points.push_back(Point(a,b,c));
	}
	//cout << "Total points : " << Readed_points.size() << endl;
	fp.close();

}

//! Builds Reeb Graph (undirected and unweighted) from the slicing information available. 

/*!
* \param slicePlane 	Plane along which slicing is being done.
* \param gridsize 	It is the size of the grid that is to be used.
* \return The pointer to the reebGraph object which contains entire information of the object.
*/

 ReebGraph * buildReebGraph( vector<Grid*>  grids , int gridsize){

 	ReebGraph *graph = new ReebGraph();

 	for(int i = 0; i < grids.size(); ++i){
 		for (int j = 0; j < grids[i]->totalPolygons; ++j)
 			graph->vertices.push_back(&grids[i]->gridPolygons[j]);
 	}

 	for (int i = 1; i < grids.size(); ++i){

 		for (int j = 0; j < grids[i]->totalPolygons; ++j){ 			

 			for (int k = 0; k < grids[i-1]->totalPolygons; ++k){
 				// if both polygons we are checking are not hole polygons then only check for intersection
				if( !(grids[i]->gridPolygons[j].isHole || grids[i-1]->gridPolygons[k].isHole) && grids[i]->gridPolygons[j].checkIntersection(&grids[i-1]->gridPolygons[k], gridsize ) ){

					//cout << "intersection between polygons " << j << " of slice "<< i<<" and " << k << " of slice "<<i-1 <<  endl; 
					grids[i]->gridPolygons[j].adjPolygons.push_back(&grids[i-1]->gridPolygons[k]);
	 				grids[i-1]->gridPolygons[k].adjPolygons.push_back(&grids[i]->gridPolygons[j]);

				}

 			}

 		}
  	}

  	for (int i = 0; i < graph->vertices.size(); ++i)
  	{
  		if(graph->vertices[i]->adjPolygons.size() > 2){

  			graph->articulationPoints.push_back(graph->vertices[i]);
  		}
  	}


  	return graph;
 }


int main(int argc, char const *argv[])
{
	
	cout <<"Please read readme.txt once before you run it further."<< endl;
	scaleFac = 5;
	char choice;
	cout << "Do you want to change the scale factor (Default is 5 ):(y/n)" << endl;
	cin>> choice;
	if(choice == 'y' || choice == 'Y'){
		cout << "Enter the scaling factor:" << endl;
		cin>> scaleFac;
	}
	string filename = argv[1];
	
	string read = "xyz" ;

	if(argc > 2) read = argv[2];
	
	filename = "input/" + filename;
	readPoints(filename , read);
	sliceNum = 0;
	s = 0;
	cout << " starting Algorithm with xz" << endl;
	calculate_slices("xz",1.2);
	cout << " Done Algorithm with xz" << endl;
	sliceNum = 0;
	s = 0;
	cout << " starting Algorithm with yx" << endl;
	
	calculate_slices("yx",1.03);
	cout << " Done Algorithm with yx" << endl;
	sliceNum = 0;
	s = 0;
	calculate_slices("zy",1.08);
	return 0;
}