#pragma once
//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  
#include "Scene.h"

class Application
{
	public:
	Application();
	~Application();
	void initialization();
	void createShaders();
	void createModels();
	void run();
};

