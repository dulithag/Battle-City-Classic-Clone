#include "tank.h"
#include "bullet.h"

CTank::CTank(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, Direction direction):
	m_nHealth(nHealth),m_nX(nX),m_nY(nY){
		m_FacingDirection = direction;
		m_bullets = Bullets;	
		m_bFired = false;
}

void CTank::set(unsigned int nHealth,unsigned int nX, unsigned int nY){
	m_nHealth = nHealth;
	m_nX = nX;
	m_nY = nY;
}

void CTank::move(Direction direction){
	m_FacingDirection = direction;
	int nDirection = (direction == LEFT) || 
							(direction == UP) ? -1 : 1;
	int nStep = (nDirection*STEP);
	//Y
	if((direction == UP) || (direction == DOWN)){
		if(m_nY+nStep>PIXSIZE/2 && m_nY+nStep<N-PIXSIZE/2){
			m_nY += nStep; 
		}
	}
	//X
	else{
		if(m_nX+nStep>PIXSIZE/2 && m_nX+nStep<N-PIXSIZE/2){
			m_nX += nStep;
		}
	}
}

void CTank::fire(){
		if(!m_bFired){
			CBullet temp(m_nX,m_nY,m_FacingDirection,this);
			m_bullets->push_back(temp);
			m_bFired = true;	
		}
}

void CTank::bulletHit(){
		m_bFired = false;
}

void CTank::printGL(){
	glPointSize(PIXSIZE);
		glBegin(GL_POINTS);
			glVertex3f(m_nX,m_nY,0);
	glEnd();
}

bool CTank::hit(unsigned int X, unsigned int Y){
	unsigned int nX0 = m_nX - PIXSIZE/2; 
	unsigned int nX1 = m_nX + PIXSIZE/2; 
	unsigned int nY0 = m_nY - PIXSIZE/2; 
	unsigned int nY1 = m_nY + PIXSIZE/2; 

	return (X<nX1 && X>nX0) && (Y<nY1 && Y>nY0);
	
}
