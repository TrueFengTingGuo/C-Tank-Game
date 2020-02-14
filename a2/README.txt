1. run sln. file

2. add an instance variable "angle". a object's next position is determined by its velocity and angle.

3. gameobjectplayer overrides the render() function. add shader.setUniform2f("offset", glm::vec2); into gameobjectplayer's render;
add shader.setUniform2f("offset", glm::vec2); into gameobject's render; Keep the values of two "glm::vec2" different from each other.
Inside the shader.vert, create a variable by typing "uniform vec2 offset;". offest the vertex by minusing the vertex.

4.download 3 png files from websites, add them to the directory "Getting Started\Demo\TextureDemo\TextureDemo".  Modify
the names pass to the function "setthisTexture()".

5.add shader.setUniform4f("Mycolor", glm::vec4); into gameobjectplayer's render; add shader.setUniform4f("Mycolor", glm::vec2); into 
gameobject's render; Keep the values of two "glm::vec4" different from each other. Inside the shader.frag, create a variable by typing
"uniform vec4 Mycolor". Change the value of FragColor by multiplying  "My Color"




