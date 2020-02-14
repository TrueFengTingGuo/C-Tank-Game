#include "Bullet.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

Bullet::Bullet(glm::vec3 &entityPos,float angle,GLuint entityTexture, GLint entityNumElements) : 
	GameObject(entityPos, entityTexture, entityNumElements,1) {
	setVelocity(glm::vec3(cos(angle)*10 , sin(angle) * 10, 0));
	} 

// Update function for moving the player object around
// Updates the GameObject's state
void Bullet::update(double deltaTime) {

	angle += 0.01f;
	// Call the parent's update method to move the object in standard way, if desired
	GameObject::update(deltaTime);
}


void Bullet::render(Shader& shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle * 180 / 3.141592f, glm::vec3(0, 0, 1));

	// Set the transformation matrix in the shader
	// TODO: Multiply your new transformations to make the transformationMatrix
	glm::mat4 transformationMatrix = translationMatrix * rotate;

	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	shader.setUniform2f("offset", glm::vec2(glfwGetTime(), 0));
	shader.setUniform4f("Mycolor", glm::vec4(0.3, 0.3, 0.3, 1));
	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

