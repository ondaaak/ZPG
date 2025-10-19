
#include "Application.h"


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

    Shader* vertexShader = new Shader(GL_VERTEX_SHADER, std::string("main_vertex_shader.glsl"));
    Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, std::string("fragment_shader.glsl"));
    Shader* constFragmentShader = new Shader(GL_FRAGMENT_SHADER, std::string("constant_fragment_shader.glsl"));
    Shader* lambertFragmentShader = new Shader(GL_FRAGMENT_SHADER, std::string("lambert_fragment_shader.glsl"));
    Shader* phongFragmentShader = new Shader(GL_FRAGMENT_SHADER, std::string("phong_fragment_shader.glsl"));
    Shader* blinnPhongFragmentShader = new Shader(GL_FRAGMENT_SHADER, std::string("blinn-phong_fragment_shader.glsl"));
    Shader* fragmentShader2 = new Shader(GL_FRAGMENT_SHADER, std::string("plane_fragment_shader.glsl"));

    ShaderProgram* shaderProgram = new ShaderProgram(*vertexShader, *fragmentShader);
    ShaderProgram* shaderProgram2 = new ShaderProgram(*vertexShader, *fragmentShader2);

    ShaderProgram* sphereProgram1 = new ShaderProgram(*vertexShader,*constFragmentShader);
    ShaderProgram* sphereProgram2 = new ShaderProgram(*vertexShader, *lambertFragmentShader);
    ShaderProgram* sphereProgram3 = new ShaderProgram(*vertexShader, *phongFragmentShader);
    ShaderProgram* sphereProgram4 = new ShaderProgram(*vertexShader, *blinnPhongFragmentShader);


    Camera camera;
    Controller controller(&camera, window);
    glfwSetWindowUserPointer(window, &controller);
    
    Model* triangleModel = new Model(triangle, sizeof(triangle) / sizeof(float) / 3, false);
    Model* sphereModel = new Model(sphere, sizeof(sphere) / sizeof(float) / 6, true);
    Model* giftModel = new Model(gift, sizeof(gift) / sizeof(float) / 6, true);
    Model* treeModel = new Model(tree, sizeof(tree) / sizeof(float) / 6, true);
    Model* bushModel = new Model(bushes, sizeof(bushes) / sizeof(float) / 6, true);
    Model* plainModel = new Model(plain, sizeof(plain) / sizeof(float) / 6, true);

    DrawableObject* triangleObject = new DrawableObject(triangleModel, shaderProgram);
    DrawableObject* plainObject = new DrawableObject(plainModel, shaderProgram2);
    DrawableObject* sphere1 = new DrawableObject(sphereModel, sphereProgram1);
    DrawableObject* sphere2 = new DrawableObject(sphereModel, sphereProgram2);
    DrawableObject* sphere3 = new DrawableObject(sphereModel, sphereProgram3);
    DrawableObject* sphere4 = new DrawableObject(sphereModel, sphereProgram4);

    Rotate* rotation = new Rotate(0.0f, glm::vec3(0.0f, 1.0f, 1.0f));
    Rotate* rotation2 = new Rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    plainObject->addTransformation(new Scale(glm::vec3(5.5f, 1.0f, 5.5f)));
    plainObject->addTransformation(new Translate(glm::vec3(0.0f, -0.01f, 0.0f)));
    sphere1->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere2->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere3->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere4->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere1->addTransformation(new Translate(glm::vec3(0.0f, 2.5f, 0.0f)));
    sphere2->addTransformation(new Translate(glm::vec3(2.5f, 0.0f, 0.0f)));
    sphere3->addTransformation(new Translate(glm::vec3(0.0f, -2.5f, 0.0f)));
    sphere4->addTransformation(new Translate(glm::vec3(-2.5f, 0.0f, 0.0f)));
/*
    sphere1->addTransformation(rotation);
    sphere2->addTransformation(rotation);
    sphere3->addTransformation(rotation);
    sphere4->addTransformation(rotation);*/
    triangleObject->addTransformation(rotation2);

    Light* light = new Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    

    sphereProgram2->SetUniform("lightPos", light->getPosition());
    sphereProgram3->SetUniform("lightPos", light->getPosition());
    sphereProgram4->SetUniform("lightPos", light->getPosition());
    

	sphereProgram3->setLight(true);
	sphereProgram4->setLight(true);



    camera.addObserver(shaderProgram);
    camera.addObserver(shaderProgram2);

	camera.addObserver(sphereProgram1);
	camera.addObserver(sphereProgram2);
	camera.addObserver(sphereProgram3);
	camera.addObserver(sphereProgram4);

    
    float randomX, randomZ;
    float alpha = 0.0f;
	srand((unsigned int)time(NULL));
   
    // trees
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
		randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(treeModel, shaderProgram);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.1f)));
        obj->addTransformation(new Rotate(i * 0.2f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }

    //bushes
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
        randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(bushModel, shaderProgram);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.5f)));
        obj->addTransformation(new Rotate(i * 0.7f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }
	
    scene1->addObject(triangleObject);
	scene2->addObject(sphere1);
    scene2->addObject(sphere2);
    scene2->addObject(sphere3);
    scene2->addObject(sphere4);
    scene3->addObject(plainObject);



    float lastFrame = glfwGetTime();
	

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        controller.processInput(deltaTime);

		alpha += 0.01f;
        rotation->setAngle(alpha);
        rotation2->setAngle(alpha);



        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        float ratio = width / (float)height;
		camera.setAspectRatio(ratio);
        
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
