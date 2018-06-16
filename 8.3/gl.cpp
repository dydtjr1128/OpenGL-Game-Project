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
void InitVisibility() {//티폿은 도형이 닫혀있지않고 공간이있어 백페이스컬링 안하는게좋음 하면 속이 투명으로보이기때문에 느려기진하지만 안하는게좋음
	//glEnable(GL_CULL_FACE);//주전자가 기본으로 시계로 만들어놔서 그렇다
	glDisable(GL_CULL_FACE);
	//물체가 CCW일때 노말백터는 항상 똑같고, 눈으로올때 CW옵션주면 앞으로오는것을 뒷면으로생각함 눈으로올때 CCW옵션주면 앞으로오는것을 앞면으로생각함
	//물체가 CCW로만들었으면 CCW로 옵션주고 CW로만들면 CW로 옵션주면됨 똑같이주면 됨
	glFrontFace(GL_CW);//시계방향을 앞으로 = Z멀어지는쪽을 앞면으로 정해버림 원래 이건 좋지않음
	glCullFace(GL_BACK);//뒷면 안보이고싶어
	//앞뒷면 설정해 그릴수 있음
	glPolygonMode(GL_FRONT, GL_FILL);//앞면을그릴때는 도형을 채워서그리고
	glPolygonMode(GL_BACK, GL_LINE);//뒷면을 그릴때는 WIRE로 그려줘
	//glCullFace(GL_FRONT);//앞면은보기싫고 뒷면만 보고싶을때 = 앞면을 지워라
	//glEnable(GL_DEPTH_TEST);//GLUT_DEPTH로 만든 깊이버퍼 활성화하겠다
	glDisable(GL_DEPTH_TEST);//이건 앞뒤구분없이 그리겠다는뜻
}
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//GLUT_DEPTH로 만든 깊이버퍼 초기화하겠다는뜻 2번째 설정
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);//GLUT_DEPTH가 깊이버퍼 만들겠다는뜻 1번째 설정
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