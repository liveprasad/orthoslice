/*
 * GridPoint.h
 *
 *  Created on: 12-Jun-2015
 *      Author: liveprasad
 */

#ifndef ENTITIES_GRIDPOINT_H_
#define ENTITIES_GRIDPOINT_H_

#include "../CommonInclude.h"

namespace std {

class GridPoint {
public:

		int x;
		int z;
		int type;
		
		GridPoint();
		GridPoint(int x, int z);
		bool equals(GridPoint g);
		int getIsotheticDist(GridPoint g);


};

} /* namespace std */


#endif /* ENTITIES_GRIDPOINT_H_ */
