#include "inc.h"
#include "tank.h"
#include "bullet.h"
#include <stdint.h>
using namespace std;

unsigned int timeperiod = 25;
CDebug debug;
//Image Dependent

#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

class CGame{

	//
	public:
	//GLuint* m_Textures;
	GLuint m_Textures[NUMTEXTURES];
	vector<unsigned char> m_PlayerPixels;
	vector<unsigned char> m_BotPixels;
	vector<unsigned char> m_BrickPixels;

		
	private:
	list<CBullet> m_BulletsEnemy;
	list<CBullet> m_BulletsPlayer;
	unsigned int m_map[MAPSIZE][MAPSIZE]; //each 15 so 510
	unsigned short m_nTimeCounter;	


	//Load a bitmap using function from stackoverflow
	void LoadBitmap(vector<unsigned char> &Pixels, const char* FilePath){

		fstream hFile(FilePath, std::ios::in | std::ios::binary);
		if (!hFile.is_open()) throw invalid_argument("Error: File Not Found.");

		hFile.seekg(0, std::ios::end);
		int Length = hFile.tellg();
		hFile.seekg(0, std::ios::beg);
		std::vector<uint8_t> FileInfo(Length);
		hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

		if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
		{
			hFile.close();
			throw invalid_argument("Error: Invalid File Format. Bitmap Required.");
		}

		if (FileInfo[28] != 24 && FileInfo[28] != 32)
		{
			hFile.close();
			throw invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
		}

		short BitsPerPixel = FileInfo[28];
		int width = FileInfo[18] + (FileInfo[19] << 8);
		int height = FileInfo[22] + (FileInfo[23] << 8);
		uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
		uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
		Pixels.resize(size);

		hFile.seekg (PixelsOffset, std::ios::beg);
		hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
		hFile.close();
	}

	
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
	
	void GameStep(){

		list<CBotTank>::iterator itT;
		list<CTank>::iterator itP;
		list<CBullet>::iterator itB;
		debug<<"hello";
		//Move Player
		switch(m_MovePlayer){
			case UP:
				m_Player.move(UP);
				m_MovePlayer = UNKNOWN;
				break;
			case DOWN:
				m_Player.move(DOWN);
				m_MovePlayer = UNKNOWN;
				break;
			case RIGHT:
				m_Player.move(RIGHT);
				m_MovePlayer = UNKNOWN;
				break;
			case LEFT:
				m_Player.move(LEFT);
				m_MovePlayer = UNKNOWN;
				break;
			default:
				break;	
		}

		//Move enemy
		for(itT = m_Enimies.begin(); itT != m_Enimies.end(); itT++){
			itT->AutoMove(itT->getPosition(), m_Player.getPosition());
		}

		//Enemy fires after each
		for(itT= m_Enimies.begin(); itT!= m_Enimies.end(); itT++){
			itT->fire();
		}

		bool skip = false;
		for(int i=0; i<BULLETSTEPS; i++){

			//Move All bullets a step
			for(itB=m_BulletsPlayer.begin(); itB!=m_BulletsPlayer.end();itB++){
				itB->propogate(skip);
			}
			for(itB=m_BulletsEnemy.begin(); itB!=m_BulletsEnemy.end(); itB++)
					itB->propogate(skip);

			//Check for out of bound	
			checkOutOfBound(&m_BulletsEnemy);
			checkOutOfBound(&m_BulletsPlayer);
			
			//check for map interaction
			checkMapInter(m_BulletsPlayer);
			checkMapInter(m_BulletsEnemy);

			
			//check for collision player bullet -> enemy
			itB = m_BulletsPlayer.begin();
			bool bHit = false;
			while(itB!= m_BulletsPlayer.end()){
				itT= m_Enimies.begin();
				while(itT!= m_Enimies.end()){
					bHit = false;	
					if(itT->hit(itB->m_nX, itB->m_nY)){
						itB->m_ptank->bulletHit();	
						debugPln("HIT Bot");
						bHit = true;
						m_BulletsPlayer.erase(itB++)	;
						m_Enimies.erase(itT++)	;
						break;
					}
					else{
						itT++;
					}
				}
				if(!bHit) itB++;
			}

			//check for collision enemy bullet -> player 
			itB = m_BulletsEnemy.begin();
			bHit = false;
			while(itB!= m_BulletsEnemy.end()){
				if(m_Player.hit(itB->m_nX, itB->m_nY)){
						debugPln("HIT Player");
						itB->m_ptank->bulletHit();
						m_BulletsEnemy.erase(itB++)	;
						--m_Player.m_nHealth;
						if(!m_Player.m_nHealth){
							bHit = true;
							debugPln("GAME OVER");
							break;
						}
				}
				itB++;
			}	
			if(!skip) {skip=true;}
		}
	

	}

public:
	list<CBotTank> m_Enimies;
	CTank m_Player;	
	Direction m_MovePlayer;
	CGame(){
		debug<<"------------------DEBUG ON-----------------";
		//m_Textures = new GLuint[NUMTEXTURES];
		m_nTimeCounter = 0;
		m_Player.set(3,150,150,&m_BulletsPlayer,&m_Textures[0],m_map,UP);
		CBotTank enemy1(3,150,30,&m_BulletsEnemy,&m_Textures[1],m_map);
		CBotTank enemy2(3,300,30,&m_BulletsEnemy,&m_Textures[1],m_map);
		CBotTank enemy3(3,30,30,&m_BulletsEnemy,&m_Textures[1],m_map);
		m_Enimies.push_back(enemy1);
		m_Enimies.push_back(enemy2);
		m_Enimies.push_back(enemy3);
		memset(m_map,0,sizeof(m_map));


		//map is 500/15 = about 34
		//
		
		for(int i=15; i<30; i++){
			for(int j=15; j<30; j++){
				m_map[i][j] = 1;
			}
		}
		m_map[10][14] = 1;
		m_map[11][15] = 1;
		m_map[11][14] = 1;
		m_map[10][20] = 1;
		m_map[33][33] = 1;
	
	}

	void loadTexture(GLuint &texture, const char* filename, vector<unsigned char> &Pixels)
	{
		LoadBitmap(Pixels, filename);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 68, 68, 0, GL_BGRA, 
											GL_UNSIGNED_BYTE,Pixels.data());	
	}

	void initGL(){
		glGenTextures(NUMTEXTURES, m_Textures);
		loadTexture(m_Textures[0], "src/tank.bmp", m_PlayerPixels);
		loadTexture(m_Textures[1], "src/bot.bmp", m_BotPixels);
		loadTexture(m_Textures[2], "src/bricks.bmp", m_BrickPixels);
	}


	void printMap(){
		int stepX = 0;
		int stepY = 0;
	
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, m_Textures[2]);
	
		for(int i=0; i<MAPSIZE; i++){
			for(int j=0; j<MAPSIZE; j++){
				glBegin( GL_QUADS );
					if(m_map[i][j] == 1){
						stepX = i*UNIT;	
						stepY = j*UNIT;	
						glTexCoord2d(0,0); glVertex2d(stepX, stepY);
						glTexCoord2d(1,0); glVertex2d(stepX+UNIT,stepY);
						glTexCoord2d(1,1); glVertex2d(stepX+UNIT,stepY+UNIT);
						glTexCoord2d(0,1); glVertex2d(stepX,stepY+UNIT);
					}
				glEnd();
			}
		}
		
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void printGL(){
		
		list<CBotTank>::iterator itT;
		list<CBullet>::iterator itB;
		
		//Print Map
		printMap();	

		//Print Player
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

	void checkMapInter(list<CBullet> &vec){
		
		list<CBullet>::iterator itB;
		itB = vec.begin();
		while(itB != vec.end()){
			unsigned int mapx0 = floor((itB->m_nX+1)/UNIT);
			unsigned int mapx1 = floor((itB->m_nX-1)/UNIT);
			unsigned int mapy0 = floor((itB->m_nY+1)/UNIT);
			unsigned int mapy1 = floor((itB->m_nY-1)/UNIT);
			bool bhit = false;

			switch(itB->m_travelingDirection){
				case DOWN:
					if(m_map[mapx0][mapy0] == 1){ m_map[mapx0][mapy0] = 0; bhit = true;}
					if(m_map[mapx1][mapy0] == 1){ m_map[mapx1][mapy0] = 0; bhit = true;}
					break;
				case UP:
					if(m_map[mapx0][mapy1] == 1){ m_map[mapx0][mapy1] = 0; bhit = true;}
					if(m_map[mapx1][mapy1] == 1){ m_map[mapx1][mapy1] = 0; bhit = true;}
					break;
				case RIGHT:
					if(m_map[mapx0][mapy0] == 1){ m_map[mapx0][mapy0] = 0; bhit = true;}
					if(m_map[mapx0][mapy1] == 1){ m_map[mapx0][mapy1] = 0; bhit = true;}
					break;
				case LEFT:
					if(m_map[mapx1][mapy0] == 1){ m_map[mapx1][mapy0] = 0; bhit = true;}
					if(m_map[mapx1][mapy1] == 1){ m_map[mapx1][mapy1] = 0; bhit = true;}
					break;
				default:
					break;
			}
			if(bhit){
				itB->m_ptank->bulletHit();		
				vec.erase(itB++);
			}
			else{
				itB++;
			}
			
		}
	}



	void InterpolateStep(){
		m_Player.moveStep();
		list<CBotTank>::iterator itT;
		list<CBullet>::iterator itB;

		for(itT = m_Enimies.begin(); itT != m_Enimies.end(); itT++){
			itT->moveStep();
		}
	
		for(itB=m_BulletsPlayer.begin(); itB!=m_BulletsPlayer.end();itB++){
			itB->propogateStep();
		}
		for(itB=m_BulletsEnemy.begin(); itB!=m_BulletsEnemy.end(); itB++){
			itB->propogateStep();
		}

	}

	void timeStep(){

		//cout<<"Time Inter :"<<m_nTimeCounter<<endl;
		if(m_nTimeCounter++ >= TIMEDIVISOR){
			//cout<<"Time Game:"<<endl;
			GameStep();
			m_nTimeCounter = 0;
		}
		InterpolateStep();
	}

};
CGame thegame;

/*
void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}*/


//____________________________________________________
// OpenGL Shizzel
//____________________________________________________

void init(){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize(N,N);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Tank");
	//glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearColor(0.5f,0.5f,0.5f,0.0f);
	thegame.initGL();
}

void keyboard(unsigned char k, int x, int y){
	//cout<<endl;

	switch(k){
		case 'w':
			//cout<<"up"<<endl;
			//thegame.m_Player.move(UP);
			thegame.m_MovePlayer = UP;
			break;
		case 's':
			//cout<<"down"<<endl;
			//thegame.m_Player.move(DOWN);
			thegame.m_MovePlayer = DOWN;
			break;
		case 'd':
			//cout<<"right"<<endl;
			//thegame.m_Player.move(RIGHT);
			thegame.m_MovePlayer = RIGHT;
			break;
		case 'a':
			//cout<<"left"<<endl;
			//thegame.m_Player.move(LEFT);
			thegame.m_MovePlayer = LEFT;
			break;
		case 'q':
			cout<<"exit"<<endl;
			exit(EXIT_FAILURE);
			break;
		case 'j':
			//cout<<"Fire"<<endl;
			thegame.m_Player.fire();
			break;
		case 't':
			//cout<<"Fire"<<endl;
			//thegame.m_Enimies[0].getNeighbours(;
			break;
	}
	
	//glutPostRedisplay();
	
}

void reshape(int w, int h){
	//cout<<"GL redisplay function <--"<<endl;
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(double)w,(double)h,0.0);
}

void display(){
	//cout<<"display function <--"<<endl;
 	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	thegame.printGL();

/*
	glBindTexture( GL_TEXTURE_2D, thegame.m_PlayerTexture);
	glBegin( GL_QUADS );
	glTexCoord2d(0,0); glVertex2d(0,0);
	glTexCoord2d(1,0); glVertex2d(64,0);
	glTexCoord2d(1,1); glVertex2d(64,64);
	glTexCoord2d(0,1); glVertex2d(0,64);
	glEnd();
*/

	glFlush();
	glutSwapBuffers();
}

void timerfunc(int t){
//	cout<<"timer"<<endl;
	thegame.timeStep();

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
