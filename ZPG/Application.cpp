
#include "Application.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
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
     -0.3f, 0.1f, 0.0f,
     0.3f, 0.1f, 0.0f,
     0.0f, 0.8f, 0.0f,
};

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"out vec4 position;"
"uniform mat4 modelMatrix;"
"void main () {"
"     position = vec4 (vp, 1.0);"
"     gl_Position =  modelMatrix * position;"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec4 position;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (position.x, position.y, position.z, 1.0);"
"}";

const char* fragment_shader2 =
"#version 330\n"
"in vec4 position;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (1.0, 0.5, 0.5, 1.0);"
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

    Shader* vertexShader = new Shader(GL_VERTEX_SHADER, vertex_shader);
    Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, fragment_shader);
    Shader* fragmentShader2 = new Shader(GL_FRAGMENT_SHADER, fragment_shader2);
    ShaderProgram* shaderProgram = new ShaderProgram(*vertexShader, *fragmentShader);
    ShaderProgram* shaderProgram2 = new ShaderProgram(*vertexShader, *fragmentShader2);
    shaderProgram->setShaderProgram();


    Model* squareModel = new Model(square, sizeof(square) / sizeof(float) / 3); 
    DrawableObject* squareObject = new DrawableObject(squareModel, shaderProgram); 

    squareObject->getTransformation().rotate(0.5f, glm::vec3(0, 0, 1));
    squareObject->getTransformation().translate(glm::vec3(0.0f, -0.5f, 0.0f));
    squareObject->getTransformation().rotate(-1.0f, glm::vec3(0, 0, 1));
	squareObject->getTransformation().scale(glm::vec3(0.5f, 0.5f, 0.5f));

    scene1 = new Scene();
    scene1->addObject(squareObject);
    activeScene = scene1;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

    return true;
}

void Application::run() {
    if (!init()) return;
    app = this;
    glfwSetKeyCallback(window, key_callback);
    // get version info
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
