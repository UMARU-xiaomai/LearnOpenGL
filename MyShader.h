#pragma once

#include <glad/glad.h> // ����glad����ȡ���еı���OpenGLͷ�ļ�

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class MyShader {
public:
	unsigned int ID;
	// ���캯����ȡ��������ɫ��
	MyShader(const char* vertexPath, const char* fragmentPath);
	// ʹ����ɫ��
	void use();
	// ����uniform����
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	string readFile(const char* filePath);
};