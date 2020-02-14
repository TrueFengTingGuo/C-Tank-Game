#pragma once

#include "GameObject.h"

// Inherits from GameObject
class asteroid : public GameObject {
public:
	asteroid(glm::vec3 &entityPos, float angle, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	void render(Shader& shader)override;

	//getter


	//setter


private:

};
