#include "MyObject.h"

MyObject::MyObject(unsigned int vao,MyShader shader) : VAO(vao) ,shader(shader){
	// 初始化变换属性
	transform.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

void MyObject::Display() {
	// 使用着色器程序
	shader.use();
	// 绑定VAO
	glBindVertexArray(VAO);
	// 设置变换矩阵
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transform.getModelMatrix()));
	// 绘制对象
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// 解绑VAO
	glBindVertexArray(0);
}