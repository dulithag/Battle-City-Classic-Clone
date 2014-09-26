#include "tank.h"
#include "bullet.h"

CTank::CTank(){
		m_nTankSize = TANKSIZE;
}

CTank::CTank(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
															GLuint *texture, Direction direction):
	m_nHealth(nHealth),m_nX(nX),m_nY(nY){
		m_FacingDirection = direction;
		m_bullets = Bullets;	
		m_bFired = false;
		m_Texture = texture;
		m_nTankSize = TANKSIZE;
}

void CTank::set(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
															GLuint *texture, Direction direction)
{
	m_nHealth = nHealth;
	m_nX = nX;
	m_nY = nY;
	m_FacingDirection = direction;
	m_bullets = Bullets;	
	m_bFired = false;
	m_Texture = texture;
}

void CTank::move(Direction direction){
	m_FacingDirection = direction;
	int nDirection = (direction == LEFT) || 
							(direction == UP) ? -1 : 1;
	int nStep = (nDirection*STEP);
	//Y
	if((direction == UP) || (direction == DOWN)){
		if(m_nY+nStep>=0 && m_nY+nStep<=N-m_nTankSize){
			m_nY += nStep; 
		}
	}
	//X
	else{
		if(m_nX+nStep>=0 && m_nX+nStep<=N-m_nTankSize){
			m_nX += nStep;
		}
	}
	cout<<"Position: "<<m_nX<<" "<<m_nY<<endl;
}

void CTank::fire(){
		unsigned int nFireOffsetX = m_nTankSize/2;
		unsigned int nFireOffsetY = m_nTankSize/2;
		if(!m_bFired){
			CBullet temp(m_nX+nFireOffsetX,m_nY+nFireOffsetY,m_FacingDirection,this);
			m_bullets->push_back(temp);
			m_bFired = true;	
		}
}

void CTank::bulletHit(){
		m_bFired = false;
}

void CTank::printGL(){

/*
		glPointSize(PIXSIZE);
			glBegin(GL_POINTS);
				glVertex3f(m_nX,m_nY,0);
		glEnd();
*/

	glEnable(GL_TEXTURE_2D);
//	glColor4f(1,1,1,1);
	glBindTexture( GL_TEXTURE_2D, *m_Texture);

	/*
	glBegin( GL_QUADS );
		glTexCoord2d(0,0); glVertex2d(m_nX-nTankSize,m_nY-nTankSize);
		glTexCoord2d(1,0); glVertex2d(m_nX+nTankSize,m_nY-nTankSize);
		glTexCoord2d(1,1); glVertex2d(m_nX+nTankSize,m_nY+nTankSize);
		glTexCoord2d(0,1); glVertex2d(m_nX-nTankSize,m_nY+nTankSize);
	glEnd();
	*/

	GLdouble x0 = 0;
	GLdouble x1 = 0;
	GLdouble x2 = 0;
	GLdouble x3 = 0;
	GLdouble y0 = 0;
	GLdouble y1 = 0;
	GLdouble y2 = 0;
	GLdouble y3 = 0;

	switch(m_FacingDirection){
		case UP:
			x1 = x2 = y2 = y3 = 1;
			break;
		case DOWN:
			y0 = x1 = y1 = x2 = 1;
			break;
		case LEFT:
			y1 = x2 = y2 = x3 = 1;	
			break;
		case RIGHT:
			y0 = x2 = x3 = y3 = 1;	
			break;
	}

	glBegin( GL_QUADS );
		glTexCoord2d(x0,y0); glVertex2d(m_nX,m_nY);
		glTexCoord2d(x1,y1); glVertex2d(m_nX+m_nTankSize,m_nY);
		glTexCoord2d(x2,y2); glVertex2d(m_nX+m_nTankSize,m_nY+m_nTankSize);
		glTexCoord2d(x3,y3); glVertex2d(m_nX,m_nY+m_nTankSize);
	glEnd();

/*	
	glBegin( GL_QUADS );
		glTexCoord2d(0,0); glVertex2d(m_nX-nTankSize,m_nY-nTankSize);
		glTexCoord2d(0,1); glVertex2d(m_nX+nTankSize,m_nY-nTankSize);
		glTexCoord2d(2,2); glVertex2d(m_nX+nTankSize,m_nY+nTankSize);
		glTexCoord2d(1,0); glVertex2d(m_nX-nTankSize,m_nY+nTankSize);
	glEnd();
*/


}

bool CTank::hit(unsigned int X, unsigned int Y){
	unsigned int nX0 = m_nX ; 
	unsigned int nX1 = m_nX + m_nTankSize; 
	unsigned int nY0 = m_nY ; 
	unsigned int nY1 = m_nY + m_nTankSize; 

	return (X<=nX1 && X>=nX0) && (Y<=nY1 && Y>=nY0);
	
}
