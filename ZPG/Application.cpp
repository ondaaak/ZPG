
#include "Application.h"
#include "Camera.h"
#include <Time.h>

Camera camera;

double lastX = 512, lastY = 400;
bool firstMouse = true;
bool rightMousePressed = false;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightMousePressed = (action == GLFW_PRESS);
        firstMouse = true;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (rightMousePressed) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = float(xpos - lastX);
        float yoffset = float(ypos - lastY);
        lastX = xpos;
        lastY = ypos;
        camera.processMouse(xoffset, yoffset);
    }
}

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
    DrawableObject* triangleObject = new DrawableObject(triangleModel, shaderProgram);

    
    Model* sphereModel = new Model(sphere, sizeof(sphere) / sizeof(float) / 6, true);
    Model* giftModel = new Model(gift, sizeof(gift) / sizeof(float) / 6, true);
    Model* treeModel = new Model(tree, sizeof(tree) / sizeof(float) / 6, true);
    Model* bushModel = new Model(bushes, sizeof(bushes) / sizeof(float) / 6, true);
    
    float randomX, randomZ;
	srand((unsigned int)time(NULL));
   

    
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
		randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(treeModel, shaderProgram);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.1f)));
        obj->addTransformation(new Rotate(i * 0.2f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }

    
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
        randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(bushModel, shaderProgram);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.5f)));
        obj->addTransformation(new Rotate(i * 0.7f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }
    
    

	DrawableObject* sphere1 = new DrawableObject(sphereModel, shaderProgram);
    DrawableObject* sphere2 = new DrawableObject(sphereModel, shaderProgram);
    DrawableObject* sphere3 = new DrawableObject(sphereModel, shaderProgram);
    DrawableObject* sphere4 = new DrawableObject(sphereModel, shaderProgram);

	sphere1->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere2->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere3->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere4->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));

	sphere1->addTransformation(new Translate(glm::vec3(0.0f, 2.5f, 0.0f)));
    sphere2->addTransformation(new Translate(glm::vec3(2.5f, 0.0f, 0.0f)));
    sphere3->addTransformation(new Translate(glm::vec3(0.0f, -2.5f, 0.0f)));
    sphere4->addTransformation(new Translate(glm::vec3(-2.5f, 0.0f, 0.0f)));
    

    
    
    scene1->addObject(triangleObject);

	scene2->addObject(sphere1);
    scene2->addObject(sphere2);
    scene2->addObject(sphere3);
    scene2->addObject(sphere4);
    
    glm::mat4 M = glm::mat4(1.0f);
    float alpha = 0.0f;

    Rotate* rotation = new Rotate(0.0f, glm::vec3(0.0f, 1.0f, 1.0f));
    sphere1->addTransformation(rotation);
    sphere2->addTransformation(rotation);
    sphere3->addTransformation(rotation);
    sphere4->addTransformation(rotation);


    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    camera.addObserver(shaderProgram);

    float lastFrame = glfwGetTime();

    

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Pohyb WSAD
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_W, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_S, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_A, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_D, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        
        triangleObject->addTransformation(new Rotate(0.01f, glm::vec3(0.0f, 0.0f, 1.0f)));
		
		

		alpha += 0.01f;
        rotation->setAngle(alpha);



        
        shaderProgram->SetUniform("viewMatrix", camera.getViewMatrix());
        shaderProgram->SetUniform("projectMatrix", glm::perspective(45.0f, 1024.f / 800.f, 0.1f, 100.0f));



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
