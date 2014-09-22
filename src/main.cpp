#include "inc.h"
#include "tank.h"
#include "bullet.h"

using namespace std;

int timeperiod = 150;


class CGame{

	list<CTank> m_Enimies;
	list<CBullet> m_BulletsEnemy;
	list<CBullet> m_BulletsPlayer;
	int map[N/STEP][N/STEP];
	
	void checkOutOfBound(list<CBullet> * bulletPtr){
		list<CBullet>::iterator itB = bulletPtr->begin();
		while(itB!= bulletPtr->end()){
			if(itB->IsOutOfBound()){
				itB->m_ptank->bulletHit();	
				bulletPtr->erase(itB++);
			}
			else
				itB++;
		}
	}

public:
	CTank m_Player;	

	CGame():m_Player(3,N/2,N/2,&m_BulletsPlayer,UP){
		CTank enemy1(3,N/3,30,&m_BulletsEnemy);
		CTank enemy2(3,N/2,30,&m_BulletsEnemy);
		m_Enimies.push_back(enemy1);
		m_Enimies.push_back(enemy2);
		memset(map,0,sizeof(map));
		
	}
	
	
	void printGL(){
		
		list<CTank>::iterator itT;
		list<CBullet>::iterator itB;
		
		m_Player.printGL();	

		//Print enimies
		for(itT=m_Enimies.begin(); itT!=m_Enimies.end(); itT++){
			itT->printGL();
		}

		//Print Bullets
		for(itB=m_BulletsEnemy.begin();itB!=m_BulletsEnemy.end(); itB++)
			itB->printGL();		
		
		for(itB=m_BulletsPlayer.begin();itB!=m_BulletsPlayer.end(); itB++)
			itB->printGL();		
		
	
	}

	void timestep(){

		list<CTank>::iterator itT;
		list<CBullet>::iterator itB;

		//Move All bullets a step
		for(itB=m_BulletsPlayer.begin(); itB!=m_BulletsPlayer.end();itB++)
			itB->propogate();

		for(itB=m_BulletsEnemy.begin(); itB!=m_BulletsEnemy.end(); itB++)
				itB->propogate();
		
		//Check for out of bound	
		checkOutOfBound(&m_BulletsEnemy);
		checkOutOfBound(&m_BulletsPlayer);
		
		//check for collision player bullet -> enemy
		itB = m_BulletsPlayer.begin();
		bool bHit = false;
		while(itB!= m_BulletsPlayer.end()){
			itT= m_Enimies.begin();
			while(itT!= m_Enimies.end()){
				bHit = false;	
				if(itT->hit(itB->m_nX, itB->m_nY)){
					itB->m_ptank->bulletHit();	
					cout<<"---HIT---"<<endl;
					bHit = true;
					m_BulletsPlayer.erase(itB++)	;
					m_Enimies.erase(itT++)	;
					break;
				}
				itT++;
			}
			if(!bHit) itB++;
		}

		//check for collision enemy bullet -> player 
		itB = m_BulletsEnemy.begin();
		bHit = false;
		while(itB!= m_BulletsEnemy.end()){
			if(m_Player.hit(itB->m_nX, itB->m_nY)){
					cout<<"HIT"<<endl;
					itB->m_ptank->bulletHit();
					m_BulletsEnemy.erase(itB++)	;
					--m_Player.m_nHealth;
					if(!m_Player.m_nHealth){
						bHit = true;
						cout<<"GAME OVER"<<endl;	
						break;
					}
			}
			itB++;
		}	

	
		//Enemy fires after each
		for(itT= m_Enimies.begin(); itT!= m_Enimies.end(); itT++){
			itT->fire();
		}

	}

};
CGame thegame;

//____________________________________________________
// OpenGL Shizzel
//____________________________________________________

void init(){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize(N,N);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Tank");
	glClearColor(0.5f,0.5f,0.5f,0.0f);
}

void keyboard(unsigned char k, int x, int y){
	cout<<endl;

	switch(k){
		case 'w':
			cout<<"up"<<endl;
			thegame.m_Player.move(UP);
			break;
		case 's':
			cout<<"down"<<endl;
			thegame.m_Player.move(DOWN);
			break;
		case 'd':
			cout<<"right"<<endl;
			thegame.m_Player.move(RIGHT);
			break;
		case 'a':
			cout<<"left"<<endl;
			thegame.m_Player.move(LEFT);
			break;
		case 'q':
			cout<<"exit"<<endl;
			exit(EXIT_FAILURE);
			break;
		case 'j':
			cout<<"Fire"<<endl;
			thegame.m_Player.fire();
			break;
	}
	
	glutPostRedisplay();
	
}

void reshape(int w, int h){
	cout<<"GL redisplay function <--"<<endl;
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(double)w,(double)h,0.0);
}

void display(){
 	glClear(GL_COLOR_BUFFER_BIT);
	cout<<"display function <--"<<endl;
	//thegame.printBoard();
	thegame.printGL();


	glFlush();
	glutSwapBuffers();
}

void timerfunc(int t){
	cout<<"timer"<<endl;
	thegame.timestep();

	glutPostRedisplay();
	glutTimerFunc(t,timerfunc,timeperiod);
}

int main(int argc, char *argv[]) {

	
	//Init glut
	glutInit(&argc, argv);

	//init to make window
	init();

	glutTimerFunc(100,timerfunc, timeperiod);

	//glutKeyboard
	glutKeyboardFunc(keyboard);

	//Disolay callback
	glutDisplayFunc(display);
	
	//glut reshapw
	glutReshapeFunc(reshape);

	//glut main loop
	glutMainLoop();


	return 0;
}
