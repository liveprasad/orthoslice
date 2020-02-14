//! Grid Class
/*!
 *  Created on 	: 26-Jun-2015
 *  Author 		: prasad shetkar
 */

#include "Grid.h"

namespace std {
Grid::Grid() {
	// TODO Auto-generated constructor stub
	outerPolygon 	= new IsoPolygon();
	gridsize 		= 0;
	totalPolygons 	= 0;
	

}

Grid::Grid(int totalPoints) {
	// TODO Auto-generated constructor stub
	this->totalPoints 	= totalPoints;
	v 	 				= new Point[totalPoints];
	outerPolygon 		= new IsoPolygon();
	gridsize 			= 0;
	totalPolygons 		= 0;
	

}

Grid::~Grid(){
	DeleteGrid();
	delete[] v;	
	free(this);
}

void Grid::find_min_max(){

	minX = maxX = v[0].x;
	minZ = maxZ = v[0].z;
	for (int i = 1; i < totalPoints; ++i)
	{
		change_Min_Max(v[i]);
	}
}

void Grid::find_Min(){

	if(totalPoints > 1){
		find_Min(&v[0]);
		MinIsoDist = getIsotheticDist(pair1,pair2);
	}
	else{

		minX = maxX = v[0].x;
		minZ = maxZ = v[0].z;
		MinIsoDist = 100;

	}
}
void Grid::change_Min_Max(Point P){

	if(P.x 		< minX)
		minX 	= P.x;
	else if(P.x > maxX)
		maxX 	= P.x;
	if(P.z 	 	< minZ)
		minZ 	= P.z;
	else if(P.z > maxZ)
		maxZ 	= P.z;
}
void Grid::find_Min(Point *points) {


	random_shuffle ( &points[0], &points[totalPoints-1] );
	minX = maxX 		= points[0].x;
	minZ = maxZ 		= points[0].z;
	gridmin 			= getEcludianDist( points[1],points[0]);
	change_Min_Max(points[1]);
	pair1 				= points[1];
	pair2 				= points[0];
	double hashLength 	= gridmin/2;

	map<string,Point> gridMap;
	map<string,Point>:: iterator mapIt;

	rehash(points,gridMap,hashLength,2);
	for(int  var = 2;  var < totalPoints; ++ var) {
		Point P 		= points[var];
		change_Min_Max(P);
		int index_x 	= int (P.x/hashLength);
		int index_z 	= int (P.z/hashLength);
		string index 	= getIndex (points[var],hashLength);
		mapIt 			= gridMap.find(index);
		double min 		= gridmin ;
		if(mapIt == gridMap.end()){
			gridMap.insert(pair<string,Point>(index,P));
			getMinof_25_Dist(index_x,index_z,P,gridMap,min,false);
		}
		else{
			getMinof_25_Dist(index_x,index_z,P,gridMap,min,true);
		}

		if(min < gridmin)
		{
			gridmin 	= min;
			hashLength 	= gridmin/2;
			rehash(points,gridMap,hashLength,var);
		}

	}

}

void Grid::getMinof_25_Dist(int i,int j,Point P,map<string,Point> &gridMap,double &min,bool iscal){

	int m = i-2;
	int k = j-2;
	for (; m < i+3; ++m) {
		for (k = j-2; k < j+3; ++k) {
			if(m==i && j==k && !iscal){
				continue;
				
			}
			ostringstream convert1 , convert2;   // stream used for the conversion

			convert1 << m;
			convert2 << k;
			string index = convert1.str() + "-" + convert2.str();
			changeMin(index,min,gridMap,P);
		}

	}
}

double Grid::getEcludianDist(Point P1,Point P2){
	double xsqaure  = abs(P1.x -P2.x);
	double zsqaure = abs(P1.z-P2.z);
	xsqaure *= xsqaure;
	zsqaure *= zsqaure;

	return sqrt(xsqaure + zsqaure );
}

double Grid::getIsotheticDist(Point P1,Point P2){

	double val = abs( P1.x-P2.x);
	double val1 = abs( P1.z-P2.z);
	if(val > val1)
		return val;
	else
		return val1;
}

void Grid::changeMin(string index, double &min, map<string,Point> &gridMap,Point P){
	Point P1;
	double val;
	try{
		P1 = gridMap.at(index);
		val =getEcludianDist(P1 ,P);
		if(val < min){
			min = val;
			pair1 = P;
			pair2 = P1;

		}
	}
	catch (out_of_range& e) {

	}
}

void Grid::rehash(Point *points,map<string,Point> &gridMap,double hashLength,int size){
	gridMap.clear();
	for (int i = 0; i < size; ++i) {
		string index = getIndex(points[i],hashLength);
		gridMap.insert(pair<string,Point>(index,points[i]));
	}
}
string Grid::getIndex(Point P,double hashLength){
	int index_x = int (P.x/hashLength);
	int index_z = int (P.z/hashLength);
	ostringstream convert1,convert2;
	convert1 << index_x;
	convert2 << index_z;
	string index = convert1.str()+ "-" + convert2.str();
	return index;
}
//! FInds gridSize for current grid based on isothetic distance and factor.
/*!
 \param f grid size factor taken from the user.
*/
void Grid::findGridSize(double f){
	//cout<< "min Iso distsance."<< MinIsoDist<< endl;
	gridsize = (int)( f * (double)MinIsoDist);
	//cout<< "grid size."<<gridsize << endl;
	if(gridsize < 1)
		gridsize = 1;

}

//!Delete entire 2 dimesional array of GridCells allocated dynamically for the perticular slice associated with this grid object
void Grid::DeleteGrid(){

	for (int i = 0; i < xgridDim; ++i)
	{
		delete[] filledGrid[i];
	}

	delete[] filledGrid;
}
//!Allocate sufficient memory for 2 dimesion grid of Gridcells.
void Grid::setGridDimesions(){

	
	filledGrid = new GridCell*[xgridDim];

	for (int i = 0; i < xgridDim; ++i)
	{
		filledGrid[i] = new GridCell[zgridDim];
	}
}

//! By using max and min value along the axes,it defines the dimesions of the 2D grid and fills the grid accordingly.

/*!
 \param  slicePlane Plane along which slicing is being done.
*/

void Grid::fillGrid(string slicePlane){


	gridStartX 	= (int)minX;
	gridStartZ 	= (int) minZ;
	gridEndX 	= ceil( maxX);
	gridEndZ 	= ceil(maxZ);
	// find out the grid dimesions by using gridsize and the additive 5 is to make the adjustment.
	xgridDim 	= abs(ceil((gridEndX - gridStartX)/gridsize)) + 5;
	zgridDim 	= abs(ceil((gridEndZ - gridStartZ)/gridsize)) + 5;
	 
	setGridDimesions();
	//set the x and z cordinates of the each gridCell(refer GridCell.cpp) of the filledGrid.
	for(int i = 0; i < xgridDim; ++i) {

		for(int j = 0; j< zgridDim; j++){
			if( abs(minX -gridStartX)<= EPSILON )
				filledGrid[i][j].x = (i-2) * gridsize + gridStartX;  
			else
				filledGrid[i][j].x = (i-1) * gridsize + gridStartX;
			if(abs(minZ -gridStartZ)<= EPSILON)
				filledGrid[i][j].z = (j-2) * gridsize + gridStartZ;
			else
				filledGrid[i][j].z = (j-1) * gridsize + gridStartZ;
			filledGrid[i][j].isFilled = false;
			filledGrid[i][j].isvisited = false;

		}
	}
	// fill the grid by setting the GridCell attributes isFilled as true if we have points in that GridCell , otherwise false. 
	for  (int  var = 0; var < totalPoints; ++var) {

		double index_X;
		double index_Z;

		double  fx = modf ((v[var].x - gridStartX)/gridsize + 1, &index_X);
		double  fz = modf ((v[var].z - gridStartZ)/gridsize + 1, &index_Z);
		if( abs(minX -gridStartX)<= EPSILON )
			index_X++;
		if(abs(minZ -gridStartZ)<= EPSILON)
			index_Z++;
		filledGrid[int(index_X)][int(index_Z)].isFilled = true;

		if(abs(fx-0.0)<= EPSILON){
			filledGrid[int(index_X)-1][int(index_Z)].isFilled = true;
			if(abs(fz-0.0)<= EPSILON){
				filledGrid[int(index_X)-1][int(index_Z)-1].isFilled = true;
				filledGrid[int(index_X)][int(index_Z)-1].isFilled = true;
			}
		}
		else if(abs(fz-0.0)<= EPSILON){
			filledGrid[int(index_X)][int(index_Z)-1].isFilled = true;
		}
	}

	/*string filename = to_string( slicing_coordinate )+ "-"+"Grid" + ".svg";

					printStart(filename);
					printGrid(filename,filledGrid,xgridDim,zgridDim);
				   	//fillgridColor(filename,filledGrid,xgridDim,zgridDim);
				    printPoints(filename);
				    fstream fw;
				    fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app);

				   	fw<<"</svg>" <<endl;
				    fw.close();*/
	
	first_dimesion 	= (xgridDim -1) * gridsize;
	second_dimesion = (zgridDim -1) * gridsize;

	//call traversing algorithm to travese the entire grid.(starting index is zero hence we use one less than actual dimesions)
	this->TraversingAlgorithm(slicePlane,xgridDim-1,zgridDim-1);
}
//! Draws the grid in the output svg file for the current grid object in gray color.
void Grid::printGrid(){

	fstream fw;
	//Open the file for writing
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app);
	for (int var = 0; var < xgridDim; ++var) {
		fw << "<line x1=\""<<filledGrid[var][0].x<<"\" y1=\""<<filledGrid[var][0].z<<"\" x2=\""<<filledGrid[var][zgridDim-1].x<<"\" y2=\""<<filledGrid[var][zgridDim-1].z<<"\" stroke=\"gray\" stroke-width=\"0.3\"/>"<<endl;
	}

	for (int var = 0; var < zgridDim; ++var) {
		fw << "<line x1=\""<<filledGrid[0][var].x<<"\" y1=\""<<filledGrid[0][var].z<<"\" x2=\""<<filledGrid[xgridDim-1][var].x<<"\" y2=\""<<filledGrid[xgridDim-1][var].z<<"\" stroke=\"gray\" stroke-width=\"0.3\"/>"<<endl;
	}
	fw.close();
}
//! Dynamically allocates the memory to store polygon points if the current allocated memory is not sufficient to store new generated point and store new point. 
/*!
 \param  polyPointsCount 				Current count of the polygon points.
 \param  g 								New gridpoints to be stored while traversing the grid.
 \param  allocatedSize_polyPoints		Number of points being stored in the current allocated memory space.
 \return The size of newly allocated 	Memory in terms of number of polypoints.
 */
int Grid::Push(int polyPointsCount,GridPoint &g , int allocatedSize_polyPoints){
    
    if(allocatedSize_polyPoints < polyPointsCount) {
    	allocatedSize_polyPoints 	= allocatedSize_polyPoints * 2;
		GridPoint *newarray 		= new GridPoint[allocatedSize_polyPoints];

		memcpy(newarray , polyPoints , (polyPointsCount -1) * sizeof(GridPoint));
		delete[] polyPoints;
		polyPoints = newarray;
		polyPoints[polyPointsCount-1] = g ;
	}
	else
		polyPoints[polyPointsCount-1] = g ;
	return allocatedSize_polyPoints;

}

//! Traverses the grid by using the traversing algorithm and all the isothetic polygons being detected get stored in the list \param gridPolygons of current grid object
/*!
 \param  slicePlane	Plane along which slicing is being done(We always assume any plane as xz plane for simplicity).	
 \param  gridMaxX 	Max X cordinate of the grid.
 \param  gridMaxZ	Max z cordinate of the grid.
 */

void Grid::TraversingAlgorithm(string slicePlane,int gridMaxX,int gridMaxZ ){

	char file[100];
	getcwd(file, 100);
	//simple conversion to create appropriate output file name
	filename = "/";
	ostringstream c1,c2,c3;
	c1 << sliceNum;
	c2 << this->gridsize;
	c3 << slicing_coordinate;
	filename = file + filename +slicePlane+"/"+slicePlane+ "_"+c1.str()+"_"+ c2.str() +"_" + c3.str() + ".svg";

	//printStart(filename);
	//printGrid(filename);
	//fillgridColor(filename,filledGrid,gridMaxX+1,gridMaxZ+1);
	int  i 							= 1;
	int  j 							= 1;
	int  int_grid 					= 0;
	bool isHoleFound 				= false;
	int  allocatedSize_polygons 	= 1;
	int  allocatedSize_polyPoints 	= 100;
	polyPoints 						= new GridPoint[allocatedSize_polyPoints];
	gridPolygons 			   		= new IsoPolygon[allocatedSize_polygons];

	while(!filledGrid[i][j].isFilled){
		i++;
		if(i == xgridDim){
			i = 1;
			j++;
		}
	}

	do{
		GridPoint Pstart 	= GridPoint(filledGrid[i][j].x,filledGrid[i][j].z);
		GridPoint P 		= Pstart,Prev,prev_prev_point;
		int pointcount 		= 1;
		int polyPointsCount = 1;		
		polyPoints[0] 		= Pstart; 

		if(isHoleFound)
			i--;
		else
			i++;
		do{
			prev_prev_point = Prev;
			Prev 			= P;
			P 				= GridPoint(filledGrid[i][j].x,filledGrid[i][j].z);
			////////////////////////////////////
			//Exclude the points of the type 1

			pointcount++;
			if( pointcount > 2 && ( (prev_prev_point.x - P.x) != 0 && (prev_prev_point.z - P.z) != 0 ) ){
				//direction is being changed so restart from this point
				polyPointsCount++;
				allocatedSize_polyPoints 	= Push(polyPointsCount,Prev, allocatedSize_polyPoints);
				pointcount 					= 2;

			}

			///////////////////////////////////////

			if(filledGrid[i-1][j].isFilled  && filledGrid[i][j-1].isFilled ){

				if(Prev.equals(GridPoint(filledGrid[i-1][j].x,filledGrid[i-1][j].z))){

					filledGrid[i-1][j-1].isvisited = true;
					j--;
				}
				else{
					filledGrid[i][j].isvisited = true;
					j++;
				}

			}
			else if(filledGrid[i][j].isFilled && filledGrid[i-1][j-1].isFilled){
				if(Prev.equals(GridPoint(filledGrid[i][j-1].x,filledGrid[i][j-1].z))){
					filledGrid[i][j-1].isvisited = true;
					i++;
				}
				else{
					filledGrid[i-1][j].isvisited = true;
					i--;
				}
			}
			else if(filledGrid[i][j-1].isFilled){

				filledGrid[i-1][j].isvisited = true;
				if(filledGrid[i-1][j-1].isFilled ){

					i--;
				}
				else{
					filledGrid[i-1][j-1].isvisited = true;
					j--;
				}
			}
			else if(filledGrid[i-1][j].isFilled){
				filledGrid[i][j-1].isvisited = true;
				if(filledGrid[i][j].isFilled){

					i++;
				}
				else{
					filledGrid[i][j].isvisited = true;
					j++;
				}
			}
			else if(filledGrid[i-1][j-1].isFilled){
				filledGrid[i][j].isvisited = true;
				if(filledGrid[i-1][j].isFilled){

					j++;
				}
				else{
					filledGrid[i-1][j].isvisited = true;
					i--;
				}
			}
			else if(filledGrid[i][j].isFilled){
				filledGrid[i-1][j-1].isvisited = true;
				if(filledGrid[i][j-1].isFilled){

					j--;
				}
				else{
					filledGrid[i][j-1].isvisited = true;
					i++;
				}
			}

		}while(!Pstart.equals(P));

		/*Create Isothetic Polygon instance with the corresponding polyPoints.*/
		IsoPolygon s= IsoPolygon(polyPointsCount, isHoleFound, sliceNum, totalPolygons);
		s.setPolygon(polyPoints);
		//fucntion in Isothetic polygon cpp to determine thr type of all points of the polygon as 1(inside angle 90) or 3 (inside angle 270)
		s.setAllPointsType();
		//function call to find the perimeter of currently detected isothetic polygon.
		s.getPerimeter();

		
		totalPolygons++;
		
		//dynamically allocate the memory space to list of isothetic polygon if currently allocated memory space is not sufficient and delete original space.
		if(totalPolygons > allocatedSize_polygons){
			allocatedSize_polygons 	= 2 * allocatedSize_polygons;
			IsoPolygon *newarray 	= new IsoPolygon[allocatedSize_polygons];
			memcpy(newarray, gridPolygons,(totalPolygons-1) * sizeof(IsoPolygon)); // before deleting original space copy entire earlier list to newly allocated space
			delete[] gridPolygons;
			gridPolygons = newarray;
		}

		//store the current polygon in the list of polygons at end.
		gridPolygons[totalPolygons-1] 	= s; 
		


		//string file = to_string( slicing_coordinate )+ to_string(int_grid)+ "-"+"iGrid" + ".svg";
		//int_grid++;
		//string file = slicePlane+ "_"+ c1.str()+ "_" + c2.str() +"_" + c3.str() + ".svg";
		//fillgridIntermediate(file,filledGrid,gridMaxX+1,gridMaxZ+1);
		isHoleFound = false;
		bool isSkip = true;
		while((filledGrid[i][j].isFilled && filledGrid[i-1][j].isvisited )|| isSkip
				|| (!filledGrid[i][j].isFilled && i< gridMaxX)){

			if(filledGrid[i][j].isFilled && filledGrid[i-1][j].isvisited  && !isSkip){
				isSkip = true;
			}
			if( filledGrid[i-1][j].isFilled && filledGrid[i][j].isvisited && isSkip )
				isSkip = false;
			if(filledGrid[i-1][j].isFilled && !filledGrid[i][j].isFilled && !filledGrid[i][j].isvisited && isSkip){
				if(filledGrid[i][j-1].isFilled){
					isHoleFound = true;
					i++;
				}
				else
					i--;
				break;
			}

			i++;

			if( i == gridMaxX){
				j++;
				if(j == gridMaxZ )
					break;
				i = 1;
			}
		}
	}while(i < gridMaxX && j < gridMaxZ );

	//printPoints(filename);
	fstream fw;
}

//!Prints the symmetry of the slice associated with this Grid object.
 
void Grid::printSliceSymmetry(){

	fstream fw;
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app);

	fw << "<line x1=\""<<gridStartX<<"\" y1=\""<< Grid_z_sym <<"\" x2=\""<<gridEndX<<"\" y2=\""<< Grid_z_sym<<"\" stroke=\"red\" stroke-width=\"1.0\"/>"<<endl;
	fw << "<line x1=\""<<Grid_x_sym<<"\" y1=\""<< gridStartZ<<"\" x2=\""<< Grid_x_sym<<"\" y2=\""<< gridEndZ<<"\" stroke=\"red\" stroke-width=\"1.0\"/>"<<endl;

	//fw<<"</svg>" <<endl;
	fw.close();
}

//Print ending of the corresponding svg file.
void Grid::printEnd(){
	fstream fw;
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app);
	fw<<"</svg>" <<endl;
	fw.close();
}

//! Compare 2 isothetic polygon with repect to their perimeter.
/*!
 \param s1 first instance of isothetic polygon.
 \param s2 second instance of isothetic polygon.
 \return true if s1 has greater perimeter than s2 , false otherwise.
*/

bool Grid::compareForPerimeter(IsoPolygon s1, IsoPolygon s2){

	return	s1.getPerimeter() > s2.getPerimeter()? true : false;

}

//!Finds the symmetry in the one slice by sorting the all polygons with respect to their perimeter in descending order and taking weighted mean(by using perimeter as weight) of the symmetry values of each polygon
/*!
 \param comp_par while considering the polygons in sorted sequence if the ratio of previous polygon's perimeter to current polygon's perimeter is less than comp_par then we discard all polygons from current polygons in that sequence.
 */
void Grid::findSliceSymmetry(int comp_par){
	
	Grid_x_sym = 0;
	Grid_z_sym = 0;
	int polyNum = 1,i;
	IsoPolygon *dummyGridPolygons = new IsoPolygon[totalPolygons];
	memcpy( dummyGridPolygons , this->gridPolygons , totalPolygons * sizeof(IsoPolygon));
	sort(gridPolygons,gridPolygons+totalPolygons,Grid::compareForPerimeter);
	outerPolygon = &gridPolygons[0];	
	double totalPerimeter = gridPolygons[0].getPerimeter() ;
	for(i = 0; i < totalPolygons-1; ++i) {
		IsoPolygon ss = gridPolygons[i+1];
		if(gridPolygons[i].getPerimeter() <= (comp_par * ss.getPerimeter())){

			totalPerimeter = totalPerimeter + ss.getPerimeter();
			polyNum = i+2;
			continue;
		}
		else{
			polyNum = i+1;
			break;
		}
	}
	for (i = 0; i < polyNum; ++i){
		IsoPolygon ss = gridPolygons[i];
		double peri = ss.getPerimeter();
		double weight = peri/totalPerimeter ;
		Grid_x_sym = Grid_x_sym + weight * ss.sym_x;
		Grid_z_sym = Grid_z_sym + weight * ss.sym_z;
	}
	// restore the original order of the polygons.
	memcpy(this->gridPolygons ,dummyGridPolygons , totalPolygons * sizeof(IsoPolygon));


	
	//outerPolygon is the outermost polygon or largest polygon in case of more than 1 outer polygon. 
	for (int i = 0; i < totalPolygons; ++i)
	{
		if( outerPolygon->x_min >= gridPolygons[i].x_min  && 
			outerPolygon->x_max <= gridPolygons[i].x_max  && 
			outerPolygon->z_min >= gridPolygons[i].z_min  && 
			outerPolygon->z_max <= gridPolygons[i].z_max   
			){

				outerPolygon 	= &gridPolygons[i];
		}		
	}

	//free(gridPolygons);

}
/*
void Grid::fillgridColor(string filename,int xgridDim,int zgridDim ){
	fstream fw;
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app);
	for(int i = 0; i < xgridDim -1; ++i) {
		for(int j = 0; j< zgridDim-1; j++){
			if(filledGrid[i][j].isFilled){
				fw<< "<polygon points=\"";
				fw<<filledGrid[i][j].x<<","<<filledGrid[i][j].z<<" ";
				fw<<filledGrid[i][j+1].x<<","<<filledGrid[i][j+1].z<<" ";
				fw<<filledGrid[i+1][j+1].x<<","<<filledGrid[i+1][j+1].z<<" ";
				fw<<filledGrid[i+1][j].x<<","<<filledGrid[i+1][j].z<<" ";
				fw<<"\" style=\"fill:white;\" />" << endl;
			}
		}
	}
	fw.close();
}


void Grid::fillgridIntermediate(string filename,int xgridDim,int zgridDim ){
	fstream fw;
	printStart();
	printGrid();
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app);
	for(int i = 0; i < xgridDim -1; ++i) {
		for(int j = 0; j< zgridDim-1; j++){
			if(filledGrid[i][j].isvisited ){
				fw<< "<polygon points=\"";
				fw<<filledGrid[i][j].x<<","<<filledGrid[i][j].z<<" ";
				fw<<filledGrid[i][j+1].x<<","<<filledGrid[i][j+1].z<<" ";
				fw<<filledGrid[i+1][j+1].x<<","<<filledGrid[i+1][j+1].z<<" ";
				fw<<filledGrid[i+1][j].x<<","<<filledGrid[i+1][j].z<<" ";

				fw<<"\" style=\"fill:blue;\" />" << endl;

			}
		}
	}
	fw<<"</svg>";
	fw.close();

}*/
//! print the real points of the object in this perticular slice associated with this grid in corresponding svg file.
void Grid::printPoints(){
	fstream fw;
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app);
	for (int i = 0; i <totalPoints ; ++i){

		fw<<"<circle cx=\""<< v[i].x <<"\" cy=\"" << v[i].z <<"\" r=\"0.2\" stroke=\"blue\" stroke-width=\"0.2\"/>"<<endl;
		//fw  <<"<text x=\""<<dcel.vertexList[i].x<<"\" y=\""<<dcel.vertexList[i].y<<"\" text-anchor=\"middle\" font-size=\"20px\" fill=\"#aaaaaa\">"<<dcel.vertexList[i].ownId<<"</text>"<<endl;
	}
	fw.close();
}
//! Prints starting format of the corresponding svg file.
void Grid::printStart(){
	fstream fw;
	fw.open(const_cast<char *> (filename.c_str()), ios::out );
	//cout << "Creating the file" << endl;

	string col = "red";
	fw << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" <<endl
			<< "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"<<endl
			<< "<svg width=\""<< canvas_x_len<<"px\" height=\""<< canvas_z_len<<"px\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"<<endl;

	fw.close();
}



//! It prints the door point in the corresponding svg file.
/*!
* 	\param 	filename 	The name of the output file.
* 	\param  k 			It depicts the number of door points.
*/

void Grid::printDoorPoints(string filename){

	fstream fw;
	fw.open(const_cast<char *> (filename.c_str()), ios::out | ios::app );
	
	for (int i = 0; i < DoorPoints.size(); i++){
		fw<<"<circle cx=\"" <<  DoorPoints[i] .x <<"\" cy=\"" << DoorPoints[i].z <<"\" r=\"1.5\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />"  << endl;		
	}

	fw.close();
	
}

} // end namespace std