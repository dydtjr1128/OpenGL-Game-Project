#include "GL/glut.h"
#include "math.h"
#include "Camera.h"
Camera cam;//카메라 하나이니까 전역변수로선언 여러 카메라로보려면 여러개해도됨 
float h = 0.25;
float a = 0.001f;
float t = 0;
float speed = 0.004;
GLUquadricObj* cyl;//실린더 변수
void InitLight() {
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 2, 3.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
void MyIdle()
{	
	t = t + 1.0f;
	if (h <= 0.25) a = +0.001f;
	if (h >= 0.75) a = -0.001f;
	h = h + a;
	glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y)
{
	switch (KeyPressed) {
		/*
		slide (0,0.1,0): 평행이동 y로 0.1이동
		pitch,yaw 회전
		*/
	case 'w':
		cam.pitch(-0.5);
		break;
	case 's':
		cam.pitch(+0.5);
		break;
	case 'a':
		cam.yaw(-0.5);
		break;
	case 'd':
		cam.yaw(+0.5);
		break;
	case 'q':
		cam.roll(-0.5);
		break;
	case 'e':
		cam.roll(+0.5);
		break;

	case 'i':
		cam.slide(0, 0.1, 0);
		break;
	case 'k':
		cam.slide(0, -0.1, 0);
		break;
	case 'j':
		cam.slide(-0.1, 0, 0);
		break;
	case 'l':
		cam.slide(0.1, 0, 0);
		break;

	case '[':
		cam.slide(0, 0, 0.1);
		break;

	case ']':
		cam.slide(0, 0, -0.1);
		break;

	}
	glutPostRedisplay();
}
void MyDisplay() {	
	float mat_diffuse1[4] = { 0.8,0.7,0.6,1 }; // 추가
	float mat_diffuse2[4] = { 0.1,0.9,0.4,1 }; // 추가
	cyl = gluNewQuadric();// 실린더값 넣어줘
	gluQuadricDrawStyle(cyl, GLU_FILL);//선으로그려줘ca
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);//이게 핵심적으로 중요함

	glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(0.0, -0.6, 0.0);
		glScalef(5, 0.3, 5);//x,z방향으로 5배 y방향 0.3배
		glutSolidCube(0.3);
	glPopMatrix();
	

	 // 몸통
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);//이게 핵심적으로 중요함
	glPushMatrix();
		 //glTranslatef(0.0, sin(t*speed)*0.3+0.3, -t*0.0005);
		glTranslatef(0.0, 0.0, 0.0);
		 glRotatef(-90, 1, 0, 0);
		 gluCylinder(cyl, 0.2, 0.1, 0.5, 20, 1);//밑반지름,윗반지름
		 glRotatef(90, 1, 0, 0);

		 //2. 오른팔
		glPushMatrix();
			 glTranslatef(0.15, 0.4, 0);
			 glutSolidSphere(0.05, 20, 20);
			 glRotatef(sin(t*speed)*70+100, 1, 0, 0);//-90~90 + 90   = 0~180
			 gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
			 //팔꿈치 관점
			 glTranslatef(0.0, 0.0, 0.2);
			 glutSolidSphere(0.05, 20, 20);
			 //팔등	 
			 glRotatef(sin(t*speed)*45 + 45, 1, 0, 0);
			 gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);

			 glTranslatef(0.0, 0.0, 0.2);
			 glutSolidSphere(0.07, 20, 20); //
		glPopMatrix();

		//3. 왼쪽팔
		glPushMatrix();
			glTranslatef(-0.15, 0.4, 0);
			glutSolidSphere(0.05, 20, 20);
			glRotatef(sin(-t*speed) * 70 + 110, 1, 0, 0);//-90~90 + 90   = 0~180
			gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);
			//팔꿈치 관점
			glTranslatef(0.0, 0.0, 0.2);
			glutSolidSphere(0.05, 20, 20);
			//팔등	 
			glRotatef(sin(-t*speed) * 45 + 45, 1, 0, 0);
			gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 1);

			glTranslatef(0.0, 0.0, 0.2);
			glutSolidSphere(0.07, 20, 20); //
		glPopMatrix();

		//머리
		glDisable(GL_LIGHTING);
		glPushMatrix();			
			glTranslatef(0.0, 0.575, 0.0);
			glutSolidSphere(0.15, 20, 20);
		glPopMatrix();
		glEnable(GL_LIGHTING);

		//왼쪽 다리
		glPushMatrix();
			glTranslatef(-0.1, 0.0, 0.0);						
			glutSolidSphere(0.07, 20, 20);
			glRotatef(sin(t*speed) * 50 + 90, 1, 0, 0);//90~140
			gluCylinder(cyl, 0.07, 0.07, 0.3,20,1);		
			//무릎관절
			glTranslatef(0.0, 0.0, 0.3);
			glutSolidSphere(0.05, 20, 20);
			//다리
			glRotatef(sin(t*speed) * 45-45, 1, 0, 0);//-90~0
			gluCylinder(cyl, 0.06, 0.06, 0.25, 20, 1);
			//발
			glTranslatef(0.0, -0.05, 0.25);
			glScalef(1, 2.3, 1);
			glutSolidCube(0.1);
		glPopMatrix();
		//왼쪽 다리
		glPushMatrix();
			glTranslatef(0.1, 0.0, 0.0);
			glutSolidSphere(0.07, 20, 20);
			glRotatef(sin(-t*speed) * 50 + 90, 1, 0, 0);//90~140
			gluCylinder(cyl, 0.07, 0.07, 0.3, 20, 1);
			//무릎관절
			glTranslatef(0.0, 0.0, 0.3);
			glutSolidSphere(0.05, 20, 20);
			//다리
			glRotatef(sin(-t*speed) * 45 - 45, 1, 0, 0);//-90~0
			gluCylinder(cyl, 0.06, 0.06, 0.25, 20, 1);
			//발
			glTranslatef(0.0, -0.05, 0.25);
			glScalef(1, 2.3, 1);
			glutSolidCube(0.1);
		glPopMatrix();
	//glPushMatrix();
	//	glColor3f(0.0, 0.0, 1.0);
	//	glTranslatef(0, h, 0);
	//	glutWireSphere(0.2, 20, 20);
	//glPopMatrix();
	glPopMatrix();
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20, 1, 0.1, 1000);
	// gluLookAt
	cam.set(2, 3, 4,
		0, 0, 0,
		0, 1, 0);//카메라 초기화 눈의좌표3 바라보는점3 업벡터(방향)3

	InitLight();
	//조명은 가장위쪽은 눈에달린카메라역할, 카메라밑은 월드좌표계의 조명위치가됨
	//translate등변환아래는 물체에따라다니는 조명이됨

	glutDisplayFunc(MyDisplay);
	glutIdleFunc(MyIdle);
	glutKeyboardFunc(MyKeyboard);
	glutMainLoop();
	return 0;
}