#include "inc.h"
#include "tank.h"
#include "bullet.h"

using namespace std;

int timeperiod = 150;


class CGame{

public:
	CTank m_Player;	
	list<CTank> m_Enimies;

	CGame():m_Player(3,N/2,N/2){
		CTank enemy1(3,N/3,N/3);
		CTank enemy2(3,N/2,5);
		m_Enimies.push_back(enemy1);
		m_Enimies.push_back(enemy2);
	}
	
	void printGL(){
		m_Player.printGL();	

		//Print enimies
		list<CTank>::iterator eme;
		for(eme = m_Enimies.begin(); eme!= m_Enimies.end(); eme++){
			eme->printGL();
		}

		//Print Bullets
		list<CBullet> *bullet = &m_Player.m_bullets;
		list<CBullet>::iterator it;
		for(it = bullet->begin(); it != bullet->end(); it++){
			it->printGL();
		}

	}

	void timestep(){

		//Move All bullets a step
		list<CBullet> *bullet = &m_Player.m_bullets;
		list<CBullet>::iterator it;
		for(it = bullet->begin(); it != bullet->end(); it++){
			it->propogate();
		}

		//Check for collision and out of bound
		it = bullet->begin();
		while(it!= bullet->end()){
			if(it->IsOutOfBound()){
				bullet->erase(it++)	;
			}
			else
				it++;
		}
		
		//check for collision
		it = bullet->begin();
		list<CTank>::iterator eme;
		bool bHit = false;
		while(it!= bullet->end()){
			eme = m_Enimies.begin();
			while(eme != m_Enimies.end()){
				bHit = false;	
				if(eme->hit(it->m_nX, it->m_nY)){
					cout<<"HIT"<<endl;
					bHit = true;
					bullet->erase(it++)	;
					m_Enimies.erase(eme++)	;
					break;
				}
				eme++;
			}
			if(!bHit) it++;
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
