#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
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
void InitVisibility() {//Ƽ���� ������ ���������ʰ� �������־� �����̽��ø� ���ϴ°����� �ϸ� ���� �������κ��̱⶧���� �������������� ���ϴ°�����
	//glEnable(GL_CULL_FACE);//�����ڰ� �⺻���� �ð�� �������� �׷���
	glDisable(GL_CULL_FACE);
	//��ü�� CCW�϶� �븻���ʹ� �׻� �Ȱ���, �����οö� CW�ɼ��ָ� �����ο��°��� �޸����λ����� �����οö� CCW�ɼ��ָ� �����ο��°��� �ո����λ�����
	//��ü�� CCW�θ�������� CCW�� �ɼ��ְ� CW�θ���� CW�� �ɼ��ָ�� �Ȱ����ָ� ��
	glFrontFace(GL_CW);//�ð������ ������ = Z�־��������� �ո����� ���ع��� ���� �̰� ��������
	glCullFace(GL_BACK);//�޸� �Ⱥ��̰�;�
	//�յ޸� ������ �׸��� ����
	glPolygonMode(GL_FRONT, GL_FILL);//�ո����׸����� ������ ä�����׸���
	glPolygonMode(GL_BACK, GL_LINE);//�޸��� �׸����� WIRE�� �׷���
	//glCullFace(GL_FRONT);//�ո�������Ȱ� �޸鸸 ��������� = �ո��� ������
	//glEnable(GL_DEPTH_TEST);//GLUT_DEPTH�� ���� ���̹��� Ȱ��ȭ�ϰڴ�
	glDisable(GL_DEPTH_TEST);//�̰� �յڱ��о��� �׸��ڴٴ¶�
}
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//GLUT_DEPTH�� ���� ���̹��� �ʱ�ȭ�ϰڴٴ¶� 2��° ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.4, 0.5, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);
	glutSolidTeapot(0.58);
	glFlush();
}
void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 2.0);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);//GLUT_DEPTH�� ���̹��� ����ڴٴ¶� 1��° ����
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(0.5, 0.5, 0.5, 0.0);
	InitLight();
	InitVisibility();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}