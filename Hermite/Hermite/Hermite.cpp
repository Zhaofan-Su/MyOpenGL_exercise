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

vertex p0(100, 250);     //控制点
vertex p1(400, 300);     //控制点
vertex c0(150, 280);     //p0处的切线上的点
vertex c1(480, 450);     //p1处的切线上的点

bool leftDown = false;
bool rightDown = false;

int squareDistance(vertex a, vertex b)
{
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

void Hermite(int n)     
{
	//求出相对于控制点的向量（切线）
	//把切点的长度扩大4倍，划线的效果更为明显（实际上不符合计算出的公式）
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
	glClear(GL_COLOR_BUFFER_BIT);       //清除。GL_COLOR_BUFFER_BIT表示清除颜色

	glPointSize(10.0f);
	glColor3f(0, 0, 1);
	//画出切点和控制点（蓝色）
	glBegin(GL_POINTS);
	glVertex2d(p0.x, p0.y);
	glVertex2d(p1.x, p1.y);
	glVertex2d(c0.x, c0.y);
	glVertex2d(c1.x, c1.y);
	glEnd();

	//画出切点与型值点的连线（红色）
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
void mouse(int button, int state, int x, int y)       //监听鼠标动作
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
void motion(int x, int y)       //移动点
{
	if (leftDown)        //左键移动控制点
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
	else if (rightDown)      //右键移动切点
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
	glutPostRedisplay();        //重新构图
}
void Reshape(int w, int h)      //两个参数：窗口被移动后大小
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
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - width) >> 1, (GetSystemMetrics(SM_CYSCREEN) - height) >> 1);       //指定窗口位置
	glutInitWindowSize(width, height);       //窗口大小
	glutCreateWindow(title);

	glClearColor(1, 1, 1, 0.0);
	glShadeModel(GL_FLAT);
}

int main(int argc, char *argv[])
{
	initWindow(argc, argv, 500, 500, "Hermite");

	puts("\n\t使用Hermite算法，用两顶点两控制点绘制三次曲线。");
	puts("\t左键移动控制点p0、p1，右键移动切线上的点c0、c1");

	glutDisplayFunc(myDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}