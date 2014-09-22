#ifndef _TANK
#define _TANK

#include "inc.h"

using namespace std;
class CBullet;


class CTank{

	bool m_bFired;

public:
	Direction m_FacingDirection;
	list<CBullet> *m_bullets;
	unsigned int m_nHealth;
	unsigned int m_nX;
	unsigned int m_nY;

	CTank(){}

	CTank(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *bullet, Direction facing = DOWN);
	void set(unsigned int nHealth,unsigned int nX, unsigned int nY);
	void move(Direction direction);
	void fire();
	bool hit(unsigned int X, unsigned int nY);
	void printGL();
	void bulletHit();
};

#endif
