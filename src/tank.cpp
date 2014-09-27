#include "tank.h"
#include "bullet.h"

CTank::CTank(){
		m_nTankSize = TANKSIZE;
}

CTank::CTank(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
								GLuint *texture,  unsigned int map[][MAPSIZE], Direction direction){
	m_nHealth = nHealth;
	m_nX = nX;
	m_nY = nY;
	m_FacingDirection = direction;
	m_bullets = Bullets;	
	m_bFired = false;
	m_Texture = texture;
	m_nTankSize = TANKSIZE;
	m_nMap = map;	
}

void CTank::set(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
								GLuint *texture, unsigned int map[][MAPSIZE], Direction direction)
{
	m_nHealth = nHealth;
	m_nX = nX;
	m_nY = nY;
	m_FacingDirection = direction;
	m_bullets = Bullets;	
	m_bFired = false;
	m_Texture = texture;
	m_nMap = map;	
}

void CTank::move(Direction direction){
	m_FacingDirection = direction;
	int nDirection = (direction == LEFT) || 
							(direction == UP) ? -1 : 1;
	int nStep = (nDirection*STEP);
	int nxtStepX = m_nX + nStep;
	int nxtStepY = m_nY + nStep;

	unsigned int nTankSize = m_nTankSize - 1;	
	int nxtmapX0 = (int)floor(nxtStepX/UNIT);
	int nxtmapY0 = (int)floor(nxtStepY/UNIT);
	int nxtmapX1 = (int)floor((nxtStepX+nTankSize)/UNIT);
	int nxtmapY1 = (int)floor((nxtStepY+nTankSize)/UNIT);
	int mapX0 = (int)floor(m_nX/UNIT);
	int mapY0 = (int)floor(m_nY /UNIT);
	int mapX1 = (int)floor((m_nX +nTankSize)/UNIT);
	int mapY1 = (int)floor((m_nY +nTankSize)/UNIT);
	int mapX2 = (int)floor((m_nX +nTankSize/2)/(UNIT));
	int mapY2 = (int)floor((m_nY +nTankSize/2)/(UNIT));

	switch(direction){
		case UP:
			if(nxtStepY>=0 && 
				m_nMap[mapX0][nxtmapY0] == 0 &&
				m_nMap[mapX2][nxtmapY0] == 0 &&
				m_nMap[mapX1][nxtmapY0] == 0)
					m_nY += nStep; 
			break;
		case DOWN:
			if(nxtStepY<=N-m_nTankSize && 
				m_nMap[mapX0][nxtmapY1] == 0 &&
				m_nMap[mapX2][nxtmapY1] == 0 &&
				m_nMap[mapX1][nxtmapY1] == 0)
					m_nY += nStep; 
			break;
		case LEFT:	
			if(nxtStepX>=0 && 
				m_nMap[nxtmapX0][mapY0] == 0 &&
				m_nMap[nxtmapX0][mapY2] == 0 &&
				m_nMap[nxtmapX0][mapY1] == 0)
					m_nX += nStep; 
			break;
		case RIGHT:
			if(nxtStepX<=N-m_nTankSize && 
				m_nMap[nxtmapX1][mapY0] == 0 && 
				m_nMap[nxtmapX1][mapY2] == 0 && 
				m_nMap[nxtmapX1][mapY1] == 0)
					m_nX += nStep; 
			break;
			
	}

/*
	//Y
	if((direction == UP) || (direction == DOWN)){
		int mapX = (int)floor(m_nX/UNIT);
		int mapY = (int)floor((m_nY+nStep)/UNIT);
		if(m_nY+nStep>=0 && m_nY+nStep<=N-m_nTankSize && 
				m_nMap[mapX][mapY] == 0){
			m_nY += nStep; 
		}
	}
	//X
	else{
		if(m_nX+nStep>=0 && m_nX+nStep<=N-m_nTankSize && 
				m_nMap[(int)floor((m_nX+nStep)/UNIT)][(int)floor(m_nY/UNIT)] == 0){
			m_nX += nStep;
		}
	}
*/
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

	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
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
