#include "dib.H"
#include <GL/glut.h>
#include <GL/GLU.h>
#include "math.h"

void MyInit() {
	MyDib dib;
	dib.ReadBMP("wall.bmp");	
	dib.m_lpImage;//이미지의 시작주소
	glTexImage2D(GL_TEXTURE_2D, 0, 3, dib.GetWidth(), dib.GetHeight(), 0, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, dib.m_lpImage);//택스쳐 만들어주는 함수! 꼭 써야함

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, dib.GetWidth(), dib.GetHeight(), GL_BGR_EXT,
	  GL_UNSIGNED_BYTE, dib.m_lpImage);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // GL_REPEAT
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // GL_REPEAT
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//가장가까운데서가져옴
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);//확장단계에 리니어써서 뿌옇지만(평균내서) 부드럽게
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//멀리있는것에 쓰면 부드럽게나옴 보통 이렇게쓰는게 보통 커질때 리니어 작아질때 민맵리니어
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //텍스쳐에 대한 환경설정 GL_DECAL,GL_REPLACE, GL_MODULATE 원래 노란색이었는데 데칼 리플레이스는 원래거 쓰는거
	//모듈레이트는 색을 섞어줌
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT 가로방향
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT 세로방향 정수부분날라면 반복됨 3.4면 0.4부분

	glColor3f(1.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 0.1, 100);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}
void MyIdle()
{
	static float count = 0;
	count += 0.0008;
	float eye = 30;
	eye = sin(count)*15 + 16;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, eye, 0, 0, 0, 0, 1, 0);
	glutPostRedisplay();

}
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	/*
	// index buffer
	float MyVertices[4][3] = {{-1.0, -1.0, 0.0},
	{-1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, -1.0, 0.0} };
	float MyTexCoords[4][2] = { {0,0}, {0,1}, {1,1}, {1,0} };

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, MyVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, MyVertices);
	GLubyte MyIndex[4] = {0,1,2,3};
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, MyIndex);
	*/
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);//gl 텍트쳐 코드 좌표 0,0(그림에서의좌표)을 이 벌텍스에 적용시키겠다
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glEnd();
	glutSwapBuffers();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("openGL Sample Program");
	MyInit();
	glutDisplayFunc(MyDisplay);
	glutIdleFunc(MyIdle);
	glutMainLoop();
	return 0;
}
