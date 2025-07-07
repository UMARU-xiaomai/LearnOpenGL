#pragma once

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class MyShader {
public:
	unsigned int ID;
	// 构造函数读取并构建着色器
	MyShader(const char* vertexPath, const char* fragmentPath);
	// 使用着色器
	void use();
	// 设置uniform变量
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	string readFile(const char* filePath);
};