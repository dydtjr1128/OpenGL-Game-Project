#include <Windows.h>
#include <process.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <time.h>
#include "math.h"
#include "Camera.h"
#include "dib.h"
#include <stdio.h>

#include <vector>
using namespace std;
#define FORWARD 0
#define BACK 1
#define RIGHT 2
#define LEFT 3
#define MOVEMENT 30//�̵�����
#define M_PI 3.14159265358979323846/180
#define CAMERA_MOVEMENT 0.01
Camera cam;//ī�޶� �ϳ��̴ϱ� ���������μ��� ���� ī�޶�κ����� �������ص��� 
bool camera = false;//true : 1��Ī false = 3��Ī
bool fullscreen = false;
bool isJumping = false;
float maxZ = 10;
float maxX = 20;
float t = 0;
float t1 = 0.5;//�ι�° �ΰ�
float speed = 0.00001;
float fbposition = 0;//�յ� z��
float lrposition = 0;//�����ʿ��� x��
float jumpPosition = 0;
int direction = 0;//0 : forward, 1:back, 2:right, 3: left
float directionDegree = 0;
float humanposition = 0;
float humanposition2 =90;
float humanpositionshoot = 90;
float mouseX, mouseY;

bool enemy1Alive = true;
int enemy1Count = 0;//���� ���� ���� ī��Ʈ���� ���� �ٲ�
int enemy1Degree = 0;//��1�� ����
float enemy1fbposition = 0;//���� �յ� ���� z��
float enemy1lrposition = 0.5;//���� �¿���� x��
int enemy1direction = 0;

bool enemy2Alive = true;
int enemy2Count = 0;//���� ���� ���� ī��Ʈ���� ���� �ٲ�
int enemy2Degree = 0;//��1�� ����
float enemy2fbposition = 0.9;//���� �յ� ���� z��
float enemy2lrposition = 0;//���� �¿���� x��
int enemy2direction = 0;

float mdegree = 0;
float mmx=0;
float mmy=0;
float bulletspeed = 0.01;
float bulletSize = 0.02;
GLuint textures[5];
GLUquadricObj* cyl;//�Ǹ��� ����
class Bullet{
	float x;
	float z;
	float distance;
	int position;//FORWARD 0 BACK 1 RIGHT 2 LEFT 3
public:
	Bullet(float x, float z, float distance, int position ) {
		this->x = x;
		this->z = z;
		this->distance = distance;
		this->position = position;
	}
	float getX() { return x; }
	float getZ() { return z; }
	void setX(float x) { this->x = x; }
	void setZ(float z) { this->z = z; }
	float getDistance() { return distance; }
	int getPosition() { return position; }
};
vector<Bullet> bullets;
GLfloat MyVertices[8][3] = {//xyz
	{ -0.75, -0.25, 0.5 },
	{ -0.75, 0.25, 0.5 },
	{ -0.5, 0.25, 0.5 },
	{ -0.5, -0.25, 0.5 },
	{ -0.75, -0.25, -0.25 },
	{ -0.75, 0.25, -0.25 },
	{ -0.5, 0.25, -0.25 },
	{ -0.5, -0.25, -0.25 }
};
GLfloat MyColors[8][3] = {
	{ 0.2, 0.2, 0.2 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 }
};
GLubyte MyVertexList[24] = {
	0, 3, 2, 1,
	2, 3, 7, 6,
	0, 4, 7, 3,
	1, 2, 6, 5,
	4, 5, 6, 7,
	0, 1, 5, 4
};
//��� ��ü
GLfloat material_ambient[] = { 0.65, 0.65, 0.5, 1.0 };  //��ü Ư��
GLfloat material_diffuse[] = { 0.3, 0.3,0.1, 1.0 }; //
GLfloat material_diffuseBody[] = { 0.3, 0.5,0.2, 1.0 }; //
GLfloat material_specular[] = { 0.0, 0.0, 0.0, 1.0 }; // �ſ��� ����ŧ������ ����� ����
GLfloat material_shininess[] = { 25.0 };
//�ٴ� ��ü
GLfloat material_ambient1[] = { 0.3, 0.2, 0.1, 1.0 };  //��ü Ư��
GLfloat material_diffuse1[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat material_specular1[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat material_shininess1[] = { 25.0 };
//���� ��ü
GLfloat material_ambient2[] = { 0.2, 0.1, 0.0, 1.0 };  //��ü Ư��
GLfloat material_diffuse2[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat material_specular2[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat material_shininess2[] = { 25.0 };
void MyIdle()
{		
	t1 = t1 + 0.25f;
	glutPostRedisplay();
}
void moving() {
	t = t + 1.0f;
}
void MyMouse2(int x, int y) {
	mmx = x;
	mmy = y;
	if (camera) {//1��Ī
		mouseX = x*0.0013;
		mouseY = y*-0.006;
		cam.set(lrposition, 0.65, fbposition - 0.2,
			lrposition + mouseX - 0.9, 3.5 + mouseY, fbposition - 3.4,
			0, 1, 0);
	}
	else {
		mouseX = x*0.0007;
		mouseY = y*-0.002;
		//printf("%f %f\n", lrposition + mouseX - 0.37, mouseY + 1.5);
		cam.set(lrposition, 1, fbposition + 3.1,
			lrposition + mouseX - 0.6, mouseY + 1.6, fbposition - 0.5,
			0, 1, 0);
	}
	glutPostRedisplay();
}
void changeEnemyLifeThread(void *t) {
	Sleep(5000);
	int num = (int)(t);
	switch (num) {
	case 1://��1 5���� ��Ȱ
		enemy1Alive = true;
		break;
	case 2://��2 5���� ��Ȱ
		enemy2Alive = true;
		break;
	}
}
void shotBollet() {	//������ź ���ư��� �κ�
	
	for (int i = 0; i < bullets.size(); i++) {
		if (abs(bullets.at(i).getX()) > bullets.at(i).getDistance() || abs(bullets.at(i).getZ()) > bullets.at(i).getDistance()) {
			bullets.erase(bullets.begin() + i);
		}
		else{
			glPushMatrix();			
			switch (bullets.at(i).getPosition()) {//������ź�� ���� �����κ� ������������ ��������
			case FORWARD://������ ���� ������
				glColor3f(1.0, 1.0, 1.0);
				glTranslatef(bullets.at(i).getX(), 0.5, bullets.at(i).getZ()-0.2);
				glutSolidSphere(bulletSize,10,10);
				bullets.at(i).setZ(bullets.at(i).getZ() - bulletspeed);
				break;
			case BACK://�ڸ� ���� ������
				glColor3f(1.0, 1.0, 1.0);
				glTranslatef(bullets.at(i).getX(), 0.5, bullets.at(i).getZ()+ 0.2);
				glutSolidSphere(bulletSize, 10, 10);
				bullets.at(i).setZ(bullets.at(i).getZ() + bulletspeed);
				break;
			case LEFT:
				glColor3f(1.0, 1.0, 1.0);
				glTranslatef(bullets.at(i).getX() - 0.2, 0.5, bullets.at(i).getZ());
				glutSolidSphere(bulletSize, 10, 10);
				bullets.at(i).setX(bullets.at(i).getX() - bulletspeed);
				break;
			case RIGHT:
				glColor3f(1.0, 1.0, 1.0);
				glTranslatef(bullets.at(i).getX() + 0.2, 0.5, bullets.at(i).getZ());
				glutSolidSphere(bulletSize, 10, 10);
				bullets.at(i).setX(bullets.at(i).getX() + bulletspeed);
				break;
			}
			if ((bullets.at(i).getX() + 0.2 > enemy1lrposition && bullets.at(i).getX() - 0.2 < enemy1lrposition) && (bullets.at(i).getZ() + 0.2 > enemy1fbposition && bullets.at(i).getZ() - 0.2 < enemy1fbposition)) {
				enemy1Alive = false;
				_beginthread(changeEnemyLifeThread, 0, (void *)1);
			}
			if ((bullets.at(i).getX() + 0.2 > enemy2lrposition && bullets.at(i).getX() - 0.2 < enemy2lrposition) && (bullets.at(i).getZ() + 0.2 > enemy2fbposition && bullets.at(i).getZ() - 0.2 < enemy2fbposition)) {
				enemy2Alive = false;
				_beginthread(changeEnemyLifeThread, 0, (void *)2);
			}
			glPopMatrix();
		}			
	}	
	
}

void MyMouse(int button, int state, int x, int y) {
	
	if (state == GLUT_LEFT_BUTTON) {		
		float x = lrposition;
		float y = fbposition;
		bullets.push_back(Bullet(x, y, 5, direction));				
		humanpositionshoot = 180 ;
	}
	else if (state == 1) {
		humanpositionshoot = 90;
	}

}
void jump_Thread(void* begin) {
	isJumping = true;
	float jump = 0;
	while (jump < 180) {
		jumpPosition = sin(jump*M_PI)*0.3;		
		jump = jump + 1.0f;		
		
		Sleep(10);		
	}
	humanposition = 0;
	humanposition2 = 90;
	humanpositionshoot = 90;
	glutPostRedisplay();	
	jumpPosition = 0;
	isJumping = false;
}
void MyKeyboardUp(unsigned char key, int x, int y) {
	humanposition = 0;
	humanposition2 = 90;
	humanpositionshoot = 90;
	glutPostRedisplay();
	t = 0;
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y)
{
	//printf("%c",KeyPressed);
	humanposition = 1;
	humanposition2 = 0;	
	humanpositionshoot = 0;
	float tempx = lrposition;
	float tempz = fbposition;
	switch (KeyPressed) {
	case 'w':
		direction = FORWARD;
		fbposition -= (MOVEMENT*0.0005);		
		if (camera) {//1��Ī/ī�޶��� ��ġ�� lrpositon(���ʿ����� ��ġ��) �յ���ġ�� ī�޶� �����Ÿ� ����߷� �����ش�.
			cam.set(lrposition, 0.65, fbposition - 0.2,
				lrposition + mouseX - 0.9, 3.5 + mouseY, fbposition - 3.4,
				0, 1, 0);
		}
		else {
			cam.set(lrposition, 1, fbposition + 3.1,
				lrposition + mouseX - 0.6, mouseY + 1.6, fbposition - 0.5,
				0, 1, 0);
		}

		moving();
		break;
	case 's':
		direction = BACK;
		fbposition += (MOVEMENT*0.0005);
		if (camera) {//1��Ī
			cam.set(lrposition, 0.65, fbposition - 0.2,
				lrposition + mouseX - 0.9, 3.5 + mouseY, fbposition - 3.4,
				0, 1, 0);
		}
		else {
			cam.set(lrposition, 1, fbposition + 3.1,
				lrposition + mouseX - 0.6, mouseY + 1.6, fbposition - 0.5,
				0, 1, 0);
		}
		moving();
		break;
	case 'a':
		direction = LEFT;
		lrposition -= (MOVEMENT*0.0005);
		if (camera) {//1��Ī
			cam.set(lrposition, 0.65, fbposition - 0.2,
				lrposition + mouseX - 0.9, 3.5 + mouseY, fbposition - 3.4,
				0, 1, 0);
		}
		else {
			cam.set(lrposition, 1, fbposition + 3.1,
				lrposition + mouseX - 0.6, mouseY + 1.6, fbposition - 0.5,
				0, 1, 0);
		}
		moving();
		break;
	case 'd':
		direction = RIGHT;
		lrposition += (MOVEMENT*0.0005);
		if (camera) {//1��Ī
			cam.set(lrposition, 0.65, fbposition - 0.2,
				lrposition + mouseX - 0.9, 3.5 + mouseY, fbposition - 3.4,
				0, 1, 0);
		}
		else {
			cam.set(lrposition, 1, fbposition + 3.1,
				lrposition + mouseX - 0.6, mouseY + 1.6, fbposition - 0.5,
				0, 1, 0);
		}
		moving();
		break;
	case 'q':
		cam.roll(-0.5);
		break;
	case 'e':
		cam.roll(+0.5);
		break;
	case 'c':
		if (camera) {//1��Ī�϶� 3��Ī���� ��ȯ		
			cam.set(lrposition, 1, fbposition + 3.1,
				lrposition + mouseX - 0.6, mouseY + 1.6, fbposition - 0.5,
				0, 1, 0);
			camera = false;
		}
		else {//���� 3��Ī�ΰ�� 1��Ī���� ��ȯ
			cam.set(lrposition, 0.65, fbposition - 0.2,
				lrposition + mouseX - 0.9, 3.5 + mouseY, fbposition - 3.4,
				0, 1, 0);
			camera = true;
		}		
		break;
	case 27://ESCŰ�� ��������
		if (fullscreen) {// ��üȭ���̸�
			glutReshapeWindow(1520, 780);
			glutPositionWindow(0, 30);			
			fullscreen = false;
		}
		else {//��üȭ���� �ƴϸ�
			glutFullScreen();
			fullscreen = true;
		}
		break;	
	case ' ':
		if(isJumping == false)
			_beginthread(jump_Thread, 0, 0);
		break;
	}	
	//printf("%lf %lf %\n",fbposition, lrposition);
	if ((fbposition > -1.6 && fbposition < -1.4) && (lrposition > 0.30 && lrposition < 0.5)) {
		fbposition = tempz;
		lrposition = tempx;
	}
	else if ((fbposition > -1.6 && fbposition < -1.4) && (lrposition > -0.5 && lrposition < -0.3)) {
		fbposition = tempz;
		lrposition = tempx;
	}
	else if ((fbposition > -3.1 && fbposition < -2.9) && (lrposition > 0.3 && lrposition < 0.5)) {
		fbposition = tempz;
		lrposition = tempx;
	}
	else if ((fbposition > -3.2 && fbposition < -2.8) && (lrposition > -0.6 && lrposition < -0.2)) {
		fbposition = tempz;
		lrposition = tempx;
	}
	
	if (fbposition > maxZ-5) {
		fbposition = maxZ - 5;
	}
	else if (fbposition < -(maxZ - 5)) {
		fbposition = -(maxZ - 5);
	}
	if (lrposition > maxX-10) {
		lrposition = maxX-10;
	}
	else if (lrposition < -(maxX-10)) {
		lrposition = -(maxX - 10);
	}
	glutPostRedisplay();
}
void MyLightInit() {

	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };     //���� �ֺ��ݻ�

	GLfloat light0_position[] = { -5.0, 5.0, 2.0, 0.0 };
	GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };     //0�� ���� Ư�� r,g,b,a
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glShadeModel(GL_SMOOTH);    //���� ���̵�
	glEnable(GL_DEPTH_TEST);    //���� ���� Ȱ��ȭ
	glEnable(GL_LIGHTING);      //���� Ȱ��ȭ
	glEnable(GL_NORMALIZE);	//scale ���� ����ȿ�� �̻��ϰԳ��� �̰ž��� �������� ����	
	glEnable(GL_LIGHT0);        //0�� ���� Ȱ��ȭ
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);   //0�� ���� Ư���Ҵ�
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); //�����ֺ��ݻ�
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    //��������
}
void MyInit() {
	MyDib dib;
	//dib = &textures[index];
	
	dib.ReadBMP("sky.bmp");
	dib.m_lpImage;//�̹����� �����ּ�
	glTexImage2D(GL_TEXTURE_2D, 0, 3, dib.GetWidth(), dib.GetHeight(), 0, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, dib.m_lpImage);//�ý��� ������ִ� �Լ�! �� �����

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, dib.GetWidth(), dib.GetHeight(), GL_BGR_EXT,
		GL_UNSIGNED_BYTE, dib.m_lpImage);
		
																 //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//���尡����������
																 //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Ȯ��ܰ迡 ���Ͼ�Ἥ �ѿ�����(��ճ���) �ε巴��
																	 //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
																	 //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//�ָ��ִ°Ϳ� ���� �ε巴�Գ��� ���� �̷��Ծ��°� ���� Ŀ���� ���Ͼ� �۾����� �θʸ��Ͼ�

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //�ؽ��Ŀ� ���� ȯ�漳�� GL_DECAL,GL_REPLACE, GL_MODULATE ���� ������̾��µ� ��Į ���÷��̽��� ������ ���°�
															  //��ⷹ��Ʈ�� ���� ������

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT ���ι���
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT ���ι��� �����κг���� �ݺ��� 3.4�� 0.4�κ�

}
void enemy1() {	
	if(enemy1Alive){
	glPushMatrix();
	//glTranslatef(0.0, sin(t*speed)*0.3+0.3, -t*0.0005);
	
	if (enemy1Count == 1000) {
		enemy1direction = rand() % 4;
		switch (enemy1direction) {//���� �����κ� ���κ��� ����������� �׷ȱ� ������ ����ȸ���ϸ� �ٰ���ȸ��
		case FORWARD:
			enemy1Degree = 0;			
			break;
		case BACK:
			enemy1Degree = 180;
			break;
		case LEFT:
			enemy1Degree = 90;
			break;
		case RIGHT:
			enemy1Degree = 270;
			break;
		}
		enemy1Count = 0;
	}
	enemy1Count++;
	switch (enemy1direction) {//���� �����κ� ���κ��� ����������� �׷ȱ� ������ ����ȸ���ϸ� �ٰ���ȸ��
	case FORWARD://��������̸�
		enemy1fbposition -= (MOVEMENT*0.00004);//z������ �̸�ŭ �̵���Ų��.
		break;
	case BACK:
		enemy1fbposition += (MOVEMENT*0.00004);
		break;
	case LEFT:
		enemy1lrposition -= (MOVEMENT*0.00004);
		break;
	case RIGHT:
		enemy1lrposition += (MOVEMENT*0.00004);
		break;
	}
	if (enemy1fbposition > maxZ-5) {
		enemy1fbposition = maxZ - 5;
	}
	else if(enemy1fbposition < -(maxZ - 5)) {
		enemy1fbposition = -(maxZ - 5);
	}
	if (enemy1lrposition > maxX-10) {
		enemy1lrposition = maxX-10;
	}
	else if (enemy1lrposition < -(maxX-10)) {
		enemy1lrposition = -(maxX-10);
	}
	glTranslatef(enemy1lrposition, 0.4, enemy1fbposition);
	
	glRotatef(enemy1Degree, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);//��ü������ ������ ���̱�
	
	glRotatef(-90, 1, 0, 0);
	gluCylinder(cyl, 0.2, 0.1, 0.5, 20, 1);//�ع�����,��������
	glRotatef(90, 1, 0, 0);

	//2. ������
	glPushMatrix();
	glTranslatef(0.15, 0.4, 0);
	glutSolidSphere(0.05, 20, 20);
	glRotatef(sin(t1*speed) * 70 + 100, 1, 0, 0);//-90~90 + 90   = 0~180
	gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
	//�Ȳ�ġ ����
	glTranslatef(0.0, 0.0, 0.2);
	glutSolidSphere(0.05, 20, 20);
	//�ȵ�	 
	glRotatef(sin(t1*speed) * 45 + 45, 1, 0, 0);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);

	glTranslatef(0.0, 0.0, 0.2);
	glutSolidSphere(0.07, 20, 20); //
	glPopMatrix();

	//3. ������
	glPushMatrix();
	glTranslatef(-0.15, 0.4, 0);
	glutSolidSphere(0.05, 20, 20);
	glRotatef(sin(-t1*speed) * 70 + 110, 1, 0, 0);//-90~90 + 90   = 0~180
	gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
	//�Ȳ�ġ ����
	glTranslatef(0.0, 0.0, 0.2);
	glutSolidSphere(0.05, 20, 20);
	//�ȵ�	 
	glRotatef(sin(-t1*speed) * 45 + 45, 1, 0, 0);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);

	glTranslatef(0.0, 0.0, 0.2);
	glutSolidSphere(0.07, 20, 20); //
	glPopMatrix();

	//�Ӹ�
	
	glPushMatrix();
	glTranslatef(0.0, 0.575, 0.0);
	glutSolidSphere(0.15, 20, 20);
	glPopMatrix();
	

	//���� �ٸ�
	glPushMatrix();
	glTranslatef(-0.1, 0.0, 0.0);
	glutSolidSphere(0.07, 20, 20);
	glRotatef(sin(t1*speed) * 50 + 90, 1, 0, 0);//90~140
	gluCylinder(cyl, 0.07, 0.07, 0.3, 20, 1);
	//��������
	glTranslatef(0.0, 0.0, 0.3);
	glutSolidSphere(0.05, 20, 20);
	//�ٸ�
	glRotatef(sin(t1*speed) * 45 - 45, 1, 0, 0);//-90~0
	gluCylinder(cyl, 0.06, 0.06, 0.25, 20, 1);
	//��
	glTranslatef(0.0, -0.05, 0.25);
	glScalef(1, 2.3, 1);
	glutSolidCube(0.1);
	glPopMatrix();
	//���� �ٸ�
	glPushMatrix();
	glTranslatef(0.1, 0.0, 0.0);
	glutSolidSphere(0.07, 20, 20);
	glRotatef(sin(-t1*speed) * 50 + 90, 1, 0, 0);//90~140
	gluCylinder(cyl, 0.07, 0.07, 0.3, 20, 1);
	//��������
	glTranslatef(0.0, 0.0, 0.3);
	glutSolidSphere(0.05, 20, 20);
	//�ٸ�
	glRotatef(sin(-t1*speed) * 45 - 45, 1, 0, 0);//-90~0
	gluCylinder(cyl, 0.06, 0.06, 0.25, 20, 1);
	//��
	glTranslatef(0.0, -0.05, 0.25);
	glScalef(1, 2.3, 1);
	glutSolidCube(0.1);
	glPopMatrix();

	glPopMatrix();
	}
}
void enemy2() {
	if (enemy2Alive) {//enemy2�� ����ִٸ�
		glPushMatrix();
		if (enemy2Count == 1500) {//count�� 1500�ɶ����� ������ �ٲٰڴ�.
			enemy2direction = rand() % 4;//4������ ����
			switch (enemy2direction) {
			case FORWARD:
				enemy2Degree = 0;
				break;
			case BACK:
				enemy2Degree = 180;
				break;
			case LEFT:
				enemy2Degree = 90;
				break;
			case RIGHT:
				enemy2Degree = 270;
				break;
			}
			enemy2Count = 0;
		}
		enemy2Count++;
		switch (enemy2direction) {//���� �����κ� ���κ��� ����������� �׷ȱ� ������ ����ȸ���ϸ� �ٰ���ȸ��
		case FORWARD:
			enemy2fbposition -= (MOVEMENT*0.00004);
			break;
		case BACK:
			enemy2fbposition += (MOVEMENT*0.00004);
			break;
		case LEFT:
			enemy2lrposition -= (MOVEMENT*0.00004);
			break;
		case RIGHT:
			enemy2lrposition += (MOVEMENT*0.00004);
			break;
		}
		if (enemy2fbposition > maxZ - 5) {
			enemy2fbposition = maxZ - 5;
		}
		else if (enemy2fbposition < -(maxZ - 5)) {
			enemy2fbposition = -(maxZ - 5);
		}
		if (enemy2lrposition > maxX - 10) {
			enemy2lrposition = maxX - 10;
		}
		else if (enemy2lrposition < -(maxX - 10)) {
			enemy2lrposition = -(maxX - 10);
		}
		glTranslatef(enemy2lrposition, 0.4, enemy2fbposition);

		glRotatef(enemy2Degree, 0, 1, 0);
		glScalef(0.5, 0.5, 0.5);//��ü������ ������ ���̱�

		glRotatef(-90, 1, 0, 0);
		gluCylinder(cyl, 0.2, 0.1, 0.5, 20, 1);//�ع�����,��������
		glRotatef(90, 1, 0, 0);

		//2. ������
		glPushMatrix();
		glTranslatef(0.15, 0.4, 0);
		glutSolidSphere(0.05, 20, 20);
		glRotatef(sin(t1*speed) * 70 + 100, 1, 0, 0);//-90~90 + 90   = 0~180
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
		//�Ȳ�ġ ����
		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.05, 20, 20);
		//�ȵ�	 
		glRotatef(sin(t1*speed) * 45 + 45, 1, 0, 0);
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);

		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.07, 20, 20); //
		glPopMatrix();

		//3. ������
		glPushMatrix();
		glTranslatef(-0.15, 0.4, 0);
		glutSolidSphere(0.05, 20, 20);
		glRotatef(sin(-t1*speed) * 70 + 110, 1, 0, 0);//-90~90 + 90   = 0~180
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
		//�Ȳ�ġ ����
		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.05, 20, 20);
		//�ȵ�	 
		glRotatef(sin(-t1*speed) * 45 + 45, 1, 0, 0);
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);

		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.07, 20, 20); //
		glPopMatrix();

		//�Ӹ�

		glPushMatrix();
		glTranslatef(0.0, 0.575, 0.0);
		glutSolidSphere(0.15, 20, 20);
		glPopMatrix();


		//���� �ٸ�
		glPushMatrix();
		glTranslatef(-0.1, 0.0, 0.0);
		glutSolidSphere(0.07, 20, 20);
		glRotatef(sin(t1*speed) * 50 + 90, 1, 0, 0);//90~140
		gluCylinder(cyl, 0.07, 0.07, 0.3, 20, 1);
		//��������
		glTranslatef(0.0, 0.0, 0.3);
		glutSolidSphere(0.05, 20, 20);
		//�ٸ�
		glRotatef(sin(t1*speed) * 45 - 45, 1, 0, 0);//-90~0
		gluCylinder(cyl, 0.06, 0.06, 0.25, 20, 1);
		//��
		glTranslatef(0.0, -0.05, 0.25);
		glScalef(1, 2.3, 1);
		glutSolidCube(0.1);
		glPopMatrix();
		//���� �ٸ�
		glPushMatrix();
		glTranslatef(0.1, 0.0, 0.0);
		glutSolidSphere(0.07, 20, 20);
		glRotatef(sin(-t1*speed) * 50 + 90, 1, 0, 0);//90~140
		gluCylinder(cyl, 0.07, 0.07, 0.3, 20, 1);
		//��������
		glTranslatef(0.0, 0.0, 0.3);
		glutSolidSphere(0.05, 20, 20);
		//�ٸ�
		glRotatef(sin(-t1*speed) * 45 - 45, 1, 0, 0);//-90~0
		gluCylinder(cyl, 0.06, 0.06, 0.25, 20, 1);
		//��
		glTranslatef(0.0, -0.05, 0.25);
		glScalef(1, 2.3, 1);
		glutSolidCube(0.1);
		glPopMatrix();

		glPopMatrix();
	}
}
void MyDisplay() {

	cyl = gluNewQuadric();// �Ǹ����� �־���
	gluQuadricDrawStyle(cyl, GLU_FILL);//�����α׷���ca
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);*/
	//����
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse1);   //��ü Ư���Ҵ� ���� �Ӽ� �Ҵ�  ����ȸ���� rgb���� ��ٴ¶�
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular1);	//front�� �ո� back�� ��..SPECULAR �Ķ����� �ݻ��ϰ� �س���
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient1);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess1);
	
	glPushMatrix();		
		glColor3f(0.7, 0.7, 0.7);
		glScalef(maxX, 0.3, maxZ);//x,z�������� 5�� y���� 0.3��
		glTranslatef(0.0, -0.5, 0.0);
		glCullFace(GL_CCW);	
		glutSolidCube(1);
	glPopMatrix();
	
	// ��� ���ۺκ�
	glPushMatrix();
	
	switch (direction) {
		//���� �����κ� ���κ��� ����������� �׷ȱ� ������ ����ȸ���ϸ� �ٰ���ȸ��		
	case FORWARD://������ ������ directionDegree�� 0���� ��´�.
			directionDegree = 0;
			break;
		case BACK:
			directionDegree = 180;
			break;
		case LEFT://���ʺ����� 90�� ȸ��
			directionDegree = 90;
			break;
		case RIGHT:
			directionDegree = 270;
			break;	
	}	
	
	//directionDegree = -mmx * 80 / 1920+10 ;/*360 - atanf((540 - mmy) / (mmx - 960))*180/3;*/
	//lrposition = atanf(directionDegree);
	//fbposition = atanf(directionDegree) * atanf(directionDegree);
	//printf("%lf %lf\n",mmx, directionDegree);
	//printf("%lf %lf\n", lrposition, fbposition);
	//����
	glTranslatef(lrposition, 0.3 + jumpPosition, fbposition);
	glRotatef(directionDegree, 0, 1, 0);	
	//printf("���� %f %f\n", lrposition, fbposition);
	
	
	glRotatef(-90, 1, 0, 0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.5, 0.5, 0.5);//��ü������ ������ ���̱�
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular1);	//front�� �ո� back�� ��..SPECULAR �Ķ����� �ݻ��ϰ� �س���
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient1);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuseBody);	
	gluCylinder(cyl, 0.2, 0.1, 0.5, 20, 1);//�ع�����,��������
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);   //��ü Ư���Ҵ� ���� �Ӽ� �Ҵ�  ����ȸ���� rgb���� ��ٴ¶�
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);	//front�� �ո� back�� ��..SPECULAR �Ķ����� �ݻ��ϰ� �س���
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	//2. ������
	glPushMatrix();
		glTranslatef(0.15, 0.4, 0);
		glutSolidSphere(0.05, 20, 20);
		glRotatef((sin(t*speed) * 70 + 100)*humanposition +humanpositionshoot, 1, 0, 0);//-90~90 + 90   = 0~180		
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
		//�Ȳ�ġ ����
		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.05, 20, 20);
		//�ȵ�	 
		glRotatef((sin(t*speed) * 45 + 45)*humanposition, 1, 0, 0);
		
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.07, 20, 20); //
	glPopMatrix();

	//3. ������
	glPushMatrix();
		glTranslatef(-0.15, 0.4, 0);
		glutSolidSphere(0.05, 20, 20);
		glRotatef((sin(-t*speed) * 70 + 110)*humanposition + humanpositionshoot, 1, 0, 0);//-90~90 + 90   = 0~180
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
		//�Ȳ�ġ ����
		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.05, 20, 20);
		//�ȵ�	 
		glRotatef((sin(-t*speed) * 45 + 45)*humanposition, 1, 0, 0);
		gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.07, 20, 20); //
	glPopMatrix();
	//�Ӹ�
	glPushMatrix();
		glTranslatef(0.0, 0.575, 0.0);
		glutSolidSphere(0.15, 20, 20);
	glPopMatrix();
	//���� �ٸ�
	glPushMatrix();
		glTranslatef(-0.1, 0.0, 0.0);
		glutSolidSphere(0.07, 20, 20);
		glRotatef((sin(t*speed) * 50 + 90)*humanposition + humanposition2, 1, 0, 0);//90~140
		gluCylinder(cyl, 0.07, 0.07, 0.3, 20, 1);
		//��������
		glTranslatef(0.0, 0.0, 0.3);
		glutSolidSphere(0.07, 20, 20);
		//�ٸ�
		glRotatef((sin(t*speed) * 45 - 45)*humanposition, 1, 0, 0);//-90~0
		gluCylinder(cyl, 0.07, 0.06, 0.25, 20, 1);
		//��
		glTranslatef(0.0, -0.05, 0.25);
		glScalef(1, 2.3, 1);
		glutSolidCube(0.1);
	glPopMatrix();
	//������ �ٸ�
	glPushMatrix();
		glTranslatef(0.1, 0.0, 0.0);
		glutSolidSphere(0.07, 20, 20);
		glRotatef((sin(-t*speed) * 50 + 90)*humanposition + humanposition2, 1, 0, 0);//90~140
		gluCylinder(cyl, 0.07, 0.07, 0.3, 20, 1);
		//��������
		glTranslatef(0.0, 0.0, 0.3);
		glutSolidSphere(0.07, 20, 20);
		//�ٸ�
		glRotatef((sin(-t*speed) * 45 - 45)*humanposition, 1, 0, 0);//-90~0
		gluCylinder(cyl, 0.07, 0.06, 0.25, 20, 1);
		//��
		glTranslatef(0.0, -0.05, 0.25);
		glScalef(1, 2.3, 1);
		glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();
	//��� ��

	//����
	glPushMatrix();
		enemy1();
	glPopMatrix();
	glPushMatrix();
		enemy2();
	glPopMatrix();

	//���� �����
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse2);   //��ü Ư���Ҵ� ���� �Ӽ� �Ҵ�  ����ȸ���� rgb���� ��ٴ¶�
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular2);	//front�� �ո� back�� ��..SPECULAR �Ķ����� �ݻ��ϰ� �س���
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient2);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess2);
	glPushMatrix();	
		glScalef(1.0,2.0,1.0);
		glTranslatef(0.4, 0.7, -1.5);
		glRotatef(90, 1, 0, 0);
		gluCylinder(cyl, 0.1, 0.1, 1, 30, 1);
		glRotatef(180, 1, 0, 0);
		glutSolidCone(0.35, 0.6, 30, 1);
	glPopMatrix();	
	glPushMatrix();
		glScalef(1.0, 2.0, 1.0);
		glTranslatef(-0.4, 0.7, -1.5);
		glRotatef(90, 1, 0, 0);
		gluCylinder(cyl, 0.1, 0.1, 1, 30, 1);
		glRotatef(180, 1, 0, 0);
		glutSolidCone(0.35, 0.6, 30, 1);
	glPopMatrix();
	glPushMatrix();
		glScalef(1.3, 1.6, 1.0);
		glTranslatef(-0.4, 0.7, -3);
		glRotatef(90, 1, 0, 0);//-90~90 + 90   = 0~180
		gluCylinder(cyl, 0.1, 0.1, 1, 30, 1);
		glRotatef(180, 1, 0, 0);//-90~90 + 90   = 0~180
		glutSolidCone(0.35, 0.6, 30, 1);
	glPopMatrix();
	glPushMatrix();
		glScalef(1.3, 1.6, 1.0);
		glTranslatef(0.4, 0.7, -3);
		glRotatef(90, 1, 0, 0);//-90~90 + 90   = 0~180
		gluCylinder(cyl, 0.1, 0.1, 1, 30, 1);
		glRotatef(180, 1, 0, 0);//-90~90 + 90   = 0~180
		glutSolidCone(0.35, 0.6, 30, 1);
	glPopMatrix();

	//����
	glPushMatrix();
		glScalef(1.5, 2, 2.0);
		glTranslatef(1.5, 0.5, -0.5);
		glRotatef(90, 1, 0, 0);//-90~90 + 90   = 0~180
		glutSolidCube(1);
		glRotatef(180, 1, 0, 0);//-90~90 + 90   = 0~180
		glutSolidCone(0.35, 0.6, 30, 1);
	glPopMatrix();

	//���
	glPushMatrix();	
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-20.0, -10.0, -40.0);//gl ��Ʈ�� �ڵ� ��ǥ 0,0(�׸���������ǥ)�� �� ���ؽ��� �����Ű�ڴ�
		glTexCoord2f(0.0, 1.0); glVertex3f(-20.0, 15.0, -40.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(20.0, 15.0, -40.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(20.0, -10.0, -40.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);	
	glPopMatrix();


	glPushMatrix();
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	glColorPointer(3, GL_FLOAT, 0, MyColors);
	glVertexPointer(3, GL_FLOAT, 0, MyVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, MyVertices);\
	for (int i = 0; i < 6; i++) {
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &MyVertexList[4 * i]);
	}
	glDisable(GL_CULL_FACE);
	glPopMatrix();
	glPushMatrix();
		shotBollet();
	glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	t = 0;
	speed = 0.04;
	printf("\t\t��������������������������������������������������������������������\n");
	printf("\t\t��                      ���۹� ����-by �ֿ뼮                     ��\n");
	printf("\t\t��         �����¿� �̵�  : wasd    1��Ī,3��Ī ������ȯ : c      ��\n");
	printf("\t\t��                          ����  : Space                         ��\n");
	printf("\t\t��     ī�޶�� �ֺ� ���캸�� : ���콺 �̵�(1,3��Ī �Ѵ�)         ��\n");
	printf("\t\t��                    ������ �߻� : ���콺 Ŭ��                   ��\n");
	printf("\t\t��                ��üȭ���� <-> â��� ��ȯ : esc              ��\n");
	printf("\t\t��������������������������������������������������������������������\n");
	printf("�������� ���� ���� ������ϴ�. ����� �� 5�� �� �� ��𼱰� �ٽ� ��Ÿ���� �˴ϴ�.\n");
	
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
		glutInitWindowSize(1920, 1080);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("Moving Game-ESC�� �̿��Ͽ� â��带 ������ �� �ֽ��ϴ�.");
		glClearColor(0.9, 0.9, 0.97, 1.0);	
		srand((unsigned)time(NULL));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		MyLightInit();
		MyInit();
		gluPerspective(20, 1, 0.1, 1000);
		// gluLookAt
		cam.set(lrposition, 0.35, fbposition - 0.2,
			lrposition + mouseX*0.00007 - 0.0532, 0.35 + 0.2 + mouseY*-0.0005, fbposition - 0.4,
			0, 1, 0);	//ī�޶� �ʱ�ȭ ������ǥ3 �ٶ󺸴���3 ������(����)3
	
		glutDisplayFunc(MyDisplay);
		glutIdleFunc(MyIdle);
		glutKeyboardFunc(MyKeyboard);
	
		glutMouseFunc(MyMouse);
		glutPassiveMotionFunc(MyMouse2);
		glutKeyboardUpFunc(MyKeyboardUp);
		glutMainLoop();
		return 0;
	
}