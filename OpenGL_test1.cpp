// OpenGL_test1.cpp: 定义应用程序的入口点。
//

#include "OpenGL_test1.h"
#include "MyShader.h"	
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h" // 用于加载纹理图像
using namespace std;

#include <fstream>
#include <sstream>

int main()
{
	// 1.初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 如果使用的是Mac OS X系统

	// 2.创建窗口对象
	GLFWwindow* window = glfwCreateWindow(300, 300, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 3.初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 4.设置视口(OpenGL渲染窗口的尺寸大小)
	glViewport(0, 0, 300, 300);
	// 4.1 设置视口的回调函数(即当用户改变窗口的大小的时候，视口自动调整)
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});
	
	/*
	*-7.VAO,VBO,EBO，纹理------------------------------------------------------------------
	*/
	// 顶点数据
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};
	// EBO索引
	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	// 生成并绑定 VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 生成并绑定 VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 生成并绑定 EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 解绑 VAO（可选）
	glBindVertexArray(0);

	/*
	* -7.1 纹理---------------
	*/
	float texCoords[] = {
	0.0f, 0.0f, // 左下角
	1.0f, 0.0f, // 右下角
	0.5f, 1.0f  // 上中
	};

	// 生成纹理对象
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理对象
	// 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S轴重复
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小过滤
	
	// 使用stbi_load加载纹理图像
	int width, height, nrChannels;
	unsigned char* data = stbi_load("test_texture.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // 生成mipmap
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // 释放图像内存

	/*
	*  -8.着色器------------------------------------------------------------
	*/
	// 详见 MyShader.h 和 MyShader.cpp
	MyShader shader("vertex_shader.glsl", "fragment_shader.glsl");
	// 使用着色器程序
	shader.use();

	/*
	*----------------------------------------------------------------------
	*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 设置绘制模式为线框模式

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// 5.渲染循环
	while (!glfwWindowShouldClose(window)) //检查窗口是否被关闭
	{
		//根据按键输入设置背景颜色
		//if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		//{
		//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT);
		//}
		//else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		//{
		//	glClearColor(0.3f, 0.2f, 0.3f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT);
		//}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		shader.use();
		glBindVertexArray(VAO); // 每次绘制时绑定对应VAO即可，无需手动绑定VBO
		// glDrawArrays(GL_TRIANGLES, 0, 3); // 使用VBO绘制三角形
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 使用EBO绘制

		glfwSwapBuffers(window);//交换前后缓冲区
		glfwPollEvents();//检查是否有事件发生
	}
	// 6.清理资源
	glfwTerminate();
	return 0;
}

