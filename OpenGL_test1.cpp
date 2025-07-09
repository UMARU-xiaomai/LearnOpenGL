// OpenGL_test1.cpp: 定义应用程序的入口点。
//

#include "OpenGL_test1.h"
#include "MyShader.h"	
#include "MyTransform.h" // 自定义变换类
#include "MyObject.h" // 自定义对象类
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h" // 用于加载纹理图像
#include <thread> 
#include <chrono>
using namespace std;

#include <fstream>
#include <sstream>

const double targetFPS = 60.0;
const double frameDuration = targetFPS>0?1.0 / targetFPS:0.0;

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
	*-7.VAO,VBO,EBO------------------------------------------------------------------
	*/
	// 顶点数据
	float vertices[] = {
		// 顶点坐标			// 纹理坐标
		0.5f, 0.5f, 0.0f,   1.0f, 1.0f, // 右上角
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f,  0.0f, 1.0f  // 左上角
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// 绑定纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1); // 启用纹理坐标属性

	// 解绑 VAO（可选）
	// glBindVertexArray(0);

	/*
	* -纹理---------------
	*/

	// 生成纹理对象
	unsigned int textures[2];
	glGenTextures(2, textures);

	glActiveTexture(GL_TEXTURE0); // OpenGL默认激活纹理单元0，至少16个纹理单元(GL_TEXTURE0 ~ GL_TEXTURE15)
	glBindTexture(GL_TEXTURE_2D, textures[0]); // 绑定纹理对象
	AddTexture("box.jpg");

	glActiveTexture(GL_TEXTURE1); // OpenGL默认激活纹理单元0，至少16个纹理单元(GL_TEXTURE0 ~ GL_TEXTURE15)
	glBindTexture(GL_TEXTURE_2D, textures[1]); // 绑定纹理对象
	AddTexture("smile.png");


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

	// 设置纹理单元,由于只有在启动着色器程序后才能使用glUniform函数，所以需要在shader.use()之后设置纹理单元
	glUniform1i(glGetUniformLocation(shader.ID, "boxTexture"), 0); // 手动设置
	shader.setInt("smileTexture", 1); // 或者使用着色器类设置

	MyObject myObject(VAO, shader); // 创建自定义对象

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 设置绘制模式为线框模式

	// 检查可用的顶点属性数量
	/*int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;*/

	// 修复时间管理系统
	double lastTime = glfwGetTime(); // 用于记录上一次的时间
	double lastFrameTime = lastTime; // 用于计算帧率
	int frameCount = 0; // 帧计数器
	double fpsUpdateTime = lastTime; // 用于更新FPS显示的时间

	// 5.渲染循环
	while (!glfwWindowShouldClose(window)) //检查窗口是否被关闭
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime; // 当前帧与上一帧的时间差
		lastTime = currentTime;

		// 计算并显示帧率（每秒更新一次）
		frameCount++;
		if (currentTime - fpsUpdateTime >= 1.0) // 每秒更新一次FPS
		{
			double fps = frameCount / (currentTime - fpsUpdateTime);
			std::cout << "FPS: " << fps << std::endl;
			frameCount = 0;
			fpsUpdateTime = currentTime;
		}

		// 处理输入
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			const glm::vec3& p = myObject.transform.position;
			myObject.transform.setPosition(glm::vec3(p.x, p.y + 0.5f * deltaTime, p.z));
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			const glm::vec3& p = myObject.transform.position;
			myObject.transform.setPosition(glm::vec3(p.x, p.y - 0.5f * deltaTime, p.z));
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			const glm::vec3& p = myObject.transform.position;
			myObject.transform.setPosition(glm::vec3(p.x - 0.5f * deltaTime, p.y, p.z));
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			const glm::vec3& p = myObject.transform.position;
			myObject.transform.setPosition(glm::vec3(p.x + 0.5f * deltaTime, p.y, p.z));
		}

		// 清屏
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绘制对象
		myObject.Display(); // 绘制自定义对象

		// 交换缓冲区
		glfwSwapBuffers(window);//交换前后缓冲区
		glfwPollEvents();//检查是否有事件发生

		// 帧率限制
		double frameTime = glfwGetTime() - currentTime;
		if (frameDuration>0 && frameTime < frameDuration)
		{
			double sleepTime = frameDuration - frameTime;
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
		}
	}

	// 6.清理资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(2, textures);

	glfwTerminate();
	return 0;
}

void AddTexture(const char* address)
{
	// 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S轴重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // T轴拉伸
	// 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 缩小过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大过滤

	// 使用stbi_load加载纹理图像
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // 翻转图像以适应OpenGL的坐标系
	unsigned char* data = stbi_load(address, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = GL_RGBA;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // 生成mipmap
	}
	else
	{
		std::cout << "Failed to load texture: " << address << std::endl;
	}
	stbi_image_free(data); // 释放图像内存
}