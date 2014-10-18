#ifndef _BULLET
#define _BULLET

#include "inc.h"
using namespace std;

class CTank;


class CBullet{

public:
	unsigned int m_nX;
	unsigned int m_nY;
	unsigned int m_nPreX;
	unsigned int m_nPreY;
	CTank *m_ptank;

	Direction m_travelingDirection;	
	
	CBullet(unsigned int nX, unsigned int nY, Direction dir, CTank* tank );
		
	void propogate();
	void propogateStep();
	void printGL();
	bool IsOutOfBound();
};

#endif
