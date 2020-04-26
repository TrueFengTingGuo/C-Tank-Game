#pragma once

#include "GameObject.h"

// Inherits from GameObject
class Effect : public GameObject {
public:
	Effect(glm::vec3& entityPos, float angle, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	void render(Shader& shader)override;
};
