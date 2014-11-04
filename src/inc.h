#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <list>
#include <string.h>
#include <fstream>
#include <stdexcept>
#include <deque>
//#include "debug.h"

#ifndef _INC
#define _INC
#define N 500 
#define TANKSIZE 30
#define STEP 15
#define UNIT 15 
#define MAPSIZE 34
#define NUMTEXTURES 3
#define TIMER_TANKSTEP 5
#define TIMER_BULLETSTEP 3
enum Direction {UP, DOWN, RIGHT, LEFT, UNKNOWN};

//#define DEBUG

#include "debug.h"

#ifdef DEBUG
#define debugP(s) std::cout<<s
#define debugPln(s) std::cout<<s<<endl
#else
#define debugP(s) 
#define debugPln(s) 
#endif


#endif
