#include"GLFW/glfw3.h"
#include"glad/glad.h"
#include<iostream>

//顶点着色器
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";


//片段着色器
//只需要一个输出变量，是一个4分向量，表示最终的输出颜色
const char* framgmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"   //alpha值为1.0f表示完全不透明
"}\n\0";


//对窗口注册一个回调函数，每次窗口大小被调整时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//输入控制，检测是否按下“Esc”
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//初始化GLFW
	glfwInit();

	//基于OpenGL3.3版本

	//配置GLFW的主版本号为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//配置GLFW的此版本号为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//配置GLFW使用核心模式，只使用OpenGl的一个子集
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	//创建窗口对象，存放了所有和窗口有关的数据
	//窗口宽为800，高为600，名称为LearnOpenGL
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//注册窗口回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD是用来管理OpenGL的函数指针，在调用任何OpenGL的函数之前需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//告诉OpenGL渲染窗口（视口，Viewport）的尺寸大小
	//前两个参数控制窗口左下角的位置，后两个参数控制视口的宽高
	glViewport(0, 0, 800, 600);

	//编译定点着色器对象,运行时动态编译源码
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//把着色器源码附加到着色器对象上，进行编译
	//第一个参数为要编译的着色器对象，第二参数指定传递的源码字符串数量，第三参数是顶点着色器真正的源码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//检查着色器是否编译成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//编译片段着色器对象
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &framgmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//把两个着色器对象链接到一个用来渲染的着色器程序中
	//着色器程序对象是多个着色器合并之后并最终链接完成的版本
	//渲染对象的时候激活着色器程序

	//创建着色器程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//把编译过的着色器附加到程序对象上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//检测链接着色器程序是否失败，获取相应日志
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	//链接成功之后，删除着色器对象，不再需要了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	////输入三个顶点，只有在3个轴上都为-1到1，OpenGL才处理这个顶点
	//float vertices[] = {
	//	-0.5f,-0.5f,0.0f,
	//	0.5f,-0.5f,0.0f,
	//	0.0f,0.5f,0.0f
	//};

	//绘制一个矩形
	float vertices[] = {
		0.5f,0.5f,0.0f,  //右上角
		0.5f,-0.5f,0.0f, //右下角
		-0.5f,-0.5f,0.0f,//左下角
		-0.5f,0.5f,0.0f  //左上角
	};

	unsigned int indices[] = {  //索引从0开始
		0,1,3, //第一个三角形
		1,2,3  //第二个三角形
	};
	//顶点缓冲对象(缓冲类型是GL_ARRAY_BUFFER)管理GPU内存，顶点数组对象VAO
	unsigned int VBO, VAO;
	//创建索引缓冲对象
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	//使用一个缓冲ID生成VBO对象
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//绑定VAO
	glBindVertexArray(VAO);

	//绑定缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把顶点数据复制到缓冲的内存中
	//第一个参数为目标缓冲的类型，第二个参数指定传输数据的大小（以字节为单位）
	//第三个参数为发送的实际数据，第四个参数指定显卡如何管理给定的数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//绑定EBO，把索引复制到缓冲里
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//使用线进行绘制
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//绘制出来的是整个填充（默认模式）
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//循环渲染窗口，让GLFW退出之前一直保持运行
	while (!glfwWindowShouldClose(window))
	{
		//每次检测是否按下“Esc”
		processInput(window);

		//渲染指令
		//……
		//……

		//每次渲染设置清空屏幕所用的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//下面的函数调用之后，清除颜色缓冲，整个颜色缓冲填充为上面函数设置的颜色
		glClear(GL_COLOR_BUFFER_BIT);

		//绘制图像
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		
		//绘制三角形
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//绘制矩形
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//交换颜色缓冲，单缓冲会存在图像闪烁问题，故使用交换前后双缓冲
		glfwSwapBuffers(window);
		//检查是否有键盘输入、鼠标移动等事件触发、窗口状态更新
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//释放之前分配的资源
	glfwTerminate();
	return 0;
}



