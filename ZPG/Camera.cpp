#include "Camera.h"


Camera::Camera()
{

}
Camera::~Camera()
{
}
glm::mat4 Camera::getCamera() {
	return glm::lookAt(eye, eye + target, up);
}
