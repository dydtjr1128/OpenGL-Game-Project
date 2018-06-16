#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.3, 0.3, 0.7);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(-1, 0, 0);
	glEnd();
	glFlush();
}
void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(30, (GLdouble)w / (GLdouble)h, 1.0, 50.0);
	glOrtho(-1, 1,
		-1, 1,
		1, 5.1);//5이상은 보임 룩엣이 5만큼떨어져서 보는건데 1~4까지만 보는걸로 설정해놨기때문에
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glutPostRedisplay();
}
int main(int argc, char** argv) {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}