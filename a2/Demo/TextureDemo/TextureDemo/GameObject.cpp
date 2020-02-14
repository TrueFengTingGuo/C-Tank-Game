#include "GameObject.h"
#include <iostream>    // Needed to perform IO operations
#include <string>


using namespace std;
/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, int initName) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f); // starts out stationary
	name = initName;
}

// Updates the GameObject's state
void GameObject::update(double deltaTime) {


	// Update object position with Euler integration
	position[0] += velocity[0] * (float)deltaTime ;
	position[1] += velocity[1] * (float)deltaTime;
}

// Renders the GameObject using the shader
void GameObject::render(Shader &shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle*180/ 3.141592f, glm::vec3(0, 0, 1));
	

	// Set the transformation matrix in the shader
	// TODO: Multiply your new transformations to make the transformationMatrix
	glm::mat4 transformationMatrix = translationMatrix * rotate;

	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	shader.setUniform2f("offset", glm::vec2(0, 0));
	shader.setUniform4f("Mycolor", glm::vec4(0.3, 0.3, 0.3, 1));
	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
