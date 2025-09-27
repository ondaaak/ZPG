
#include "Application.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include <stdio.h>
#include <stdlib.h>

float points[] = {
    -0.5f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
};

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"out vec4 position;"
"void main () {"
"     position = vec4 (vp, 1.0);"
"     gl_Position = position;"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec4 position;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (position.x, position.y, position.z, 1.0);"
"}";

Application::Application()
    : window(nullptr), scene(nullptr)
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
    ShaderProgram* shaderProgram = new ShaderProgram(*vertexShader, *fragmentShader);
    shaderProgram->setShaderProgram();

    DrawableObject* object = new DrawableObject(points, sizeof(points) / sizeof(float), shaderProgram);
    scene = new Scene();
    scene->addObject(object);

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
        scene->render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void Application::cleanup() {
    delete scene;
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
