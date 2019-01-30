#include<string>
#include<gl/glut.h>
#include<math.h>
#include<Windows.h>
#include<algorithm>

using namespace std;

struct vertex
{
	int x, y;
	vertex(int dx, int dy)
	{
		x = dx;
		y = dy;
	}
};

vertex p0(100, 250);     //���Ƶ�
vertex p1(400, 300);     //���Ƶ�
vertex c0(150, 280);     //p0���������ϵĵ�
vertex c1(480, 450);     //p1���������ϵĵ�

bool leftDown = false;
bool rightDown = false;

int squareDistance(vertex a, vertex b)
{
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

void Hermite(int n)     
{
	//�������ڿ��Ƶ�����������ߣ�
	//���е�ĳ�������4�������ߵ�Ч����Ϊ���ԣ�ʵ���ϲ����ϼ�����Ĺ�ʽ��
	vertex tempC0((c0.x - p0.x) << 2, (c0.y - p0.y) << 2);
	vertex tempC1((c1.x - p1.x) << 2, (c1.y - p1.y) << 2);

	double delTa = 1.0 / n;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n; i++)
	{
		double t = i * delTa;

		double t1 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
		double t2 = -2 * pow(t, 3) + 3 * pow(t, 2);
		double t3 = pow(t, 3) - 2 * pow(t, 2) + t;
		double t4 = pow(t, 3) - pow(t, 2);

		glVertex2d(p0.x*t1 + p1.x*t2 + tempC0.x*t3 + tempC1.x*t4, p0.y*t1 + p1.y*t2 + tempC0.y*t3 + tempC1.y*t4);
	}
	glEnd();
}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);       //�����GL_COLOR_BUFFER_BIT��ʾ�����ɫ

	glPointSize(10.0f);
	glColor3f(0, 0, 1);
	//�����е�Ϳ��Ƶ㣨��ɫ��
	glBegin(GL_POINTS);
	glVertex2d(p0.x, p0.y);
	glVertex2d(p1.x, p1.y);
	glVertex2d(c0.x, c0.y);
	glVertex2d(c1.x, c1.y);
	glEnd();

	//�����е�����ֵ������ߣ���ɫ��
	glColor3f(1, 0, 0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2d(p0.x, p0.y);    glVertex2d(c0.x, c0.y);
	glVertex2d(p1.x, p1.y);    glVertex2d(c1.x, c1.y);
	glEnd();

	Hermite(200);

	glFlush();
	glutSwapBuffers();
}
void mouse(int button, int state, int x, int y)       //������궯��
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		leftDown = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		leftDown = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rightDown = true;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		rightDown = false;
	}
}
void motion(int x, int y)       //�ƶ���
{
	if (leftDown)        //����ƶ����Ƶ�
	{
		if (squareDistance(vertex(x, y), c0) < 400)    
		{
			c0.x = x;
			c0.y = y;
		}
		else if (squareDistance(vertex(x, y), c1) < 400)
		{
			c1.x = x;
			c1.y = y;
		}
	}
	else if (rightDown)      //�Ҽ��ƶ��е�
	{
		if (squareDistance(vertex(x, y), p0) < 400)
		{
			p0.x = x;
			p0.y = y;
		}
		else if (squareDistance(vertex(x, y), p1) < 400)
		{
			p1.x = x;
			p1.y = y;
		}
	}
	glutPostRedisplay();        //���¹�ͼ
}
void Reshape(int w, int h)      //�������������ڱ��ƶ����С
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initWindow(int &argc, char *argv[], int width, int height, const char* title)    
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - width) >> 1, (GetSystemMetrics(SM_CYSCREEN) - height) >> 1);       //ָ������λ��
	glutInitWindowSize(width, height);       //���ڴ�С
	glutCreateWindow(title);

	glClearColor(1, 1, 1, 0.0);
	glShadeModel(GL_FLAT);
}

int main(int argc, char *argv[])
{
	initWindow(argc, argv, 500, 500, "Hermite");

	puts("\n\tʹ��Hermite�㷨���������������Ƶ�����������ߡ�");
	puts("\t����ƶ����Ƶ�p0��p1���Ҽ��ƶ������ϵĵ�c0��c1");

	glutDisplayFunc(myDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}