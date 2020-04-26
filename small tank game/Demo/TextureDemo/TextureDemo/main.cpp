#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Shader.h"
#include "Window.h"
#include "PlayerGameObject.h"
#include "Bullet.h"
#include "asteroid.h"
#include "Effect.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Transform Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);

// Global texture info
GLuint tex[4];

// Global game object info
std::vector<GameObject*> gameObjects;


// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[]  = {
		//  square (two triangles)
		   //  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}


void setthisTexture(GLuint w, char *fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setallTexture(void)
{
//	tex = new GLuint[4];
	glGenTextures(3, tex);
	setthisTexture(tex[0], "PlayerShip.png");
	setthisTexture(tex[1], "effect2.png");
	setthisTexture(tex[2], "asteroid2.png");
	setthisTexture(tex[3], "bullet2.png");

	glBindTexture(GL_TEXTURE_2D, tex[0]);
}

void setup(void)
{

	// Set up z-buffer for rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	int size = 6;


	// Set up the textures
	setallTexture();

	// Setup the player object (position, texture, vertex count)
	// Note, player object should always be the first object in the game object vector 
	gameObjects.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[0], size));

	// Setup other objects


}

void controls(void)
{
	PlayerGameObject *player = dynamic_cast <PlayerGameObject*>(gameObjects[0]);

	glm::vec3 curpos = player->getPosition();

	float oldangle = player->getAngle();
	glm::vec3 oldvelocity = player->getVelocity();
	float hypo = sqrt(pow(oldvelocity[0], 2) + pow(oldvelocity[1], 2));

	// Checking for player input and making changes accordingly
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_W) == GLFW_PRESS) {

		float angle = player->getAngle();
		float scale = 0.004;
		player->setVelocity(oldvelocity + glm::vec3(cos(angle) * scale, sin(angle) * scale, 0));

	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
		
		player->setVelocity(oldvelocity - oldvelocity * .002f );

	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_D) == GLFW_PRESS) {	

		player->setAngle(oldangle - 0.005);

		player->setVelocity(glm::vec3(cos(oldangle)* hypo, sin(oldangle)* hypo, 0) );
		
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_A) == GLFW_PRESS) {

		player->setAngle(oldangle + 0.005);

		player->setVelocity(glm::vec3(cos(oldangle) * hypo, sin(oldangle) * hypo, 0));
	
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_C) == GLFW_PRESS) {

		if (player->getcurrentFireCooldown() <= 0) {
			gameObjects.push_back(new Bullet(curpos, oldangle, tex[3], 6));
			player->resetCurrentFireCooldown();
		}
	}
}

void addAsteroid() {

	int addingSpeed = 1000;
	int rotateSpeed = 10;

	if ((rand() % addingSpeed + 1) == 5) {
		gameObjects.push_back(new asteroid(glm::vec3(-7, 7, 0), glfwGetTime()*rotateSpeed, tex[2], 6));
	}

	if ((rand() % addingSpeed + 1) == 5) {
		gameObjects.push_back(new asteroid(glm::vec3(7, 7, 0), glfwGetTime()*rotateSpeed, tex[2], 6));
	}

	if ((rand() % addingSpeed + 1) == 5) {
		gameObjects.push_back(new asteroid(glm::vec3(-7, -7, 0), glfwGetTime()*rotateSpeed, tex[2], 6));
	}

	if ((rand() % addingSpeed + 1) == 5) {
		gameObjects.push_back(new asteroid(glm::vec3(7, -7, 0), glfwGetTime()*rotateSpeed, tex[2], 6));
	}
	
}
void gameLoop(Window &window, Shader &shader, double deltaTime)
{
	// Clear background
	window.clear(viewport_background_color_g);

	// set view to zoom out, centred by default at 0,0
	float cameraZoom = 0.15f;
	glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));
	shader.setUniformMat4("viewMatrix", viewMatrix);

	//add Asteroid
	addAsteroid();

	// apply user input
	controls();

	
	// Update and render all game objects
	for (int i = 0; i < gameObjects.size(); i++) {
		// Get the current object
		GameObject* currentGameObject = gameObjects[i];
		
		if (i == 0) {
			
			glm::vec3 curpos = currentGameObject->getPosition();
			glm::vec3 oldvelocity = currentGameObject->getVelocity();

			if (curpos.x < -6.5) {

				currentGameObject->setVelocity(oldvelocity - oldvelocity * .02f);
				//currentGameObject->setPosition(glm::vec3(7, curpos.y,0));
			}
			if (curpos.x > 6.5) {
				currentGameObject->setVelocity(oldvelocity - oldvelocity * .02f);
				//currentGameObject->setPosition(glm::vec3(-7, curpos.y, 0));
			}
			if (curpos.y < -6.5) {
				currentGameObject->setVelocity(oldvelocity - oldvelocity * .02f);
				//currentGameObject->setPosition(glm::vec3(curpos.x, 7, 0));
			}
			if (curpos.y > 6.5) {
				currentGameObject->setVelocity(oldvelocity - oldvelocity * .02f);
				//currentGameObject->setPosition(glm::vec3(curpos.x,-7, 0));
			}
		}
		else {
			glm::vec3 curpos = currentGameObject->getPosition();
			if (curpos.x < -8|| curpos.x > 8 || curpos.y < -8|| curpos.y > 8) {

				gameObjects.erase(gameObjects.begin() + i);
			
			}
			
		}
		// Update game objects
		currentGameObject->update(deltaTime);


		// Check for collision between game objects
		for (int j = 0; j < gameObjects.size(); j++) {
			GameObject* otherGameObject = gameObjects[j];

			float distance = glm::length(currentGameObject->getPosition() - otherGameObject->getPosition());
			if (distance < 1.0f) {
				// This is where you would perform collision response between objects
				
				if (currentGameObject->getName() == 2 && otherGameObject->getName() == 2) {
					//otherGameObject->setVelocity(otherGameObject->getVelocity() * (-1.0f));
				}

				if (currentGameObject->getName() == 1 && otherGameObject->getName() == 2) {

					for (float angleToGo = 0; angleToGo < 360; angleToGo+=5) {

							gameObjects.push_back(new Effect(currentGameObject->getPosition(), angleToGo, tex[1], 6));

						
					}
					

					if (j < i) {
						gameObjects.erase(gameObjects.begin() + i);
						gameObjects.erase(gameObjects.begin() + j);
					}
					else {
						gameObjects.erase(gameObjects.begin() + j);
						gameObjects.erase(gameObjects.begin() + i);
					}
					
					i-=2;
					break;
				}

				if (currentGameObject->getName() == 0 && otherGameObject->getName() == 2) {

					exit(0);
				}
			}
		}

		// Render game objects
		currentGameObject->render(shader);
	}

	// Update other events like input handling
	glfwPollEvents();

	// Push buffer drawn in the background onto the display
	glfwSwapBuffers(window.getWindow());
}

// Main function that builds and runs the game
int main(void){
	try {

		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);
		Shader shader("shader.vert", "shader.frag");
		shader.enable();

		setup();

		// Run the main loop
		double lastTime = glfwGetTime();
		while (!glfwWindowShouldClose(window.getWindow())) {
			
			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			gameLoop(window, shader, deltaTime);
		}
	}
	catch (std::exception &e){
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
}
