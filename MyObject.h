#pragma once
#include <iostream>
#include "MyTransform.h"
#include "MyShader.h"

class MyObject {
public:
	MyObject() = delete; // ��ֹĬ�Ϲ��캯��
	MyObject(unsigned int vao,MyShader shader);
	unsigned int VAO; // �����������
	MyTransform transform; // ����ı任����
	MyShader shader; // ��ɫ������
	void Display();
};