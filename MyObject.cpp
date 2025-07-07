#include "MyObject.h"

MyObject::MyObject(unsigned int vao,MyShader shader) : VAO(vao) ,shader(shader){
	// ��ʼ���任����
	transform.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

void MyObject::Display() {
	// ʹ����ɫ������
	shader.use();
	// ��VAO
	glBindVertexArray(VAO);
	// ���ñ任����
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transform.getModelMatrix()));
	// ���ƶ���
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// ���VAO
	glBindVertexArray(0);
}