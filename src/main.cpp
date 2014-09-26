#include "inc.h"
#include "tank.h"
#include "bullet.h"
#include <stdint.h>

using namespace std;

int timeperiod = 150;

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

	private:
	list<CTank> m_Enimies;
	list<CBullet> m_BulletsEnemy;
	list<CBullet> m_BulletsPlayer;
	int map[N/STEP][N/STEP];

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

public:
	CTank m_Player;	

	CGame(){

		//m_Textures = new GLuint[NUMTEXTURES];

		m_Player.set(3,100,140,&m_BulletsPlayer,&m_Textures[0],UP);
		CTank enemy1(3,N/3,30,&m_BulletsEnemy,&m_Textures[1]);
		CTank enemy2(3,N/2,30,&m_BulletsEnemy,&m_Textures[1]);
		CTank enemy3(3,30,30,&m_BulletsEnemy,&m_Textures[1]);
		m_Enimies.push_back(enemy1);
		m_Enimies.push_back(enemy2);
		m_Enimies.push_back(enemy3);
		memset(map,0,sizeof(map));
	}

	void loadTexture(GLuint &texture, const char* filename, vector<unsigned char> &Pixels)
	{
		LoadBitmap(Pixels, filename);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 68, 68, 0, GL_RGBA, 
											GL_UNSIGNED_BYTE,Pixels.data());	
	}

	void initGL(){
		glGenTextures(NUMTEXTURES, m_Textures);
		loadTexture(m_Textures[0], "src/tank.bmp", m_PlayerPixels);
		loadTexture(m_Textures[1], "src/bot.bmp", m_BotPixels);

	
		//makeCheckImage();
		/*
		LoadBitmap(m_PlayerPixels, "src/tank.bmp");
		LoadBitmap(m_BotPixels, "src/bot.bmp");

		glGenTextures(1, &m_PlayerTexture);
		glBindTexture(GL_TEXTURE_2D, m_PlayerTexture);
		glGenTextures(2, &m_BotTexture);
		glBindTexture(GL_TEXTURE_2D, m_BotTexture);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 68, 68, 0, GL_RGBA, GL_UNSIGNED_BYTE,m_PlayerPixels.data());	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 68, 68, 0, GL_RGBA, GL_UNSIGNED_BYTE,m_BotPixels.data());	
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageHeight , checkImageHeight , 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);	
		*/
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
}


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
	cout<<endl;

	switch(k){
		case 'w':
			//cout<<"up"<<endl;
			thegame.m_Player.move(UP);
			break;
		case 's':
			//cout<<"down"<<endl;
			thegame.m_Player.move(DOWN);
			break;
		case 'd':
			//cout<<"right"<<endl;
			thegame.m_Player.move(RIGHT);
			break;
		case 'a':
			//cout<<"left"<<endl;
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
	//cout<<"timer"<<endl;
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
