/*
 * CommonInclude.h
 *
 *  Created on: 12-Jun-2015
 *      Author: liveprasad
 */

#ifndef COMMONINCLUDE_H_
#define COMMONINCLUDE_H_


#include <time.h>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>


#include <cmath>
#include <string>
#include <algorithm>
#include <stack>
#include <map>
#include <utility>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <limits.h>





#ifdef __CUDACC__
#define CUDA_CALLABLE_MEMBER __host__ __device__
#else
#define CUDA_CALLABLE_MEMBER
#endif 

#endif /* COMMONINCLUDE_H_ */
