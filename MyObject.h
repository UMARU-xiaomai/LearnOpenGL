#pragma once
#include <iostream>
#include "MyTransform.h"
#include "MyShader.h"

class MyObject {
public:
	MyObject() = delete; // 禁止默认构造函数
	MyObject(unsigned int vao,MyShader shader);
	unsigned int VAO; // 顶点数组对象
	MyTransform transform; // 对象的变换属性
	MyShader shader; // 着色器程序
	void Display();
};