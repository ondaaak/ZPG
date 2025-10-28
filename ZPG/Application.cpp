
#include "Application.h"


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
    case 4:
        activeScene = scene4;
        printf("Switched to Scene 4\n");
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
    if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS) app->switchScene(4);
}

Application::Application()
	: window(nullptr), scene1(nullptr), scene2(nullptr), scene3(nullptr), scene4(nullptr), activeScene(nullptr)
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
    scene4 = new Scene();
    activeScene = scene1;


    ShaderProgram* forestShaderProgram = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));
    ShaderProgram* groundShaderProgram = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));
    ShaderProgram* sphereProgram1 = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));
    ShaderProgram* sphereProgram2 = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));
    ShaderProgram* sphereProgram3 = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));
    ShaderProgram* sphereProgram4 = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));
    ShaderProgram* forestLightShaderProgram = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("constant_fragment_shader.glsl"));
    ShaderProgram* solarProgram = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));


    Camera camera;
    Controller controller(&camera, window);
    glfwSetWindowUserPointer(window, &controller);
    
    Model* triangleModel = new Model(triangle, sizeof(triangle) / sizeof(float) / 3, false);
    Model* sphereModel = new Model(sphere, sizeof(sphere) / sizeof(float) / 6, true);
    Model* giftModel = new Model(gift, sizeof(gift) / sizeof(float) / 6, true);
    Model* treeModel = new Model(tree, sizeof(tree) / sizeof(float) / 6, true);
    Model* bushModel = new Model(bushes, sizeof(bushes) / sizeof(float) / 6, true);
    Model* plainModel = new Model(plain, sizeof(plain) / sizeof(float) / 6, true);
	Model* catModel = new Model("12221_Cat_v1_l3.obj");

	DrawableObject* catObject = new DrawableObject(catModel, forestShaderProgram);

    catObject->addTransformation(new Translate(glm::vec3(0.0f, -0.2f, 0.0f)));
	catObject->addTransformation(new Scale(glm::vec3(0.005f, 0.005f, 0.005f)));
	catObject->addTransformation(new Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    DrawableObject* triangleObject = new DrawableObject(triangleModel, forestShaderProgram);
    DrawableObject* plainObject = new DrawableObject(plainModel, groundShaderProgram);
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
    
    Light forestLight1(glm::vec3(2.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    Light forestLight2(glm::vec3(-2.0f, 0.2f, 1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    std::vector<Light> forestLights = { forestLight1 , forestLight2 };

    DrawableObject* forestSphere1 = new DrawableObject(sphereModel, forestLightShaderProgram);
    DrawableObject* forestSphere2 = new DrawableObject(sphereModel, forestLightShaderProgram);

    Translate* forestSphere1Translate = new Translate(forestLight1.getPosition());
    forestSphere1->addTransformation(forestSphere1Translate);

    Translate* forestSphere2Translate = new Translate(forestLight2.getPosition());
    forestSphere2->addTransformation(forestSphere2Translate);



	forestSphere1->addTransformation(new Scale(glm::vec3(0.01f)));
    forestSphere2->addTransformation(new Scale(glm::vec3(0.01f)));



    Light light1 (glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Light light2 (glm::vec3(0.0f, -3.0f, 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    std::vector<Light> lights = { light1 };
	printf("Number of lights: %zu\n", lights.size());

    
	sphereProgram1->setLightUniforms(lights);
	sphereProgram2->setLightUniforms(lights);
	sphereProgram3->setLightUniforms(lights);
	sphereProgram4->setLightUniforms(lights);
    

    

   
    camera.addObserver(forestShaderProgram);
    camera.addObserver(groundShaderProgram);
	camera.addObserver(forestLightShaderProgram);
	camera.addObserver(sphereProgram1);
	camera.addObserver(sphereProgram2);
	camera.addObserver(sphereProgram3);
	camera.addObserver(sphereProgram4);


    /* fix
    for (Light& light : forestLights) {
        light.addObserver(forestShaderProgram);
        light.addObserver(groundShaderProgram);

    }*/
    
    float randomX, randomZ;
    float alpha = 0.0f;
	srand((unsigned int)time(NULL));
   
    // trees
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
		randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(treeModel, forestShaderProgram);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.1f)));
        obj->addTransformation(new Rotate(i * 0.2f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }

    //bushes
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
        randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(bushModel, forestShaderProgram);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.5f)));
        obj->addTransformation(new Rotate(i * 0.7f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }

    DrawableObject* slunce = new DrawableObject(sphereModel, sphereProgram1);
    DrawableObject* zeme = new DrawableObject(sphereModel, sphereProgram3);
    DrawableObject* mesic = new DrawableObject(sphereModel, sphereProgram3);

    slunce->addTransformation(new Scale(glm::vec3(0.5f, 0.5f, 0.5f)));

    Light* sunLight = new Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

   

    Rotate* earthOrbitRotation = new Rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    Translate* earthOrbitTranslation = new Translate(glm::vec3(3.0f, 0.0f, 0.0f));

    zeme->addTransformation(earthOrbitRotation);
    zeme->addTransformation(earthOrbitTranslation);
    zeme->addTransformation(new Scale(glm::vec3(0.3f, 0.3f, 0.3f)));

    Rotate* moonOrbitRotation = new Rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)); 
    Translate* moonOrbitTranslation = new Translate(glm::vec3(0.8f, 0.0f, 0.0f)); 

    mesic->addTransformation(earthOrbitRotation);
    mesic->addTransformation(earthOrbitTranslation);
    mesic->addTransformation(moonOrbitRotation);
    mesic->addTransformation(moonOrbitTranslation);
    mesic->addTransformation(new Scale(glm::vec3(0.1f, 0.1f, 0.1f)));

    scene1->addObject(triangleObject);
	scene2->addObject(sphere1);
    scene2->addObject(sphere2);
    scene2->addObject(sphere3);
    scene2->addObject(sphere4);
    scene3->addObject(plainObject);
	scene3->addObject(forestSphere1);
	scene3->addObject(forestSphere2);
	scene3->addObject(catObject);
	scene4->addObject(slunce);
	scene4->addObject(zeme);
	scene4->addObject(mesic);

    float lastFrame = glfwGetTime();
    float earthAngle = 0.0f;
    float moonAngle = 0.0f;

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
        
		earthAngle += 0.005f;
		moonAngle += 0.01f;  


   
        float dx = ((rand() / (float)RAND_MAX) - 0.5f) * 0.05f; 
        float dy = ((rand() / (float)RAND_MAX) - 0.5f) * 0.02f; 
        float dz = ((rand() / (float)RAND_MAX) - 0.5f) * 0.05f;


        glm::vec3 pos = forestLight1.getPosition();
        pos.x += dx;
        pos.y += dy;
        pos.z += dz;

        
        if (pos.y < 0.1f) pos.y = 0.1f;
        if (pos.y > 1.0f) pos.y = 1.0f;

        forestLight1.setPosition(pos);


        pos = forestLight2.getPosition();
        pos.x += dx;
        pos.y += dy;
        pos.z += dz;

        if (pos.y < 0.1f) pos.y = 0.1f;
        if (pos.y > 1.0f) pos.y = 1.0f;

        forestLight2.setPosition(pos);


		forestSphere1Translate->setOffset(forestLight1.getPosition());
		forestSphere2Translate->setOffset(forestLight2.getPosition());



        
        forestLights[0] = forestLight1;
        forestLights[1] = forestLight2;
        forestShaderProgram->setLightUniforms(forestLights);
        groundShaderProgram->setLightUniforms(forestLights);

        earthOrbitRotation->setAngle(earthAngle);
        moonOrbitRotation->setAngle(moonAngle);

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
    delete scene4;
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
