#include "dib.H"
#include <GL/glut.h>
#include <GL/GLU.h>
#include "math.h"

void MyInit() {
	MyDib dib;
	dib.ReadBMP("wall.bmp");	
	dib.m_lpImage;//�̹����� �����ּ�
	glTexImage2D(GL_TEXTURE_2D, 0, 3, dib.GetWidth(), dib.GetHeight(), 0, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, dib.m_lpImage);//�ý��� ������ִ� �Լ�! �� �����

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, dib.GetWidth(), dib.GetHeight(), GL_BGR_EXT,
	  GL_UNSIGNED_BYTE, dib.m_lpImage);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // GL_REPEAT
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // GL_REPEAT
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//���尡����������
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);//Ȯ��ܰ迡 ���Ͼ�Ἥ �ѿ�����(��ճ���) �ε巴��
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//�ָ��ִ°Ϳ� ���� �ε巴�Գ��� ���� �̷��Ծ��°� ���� Ŀ���� ���Ͼ� �۾����� �θʸ��Ͼ�
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //�ؽ��Ŀ� ���� ȯ�漳�� GL_DECAL,GL_REPLACE, GL_MODULATE ���� ������̾��µ� ��Į ���÷��̽��� ������ ���°�
	//��ⷹ��Ʈ�� ���� ������
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT ���ι���
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT ���ι��� �����κг���� �ݺ��� 3.4�� 0.4�κ�

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
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);//gl ��Ʈ�� �ڵ� ��ǥ 0,0(�׸���������ǥ)�� �� ���ؽ��� �����Ű�ڴ�
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
