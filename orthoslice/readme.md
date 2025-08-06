compile program : g++ a4.cpp -std=c++11 
run : a.out in3 (in3 is input file name)

OUTPUT DIRECTIONS:
1) Please create 3 empty folder in directory where a4.cpp file is persent.
2) Give name to these folders as 'xy' , 'yx' , 'zy'
3) In these folder program will put all the slices output along respective projection plane.
   e.g 'xy' will contain all the slices taken along the z axis.
5) please make these folder empty each time you run the program to avoid the mess up of ouput between two 
   consecutive runs.
4) file name format is projectionPlane_sliceNumber_gridcellsize_yvalue.

INPUT DIRECTIONS:

1) First input parameter is scale factor and it is just to scale the object large enough to see 
   the output in respective svg file clearly.
2) It will ask for the slice size along any perticular axis(with respect to projection plane.)
3) It will ask for the grid size factor.
  
It will ask for them in the order xz(along y ) , yx(along z) , zy(along x)

INPUT FILE DIRECTIONS:

1) Please check the input file in3 in the folder.
2) It contains points listed line by line.
3) Each line has 3 co-ordinates x,y,z separated by space between them.
4) there is no space after last point listed inthe file.

