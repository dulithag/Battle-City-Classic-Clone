#include "inc.h"

using namespace std;

#ifndef _BULLET
#define _BULLET

class CBullet{

public:
	unsigned int m_nX;
	unsigned int m_nY;
	bool bDelete;

	Direction m_travelingDirection;	
	
	CBullet(unsigned int nX, unsigned int nY, Direction dir );
		
	void propogate();

	void printGL();
	bool IsOutOfBound();
};

#endif
