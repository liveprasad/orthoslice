OBJS    = Point.o Segment.o IsoPolygon.o GridPoint.o Grid.o ReebGraph.o main.o

all: $(OBJS)
	g++ $(OBJS) -g -o orthoslice

%.o: Entities/%.cpp Entities/%.h
	g++ -g -I. -c $< -o $@

%.o: %.cpp 
	g++ -g -I. -c $< -o $@

clean:
	rm -f *.o *.obj orthoslice 

cleanout:
	rm -f *.obj xz/*.svg zy/*.svg yx/*.svg