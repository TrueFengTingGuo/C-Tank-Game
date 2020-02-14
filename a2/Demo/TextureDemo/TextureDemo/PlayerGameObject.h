#pragma once

#include "GameObject.h"

// Inherits from GameObject
class PlayerGameObject : public GameObject {
public:
	PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	void render(Shader& shader)override;

	//getter
	inline float getcurrentFireCooldown() { return currentFireCooldown; }

	//setter
	void setCurrentFireCooldown(float newCooldown) { currentFireCooldown = newCooldown; }

	void resetCurrentFireCooldown() { currentFireCooldown = InitalfireRate; }

private:
	float InitalfireRate = 1000;
	float currentFireCooldown = 0;
};
