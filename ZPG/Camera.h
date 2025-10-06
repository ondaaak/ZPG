#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 getCamera();


private:
	~Camera();

	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;


};

