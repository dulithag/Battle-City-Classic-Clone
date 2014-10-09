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
	m_nPreX = nX;
	m_nPreY = nY;
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
	m_nPreX = nX;
	m_nPreY = nY;
	m_FacingDirection = direction;
	m_bullets = Bullets;	
	m_bFired = false;
	m_Texture = texture;
	m_nMap = map;	
}


void CTank::moveStep(){
	if(int(m_nX) - int(m_nPreX) > 0)	
		m_nPreX += UNIT/TIMEDIVISOR;
	if(int(m_nX) - int(m_nPreX) < 0){	
		m_nPreX -= UNIT/TIMEDIVISOR;
		cout<<"Moving"<<endl;
	}
	if(int(m_nY) - int(m_nPreY) > 0)	
		m_nPreY += UNIT/TIMEDIVISOR;
	if(int(m_nY) - int(m_nPreY) < 0)	
		m_nPreY -= UNIT/TIMEDIVISOR;
}

void CTank::move(Direction direction){
	m_FacingDirection = direction;
	int nDirection = (direction == LEFT) || 
							(direction == UP) ? -1 : 1;
	int nStep = (nDirection*STEP);
	m_nPreX = m_nX;
	m_nPreY = m_nY;
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
			if(nxtStepY<=int(N-(int)m_nTankSize) && 
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
			if(nxtStepX<=int(N-m_nTankSize) && 
				m_nMap[nxtmapX1][mapY0] == 0 && 
				m_nMap[nxtmapX1][mapY2] == 0 && 
				m_nMap[nxtmapX1][mapY1] == 0)
					m_nX += nStep; 
			break;
		default:
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
	debugP("Position: ");debugP(m_nX);debugP(",");debugPln(m_nY);
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
		default:
			break;
	}

/*
	glBegin( GL_QUADS );
		glTexCoord2d(x0,y0); glVertex2d(m_nX,m_nY);
		glTexCoord2d(x1,y1); glVertex2d(m_nX+m_nTankSize,m_nY);
		glTexCoord2d(x2,y2); glVertex2d(m_nX+m_nTankSize,m_nY+m_nTankSize);
		glTexCoord2d(x3,y3); glVertex2d(m_nX,m_nY+m_nTankSize);
	glEnd();
*/
	glBegin( GL_QUADS );
		glTexCoord2d(x0,y0); glVertex2d(m_nPreX,m_nPreY);
		glTexCoord2d(x1,y1); glVertex2d(m_nPreX+m_nTankSize,m_nPreY);
		glTexCoord2d(x2,y2); glVertex2d(m_nPreX+m_nTankSize,m_nPreY+m_nTankSize);
		glTexCoord2d(x3,y3); glVertex2d(m_nPreX,m_nPreY+m_nTankSize);
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

CPosition CTank::getPosition(){
	return CPosition(m_nX,m_nY,m_FacingDirection);
}


//================================================================
//BOT TANK
//================================================================

	CBotTank::CBotTank(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
								GLuint *texture, unsigned int map[][MAPSIZE], Direction dir )
										:CTank(nHealth,nX, nY, Bullets, texture, map, dir){
SearchDone = false;	}

	void CBotTank::getNeighbours(deque<CPosition> &list, CPosition P){
		//cout<<P.x<<" "<<P.y<<endl;
	
		CPosition p0((int)P.x+1,P.y,LEFT); 
		CPosition p1((int)P.x-1,P.y,RIGHT); 
		CPosition p2(P.x,(int)P.y+1,UP); 
		CPosition p3(P.x,(int)P.y-1,DOWN); 

		//cout<<nNX0 << nNX1 << nNY0 << nNY1 <<endl;
		if(p0.x>=0 && p0.x<MAPSIZE && p0.y>=0 && p0.y<MAPSIZE && !inSearchSpace(p0))
			list.push_back(p0);
		if(p1.x>=0 && p1.x<MAPSIZE && p1.y>=0 && p1.y<MAPSIZE && !inSearchSpace(p1))
			list.push_back(p1);
		if(p2.x>=0 && p2.x<MAPSIZE && p2.y>=0 && p2.y<MAPSIZE && !inSearchSpace(p2))
			list.push_back(p2);
		if(p3.x>=0 && p3.x<MAPSIZE && p3.y>=0 && p3.y<MAPSIZE && !inSearchSpace(p3))
			list.push_back(p3);
		
/*
		if(!(P0.x<0 || nNX0>=MAPSIZE))
			list.push_back(P0);	
		if(!(nNX1<0 || nNX1>=MAPSIZE))
			list.push_back(P1);	
		if(!(nNY0<0 || nNY0>=MAPSIZE))
			list.push_back(P2);	
		if(!(nNY1<0 || nNY1>=MAPSIZE))
			list.push_back(P3);	
*/
		//cout<<"Neighbour List size :"<<list.size()<<endl;
	}

	bool CBotTank::inSearchSpace(CPosition pos, CPosition* nodefound){
		for (unsigned int i = 0; i < m_searchSpace.size(); i++) {
			if(m_searchSpace[i] == pos){
				if(nodefound != NULL) *nodefound = m_searchSpace[i];
				return true;
			}
		}
		return false;
	}
	

	void CBotTank::bfs(CPosition start,CPosition goal){

		CPosition CheckNode;
		start.x = m_nX/UNIT;
		start.y = m_nY/UNIT;
		m_searchedPt = 0;

		cout<<"Start BFS"<<endl;
		cout<<"Start search at ("<<start.x<<" , "<<start.y<<")"<<endl;
		cout<<"Goal Node at ("<<goal.x<<" , "<<goal.y<<")"<<endl;

		//init
		m_searchSpace.push_back(start);
		while(m_searchedPt < m_searchSpace.size()){

			CheckNode = m_searchSpace[m_searchedPt++];
			cout<<"Checking ("<<CheckNode.x<<" , "<<CheckNode.y<<")"<<endl;

			if(CheckNode == goal){
				cout<<"FOUND"<<endl;
				m_searchSpace.push_back(CheckNode);
				break;
			}
			else{
				getNeighbours(m_searchSpace, CheckNode);
				//for(unsigned int i=0; i<m_searchSpace.size(); i++){
				//	cout<<m_searchSpace[i].x<<" , "<<m_searchSpace[i].y<<endl;
				//}	
			}	
		}
		cout<<"End BFS"<<endl;

			
		//Find Path to goal node
		while(CheckNode != start){
			//cout<<"here"<<endl;
			CPosition nextNode;
			switch(CheckNode.direction){
				case UP:
					nextNode.set(CheckNode.x,CheckNode.y-1);
					break;
				case DOWN:
					nextNode.set(CheckNode.x,CheckNode.y+1);
					break;
				case LEFT:
					nextNode.set(CheckNode.x-1,CheckNode.y);
					break;
				case RIGHT:
					nextNode.set(CheckNode.x+1,CheckNode.y);
					break;
				default:
					break;

			}			
			bool gotit = inSearchSpace(nextNode,&CheckNode);
		
			if(gotit){
				path.push_back(CheckNode);
			}
			else{
				cout<<"ERROR finding path"<<endl;
				break;
			}
		}

		/*	
		cout<<"PATH ->"<<endl;
		for(int i=0; i<path.size();i++){
			cout<<"("<<path[i].x<<" "<<path[i].y<<")"<<endl;
			}
		}
		*/
		
		
	
	}
	
	void CBotTank::AutoMove(CPosition _start,CPosition _goal){
		CPosition start = _start/UNIT;	
		CPosition goal = _goal/UNIT;	
		if(path.size() <= 0){
			m_searchSpace.clear();
			bfs(start, goal);
		}
		//PathToGoal(start, m_searchSpace.back());
		cout<<"Path size "<<path.size()<<endl;
		if(path.size()>0){
			path.pop_back();
			move(CPosition::GetOppositeDirection(path.back().direction));
		}
		//path.clear();
	}





