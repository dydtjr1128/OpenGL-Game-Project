#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
float angle = 0;
void MyIdle()
{
	angle += 0.1f;
	glutPostRedisplay();
}
void MyLightInit() {
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };     //���� �ֺ��ݻ�

	GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };     //0�� ���� Ư�� r,g,b,a
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light1_ambient[] = { 0.0, 0.0, 0.0, 1.0 };     //1�� ���� Ư�� ��������Ʈ
	GLfloat light1_diffuse[] = { 0.5, 0.2, 0.3, 1.0 };
	GLfloat light1_specular[] = { 0.0, 0.0, 0.0, 1.0 };

	GLfloat material_ambient[] = { 0.2, 0.0, 0.0, 1.0 };  //��ü Ư��
	GLfloat material_diffuse[] = { 0.8, 0.0, 0.0, 1.0 }; //������ �ݻ��ϰڴ�
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // �ſ��� ����ŧ������ ����� ����
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_PHONG_WIN);    //���� ���̵�
	glEnable(GL_DEPTH_TEST);    //���� ���� Ȱ��ȭ
	glEnable(GL_LIGHTING);      //���� Ȱ��ȭ

	glEnable(GL_LIGHT0);        //0�� ���� Ȱ��ȭ
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);   //0�� ���� Ư���Ҵ�
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT1);        //1�� ���� Ȱ��ȭ
	glDisable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);   //1�� ���� Ư���Ҵ�
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);   //��ü Ư���Ҵ� ���� �Ӽ� �Ҵ�  ����ȸ���� rgb���� ��ٴ¶�
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);	//float�� ����� back�� ���̾�..SPECULAR �Ķ����� �ݻ��ϰ� �س���
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);	
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); //�����ֺ��ݻ�
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    //��������
}

void MyDisplay() {
	GLfloat LightPosition0[] = { 0.0, 0.0, 2.0, 0.0 };   	//0�� ������ġ ��ü�������� 0,0,2.0��ġ�� ���� �޾Ƴ��� 
	//�������� 1�̸� 0,0,2�� ���� �ִٴ� ���̰� 0�̸� ���⼺���� �� 0,0,2�������� �����ִ� ������ �ȴٴ� ��
	GLfloat LightPosition1[] = { 1.0, 1.0, 1.0, 1.0 };     //1�� ������ġ
	GLfloat LightDirection1[] = { -0.5, -1.0, -1.0 };       //1�� ���� ����
	GLfloat SpotAngle1[] = { 20.0 };                        //1�� ���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);                             //�𵨺� ���
	glLoadIdentity();                                       //�׵���� �ε�
	

	gluLookAt(.5, .5, .5, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);    //������ȯ �迧 ���� ���������� ������ ����ٴ� ���Ǳ������� �迣 �Ʒ��� ������ ���ε�ó�� ���� ��ȯ�Ʒ��� ��ȯ�����ϸ鼭 ����ٴ�
															 //���� �̵� ȸ���� �������
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);      //��ġ�� ����
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);      //����Ʈ����Ʈ
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LightDirection1);   //����
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, SpotAngle1);       //����
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);             //�ڻ��� �¼�
	glTranslatef(0.3, 0.3, 0.0);                            //�𵨺�ȯ
	glRotatef(angle, 0, 1, 0);

	glutSolidTorus(0.3, 0.6, 100, 100);                     //��ȯü ����
	glutSwapBuffers();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);                //�������
	glLoadIdentity();                           //�׵���� �ε�
	glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);   //��������
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("openGL Sample Drawing");
	MyLightInit();
	glutIdleFunc(MyIdle);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}
