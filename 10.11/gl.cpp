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
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };     //전역 주변반사

	GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };     //0번 광원 특성 r,g,b,a
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light1_ambient[] = { 0.0, 0.0, 0.0, 1.0 };     //1번 광원 특성 스폿라이트
	GLfloat light1_diffuse[] = { 0.5, 0.2, 0.3, 1.0 };
	GLfloat light1_specular[] = { 0.0, 0.0, 0.0, 1.0 };

	GLfloat material_ambient[] = { 0.2, 0.0, 0.0, 1.0 };  //물체 특성
	GLfloat material_diffuse[] = { 0.8, 0.0, 0.0, 1.0 }; //빨강만 반사하겠다
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // 거울의 스펙큘러색은 흰색이 좋음
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_PHONG_WIN);    //구로 셰이딩
	glEnable(GL_DEPTH_TEST);    //깊이 버퍼 활성화
	glEnable(GL_LIGHTING);      //조명 활성화

	glEnable(GL_LIGHT0);        //0번 광원 활성화
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);   //0번 광원 특성할당
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT1);        //1번 광원 활성화
	glDisable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);   //1번 광원 특성할당
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);   //물체 특성할당 재질 속성 할당  밝은회색에 rgb조명 쏜다는뜻
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);	//float는 제대로 back은 와이어..SPECULAR 파란색만 반사하게 해놓음
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);	
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); //전역주변반사
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    //근접시점
}

void MyDisplay() {
	GLfloat LightPosition0[] = { 0.0, 0.0, 2.0, 0.0 };   	//0번 광원위치 전체공간에서 0,0,2.0위치에 조명 달아놓음 
	//마지막건 1이면 0,0,2에 점이 있다는 끝이고 0이면 방향성광원 즉 0,0,2방향으로 내리쫴는 광원이 된다는 뜻
	GLfloat LightPosition1[] = { 1.0, 1.0, 1.0, 1.0 };     //1번 광원위치
	GLfloat LightDirection1[] = { -0.5, -1.0, -1.0 };       //1번 광원 방향
	GLfloat SpotAngle1[] = { 20.0 };                        //1번 광원 각도
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);                             //모델뷰 행렬
	glLoadIdentity();                                       //항등행렬 로드
	

	gluLookAt(.5, .5, .5, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);    //시점변환 룩엣 위에 조명있으면 조명이 따라다님 눈의기준으로 룩엔 아래에 있으면 가로등처럼 고정 변환아래면 변환까지하면서 따라다님
															 //조명도 이동 회전에 영향받음
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);      //위치성 광원
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);      //스포트라이트
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LightDirection1);   //방향
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, SpotAngle1);       //각도
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);             //코사인 승수
	glTranslatef(0.3, 0.3, 0.0);                            //모델변환
	glRotatef(angle, 0, 1, 0);

	glutSolidTorus(0.3, 0.6, 100, 100);                     //원환체 정의
	glutSwapBuffers();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);                //투상행렬
	glLoadIdentity();                           //항등행렬 로드
	glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);   //평행투상
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
