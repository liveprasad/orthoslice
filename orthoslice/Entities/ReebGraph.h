/*
 * ReebGraph.h
 *
 *  Created on: 26-Jun-2015
 *      Author: liveprasad
 */

#ifndef ENTITIES_REEBGRAPH_H_
#define ENTITIES_REEBGRAPH_H_
#include "Constants.h"
#include "../CommonInclude.h"
#include "IsoPolygon.h"

using namespace std;

namespace std{

class ReebGraph {
public:

	vector<IsoPolygon *>  vertices;
	vector<IsoPolygon *>  articulationPoints;	

	ReebGraph();
	~ReebGraph();

	

};

} /* namespace std */

#endif /* ENTITIES_REEBGRAPH_H_ */