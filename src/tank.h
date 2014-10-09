#ifndef _TANK
#define _TANK

#include "inc.h"

using namespace std;
class CBullet;

class CPosition{
	public:
		unsigned int x;
		unsigned int y;	
		Direction direction;
		CPosition(){}
		CPosition(unsigned int _x,unsigned int _y, Direction _dir=UNKNOWN)
									:x(_x),y(_y),direction(_dir){}
		void set(int _x, int _y, Direction dir = UNKNOWN){
			x = _x;
			y = _y;
			direction = dir;
		}

		bool operator ==(CPosition pos){
			return (x==pos.x) && (y==pos.y);
		}
		bool operator != (CPosition pos){
			return (x!=pos.x) || (y!=pos.y);
		}
		CPosition& operator/(unsigned int div){
			x /= div;
			y /= div;		
			return *this;
		}

		void print(){
			cout<<"Pos :("<<x<<" , "<<y<<")"<<endl;
		}
	
		static Direction GetOppositeDirection(Direction _dir){
			switch(_dir){
				case UP:
					return DOWN;
				case DOWN:
					return UP;
				case RIGHT:
					return LEFT;
				case LEFT:
					return RIGHT;
				default:
					return UNKNOWN;
			}
		}
};

class CTank{

	bool m_bFired;
	unsigned int m_nTankSize;
	GLuint* m_Texture;
	unsigned int (*m_nMap)[MAPSIZE];
public:
	Direction m_FacingDirection;
	list<CBullet> *m_bullets;
	
	unsigned int m_nHealth;
	unsigned int m_nX;
	unsigned int m_nY;
	unsigned int m_nPreX;
	unsigned int m_nPreY;

	CTank();
	CTank(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
								GLuint *texture, unsigned int map[][MAPSIZE], Direction dir = DOWN);
	void set(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
								GLuint *texture, unsigned int map[][MAPSIZE],Direction dir= DOWN);
	void moveStep();
	void move(Direction direction);
	void fire();
	bool hit(unsigned int X, unsigned int nY);
	void printGL();
	void bulletHit();
	CPosition getPosition();
};


class CBotTank : public CTank {
	
public:
	bool SearchDone;
	vector<CPosition> path; 
	int pathPos;
	deque<CPosition> m_searchSpace;
	unsigned int m_searchedPt;
	CBotTank();
	CBotTank(unsigned int nHealth,unsigned int nX, unsigned int nY, list<CBullet> *Bullets, 
								GLuint *texture, unsigned int map[][MAPSIZE], Direction dir = DOWN);
	bool inSearchSpace(CPosition pos, CPosition* nodefound = NULL);
	void AutoMove(CPosition start,CPosition goal);
	
	void bfs(CPosition start,CPosition goal);
	void PathToGoal(CPosition start,CPosition goal);
	void getNeighbours(deque<CPosition> &list, CPosition P);
};

#endif
