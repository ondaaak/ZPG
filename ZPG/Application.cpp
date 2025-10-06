
#include "Application.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "../Models/sphere.h"
#include "../Models/gift.h"
#include "../Models/tree.h"
#include "../Models/bushes.h"
#include "../Models/suzi_flat.h"
#include <stdio.h>
#include <stdlib.h>

float square[] = {
    -0.5f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
};

float triangle[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

// X'= P * V * M * X;
/*
const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"out vec4 position;"
"uniform mat4 modelMatrix;"
"uniform mat4 viewMatrix;"
"uniform mat4 projectionMatrix;"
"void main () {"
"     position = vec4 (vp, 1.0);"
"     gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);"
"}";
*/

const char* vertex_shader =
"#version 330\n"
"uniform mat4 modelMatrix;"
"uniform mat4 projectMatrix;"
"uniform mat4 viewMatrix;"
"out vec3 vertexColor;"
"out vec4 position;"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 vn;"
"void main () {"
"     position = vec4 (vp, 1.0);"
"     vertexColor=vn;"
"     gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);"
"}";


const char* fragment_shader =
"#version 330\n"
"out vec4 frag_colour;"
"in vec3 vertexColor;"
"in vec4 position;"
"void main () {"
"     frag_colour = vec4(position.x, position.y, position.z, 1.0);"
"}";


/*
const char* fragment_shader =
"#version 330\n"
"in vec4 position;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (position.x, position.y, position.z, 1.0);"
"}";
*/
const char* fragment_shader2 =
"#version 330\n"
"in vec4 position;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (0.2, 0.5, 0.5, 1.0);"
"}";

static Application* app = nullptr;

void Application::switchScene(int sceneNumber) {
	switch (sceneNumber) {
	case 1:
		activeScene = scene1;
		printf("Switched to Scene 1\n");
		break;
	case 2:
		activeScene = scene2;
		printf("Switched to Scene 2\n");
		break;
	case 3:
		activeScene = scene3;
		printf("Switched to Scene 3\n");
		break;
	default:
		printf("Invalid scene number: %d\n", sceneNumber);
		break;
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) app->switchScene(1);
    if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) app->switchScene(2);
    if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS) app->switchScene(3);

}

Application::Application()
	: window(nullptr), scene1(nullptr), scene2(nullptr), scene3(nullptr), activeScene(nullptr)
{}

Application::~Application() {
    cleanup();
}

bool Application::init() {

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    window = glfwCreateWindow(1024, 800, "ZPG", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);

    return true;
}

void Application::run() {
    if (!init()) return;
    app = this;
    glfwSetKeyCallback(window, key_callback);

    scene1 = new Scene();
    scene2 = new Scene();
    scene3 = new Scene();
    activeScene = scene1;


    Shader* vertexShader = new Shader(GL_VERTEX_SHADER, vertex_shader);
    Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, fragment_shader);
    Shader* fragmentShader2 = new Shader(GL_FRAGMENT_SHADER, fragment_shader2);
    ShaderProgram* shaderProgram = new ShaderProgram(*vertexShader, *fragmentShader);
    ShaderProgram* shaderProgram2 = new ShaderProgram(*vertexShader, *fragmentShader2);
    
    
    Model* triangleModel = new Model(triangle, sizeof(triangle) / sizeof(float) / 3,false);
    DrawableObject* triangleObject = new DrawableObject(triangleModel, shaderProgram2);

    
    Model* sphereModel = new Model(sphere, sizeof(sphere) / sizeof(float) / 6, true);
    Model* giftModel = new Model(gift, sizeof(gift) / sizeof(float) / 6, true);
    Model* treeModel = new Model(tree, sizeof(tree) / sizeof(float) / 6, true);
    Model* bushModel = new Model(bushes, sizeof(bushes) / sizeof(float) / 6, true);





	
  	
    for (int i = 0; i < 5; ++i) {
        DrawableObject* obj = new DrawableObject(sphereModel, shaderProgram2);
        obj->getTransformation().setPosition(glm::vec3(-0.8f + i * 0.4f, 0.7f, 0.0f));
        obj->getTransformation().setScale(glm::vec3(0.1f));
        obj->getTransformation().setRotation(i * 0.5f, glm::vec3(0, 0, 1));
        scene3->addObject(obj);
    }

    for (int i = 0; i < 5; ++i) {
        DrawableObject* obj = new DrawableObject(giftModel, shaderProgram2);
        obj->getTransformation().setPosition(glm::vec3(-0.8f + i * 0.4f, 0.2f, 0.0f));
        obj->getTransformation().setScale(glm::vec3(0.8f));
        obj->getTransformation().setRotation(i * 0.3f, glm::vec3(0, 1, 1));
        scene3->addObject(obj);
    }
    
    
    for (int i = 0; i < 5; ++i) {
        DrawableObject* obj = new DrawableObject(treeModel, shaderProgram);
        obj->getTransformation().setPosition(glm::vec3(-0.8f + i * 0.4f, -0.1f, 0.0f));
        obj->getTransformation().setScale(glm::vec3(0.1f));
        obj->getTransformation().setRotation(i * 0.2f, glm::vec3(1, 0, 0));
        scene3->addObject(obj);
    }

    
    for (int i = 0; i < 5; ++i) {
        DrawableObject* obj = new DrawableObject(bushModel, shaderProgram);
        obj->getTransformation().setPosition(glm::vec3(-0.8f + i * 0.4f, -0.5f, 0.0f));
        obj->getTransformation().setScale(glm::vec3(0.5f));
        obj->getTransformation().setRotation(i * 0.7f, glm::vec3(1, 1, 0));
        scene3->addObject(obj);
    }
    
    

	DrawableObject* sphere1 = new DrawableObject(sphereModel, shaderProgram);
    DrawableObject* sphere2 = new DrawableObject(sphereModel, shaderProgram);
    DrawableObject* sphere3 = new DrawableObject(sphereModel, shaderProgram);
    DrawableObject* sphere4 = new DrawableObject(sphereModel, shaderProgram);

	sphere1->getTransformation().scale(glm::vec3(0.2f, 0.2f, 0.2f));
    sphere2->getTransformation().scale(glm::vec3(0.2f, 0.2f, 0.2f));
    sphere3->getTransformation().scale(glm::vec3(0.2f, 0.2f, 0.2f));
    sphere4->getTransformation().scale(glm::vec3(0.2f, 0.2f, 0.2f));

	sphere1->getTransformation().setPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    sphere2->getTransformation().setPosition(glm::vec3(0.5f, 0.0f, 0.0f));
    sphere3->getTransformation().setPosition(glm::vec3(0.0f, -0.5f, 0.0f));
    sphere4->getTransformation().setPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
    

    
    
    scene1->addObject(triangleObject);

	scene2->addObject(sphere1);
    scene2->addObject(sphere2);
    scene2->addObject(sphere3);
    scene2->addObject(sphere4);
    
    glm::mat4 M = glm::mat4(1.0f);
    float alpha = 0;
    GLint matrixID;



    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        triangleObject->getTransformation().rotate(0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
		

		if (alpha >=0 && alpha <= 5) alpha += 0.02;
		else alpha = 0;

        M = glm::rotate(glm::mat4(1.0f), alpha, glm::vec3(0.0f, 0.0f, 1.0f));
		shaderProgram->SetUniform("modelMatrix", M);

        M = glm::lookAt(glm::vec3(0.0f, 0.0f, alpha), glm::vec3(0.f,0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        shaderProgram->SetUniform("viewMatrix", M);

        M = glm::perspective(45.0f, 1024.f / 800.f, 0.1f, 100.0f);
        shaderProgram->SetUniform("projectMatrix", M);








		if (activeScene) activeScene->render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void Application::cleanup() {
    delete scene1;
	delete scene2;
	delete scene3;
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
