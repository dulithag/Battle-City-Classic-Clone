#include "bullet.h"


CBullet::CBullet(unsigned int nX, unsigned int nY, Direction dir ):
					m_nX(nX),m_nY(nY),m_travelingDirection(dir){
	bDelete = false;
}

	
void CBullet::propogate(){
	int nDirection = (m_travelingDirection == LEFT) || 
							(m_travelingDirection == UP) ? -1 : 1;
	int nStep = nDirection * STEP * 5;
	if((m_travelingDirection == UP) || (m_travelingDirection == DOWN))
			m_nY += nStep;
	else
			m_nX += nStep;
}	

bool CBullet::IsOutOfBound(){
		return m_nY<50 || m_nY>N || m_nX<0 || m_nX>N;
}

void CBullet::printGL(){
	glPointSize(5);
	glBegin(GL_POINTS);
		glVertex3f(m_nX,m_nY,0);
	glEnd();
}


