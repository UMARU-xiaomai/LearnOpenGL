#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyTransform
{
	public:
		glm::vec3 position;  // Î»ÖÃ
		glm::vec3 rotation; // Ðý×ª
		glm::vec3 scale;    // Ëõ·Å
		MyTransform();
		void setPosition(const glm::vec3& pos);
		void setRotation(const glm::vec3& rot);
		void setScale(const glm::vec3& scl);
	glm::mat4 getModelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
		return model;
	}
};