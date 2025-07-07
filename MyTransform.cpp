#include "MyTransform.h"

MyTransform::MyTransform() 
	: position(0.0f), rotation(0.0f), scale(1.0f) {
}

void MyTransform::setPosition(const glm::vec3& pos) {
	position = pos;
}

void MyTransform::setRotation(const glm::vec3& rot) {
	rotation = rot;
}

void MyTransform::setScale(const glm::vec3& scl) {
	scale = scl;
}