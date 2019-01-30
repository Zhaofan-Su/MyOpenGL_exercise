#include"GLFW/glfw3.h"
#include"glad/glad.h"
#include<iostream>

//������ɫ��
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";


//Ƭ����ɫ��
//ֻ��Ҫһ�������������һ��4����������ʾ���յ������ɫ
const char* framgmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"   //alphaֵΪ1.0f��ʾ��ȫ��͸��
"}\n\0";


//�Դ���ע��һ���ص�������ÿ�δ��ڴ�С������ʱ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//������ƣ�����Ƿ��¡�Esc��
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//��ʼ��GLFW
	glfwInit();

	//����OpenGL3.3�汾

	//����GLFW�����汾��Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//����GLFW�Ĵ˰汾��Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//����GLFWʹ�ú���ģʽ��ֻʹ��OpenGl��һ���Ӽ�
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ��󣬴�������кʹ����йص�����
	//���ڿ�Ϊ800����Ϊ600������ΪLearnOpenGL
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//ע�ᴰ�ڻص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD����������OpenGL�ĺ���ָ�룬�ڵ����κ�OpenGL�ĺ���֮ǰ��Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//����OpenGL��Ⱦ���ڣ��ӿڣ�Viewport���ĳߴ��С
	//ǰ�����������ƴ������½ǵ�λ�ã����������������ӿڵĿ��
	glViewport(0, 0, 800, 600);

	//���붨����ɫ������,����ʱ��̬����Դ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//����ɫ��Դ�븽�ӵ���ɫ�������ϣ����б���
	//��һ������ΪҪ�������ɫ�����󣬵ڶ�����ָ�����ݵ�Դ���ַ������������������Ƕ�����ɫ��������Դ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//�����ɫ���Ƿ����ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//����Ƭ����ɫ������
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

	//��������ɫ���������ӵ�һ��������Ⱦ����ɫ��������
	//��ɫ����������Ƕ����ɫ���ϲ�֮������������ɵİ汾
	//��Ⱦ�����ʱ�򼤻���ɫ������

	//������ɫ���������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//�ѱ��������ɫ�����ӵ����������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//���������ɫ�������Ƿ�ʧ�ܣ���ȡ��Ӧ��־
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	//���ӳɹ�֮��ɾ����ɫ�����󣬲�����Ҫ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	////�����������㣬ֻ����3�����϶�Ϊ-1��1��OpenGL�Ŵ����������
	//float vertices[] = {
	//	-0.5f,-0.5f,0.0f,
	//	0.5f,-0.5f,0.0f,
	//	0.0f,0.5f,0.0f
	//};

	//����һ������
	float vertices[] = {
		0.5f,0.5f,0.0f,  //���Ͻ�
		0.5f,-0.5f,0.0f, //���½�
		-0.5f,-0.5f,0.0f,//���½�
		-0.5f,0.5f,0.0f  //���Ͻ�
	};

	unsigned int indices[] = {  //������0��ʼ
		0,1,3, //��һ��������
		1,2,3  //�ڶ���������
	};
	//���㻺�����(����������GL_ARRAY_BUFFER)����GPU�ڴ棬�����������VAO
	unsigned int VBO, VAO;
	//���������������
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	//ʹ��һ������ID����VBO����
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//��VAO
	glBindVertexArray(VAO);

	//�󶨻���
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�Ѷ������ݸ��Ƶ�������ڴ���
	//��һ������ΪĿ�껺������ͣ��ڶ�������ָ���������ݵĴ�С�����ֽ�Ϊ��λ��
	//����������Ϊ���͵�ʵ�����ݣ����ĸ�����ָ���Կ���ι������������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//��EBO�����������Ƶ�������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//ʹ���߽��л���
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//���Ƴ�������������䣨Ĭ��ģʽ��
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//ѭ����Ⱦ���ڣ���GLFW�˳�֮ǰһֱ��������
	while (!glfwWindowShouldClose(window))
	{
		//ÿ�μ���Ƿ��¡�Esc��
		processInput(window);

		//��Ⱦָ��
		//����
		//����

		//ÿ����Ⱦ���������Ļ���õ���ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//����ĺ�������֮�������ɫ���壬������ɫ�������Ϊ���溯�����õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);

		//����ͼ��
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		
		//����������
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//���ƾ���
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//������ɫ���壬����������ͼ����˸���⣬��ʹ�ý���ǰ��˫����
		glfwSwapBuffers(window);
		//����Ƿ��м������롢����ƶ����¼�����������״̬����
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//�ͷ�֮ǰ�������Դ
	glfwTerminate();
	return 0;
}



