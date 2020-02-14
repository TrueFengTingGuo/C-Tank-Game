#include "PlayerGameObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityTexture, entityNumElements, 0) {}

// Update function for moving the player object around
void PlayerGameObject::update(double deltaTime) {

	// special player updates go here


	if (currentFireCooldown > 0) { 
		currentFireCooldown -= 1.0f; 
	}
	
	velocity[0] = velocity[0] * 0.998;
	velocity[1] = velocity[1] * 0.998;

	position[0] += velocity[0] * (float)deltaTime ;
	position[1] += velocity[1] * (float)deltaTime ;
}

void PlayerGameObject::render(Shader& shader) {
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

