
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
    : window(nullptr), scene1(nullptr), scene2(nullptr), scene3(nullptr), scene4(nullptr), activeScene(nullptr),
    sunLight(nullptr), flashlight(nullptr), isFlashlightOn(true), fKeyPressedLastFrame(false)
{
}
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

    // ... (vytváøení shader programù zùstává stejné) ...
    ShaderProgram* triangleShaderProgram = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("constant_fragment_shader.glsl"));
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

    // ... (vytváøení modelù a objektù zùstává stejné) ...
    Model* triangleModel = new Model(triangle, sizeof(triangle) / sizeof(float) / 3, false);
    Model* sphereModel = new Model(sphere, sizeof(sphere) / sizeof(float) / 6, true);
    Model* giftModel = new Model(gift, sizeof(gift) / sizeof(float) / 6, true);
    Model* treeModel = new Model(tree, sizeof(tree) / sizeof(float) / 6, true);
    Model* bushModel = new Model(bushes, sizeof(bushes) / sizeof(float) / 6, true);
    Model* plainModel = new Model(plain, sizeof(plain) / sizeof(float) / 6, true);
    Model* bearModel = new Model("13577_Tibetan_Hill_Fox_v1_L3.obj");
    Model* catModel = new Model("12221_Cat_v1_l3.obj");

    DrawableObject* catObject = new DrawableObject(catModel, forestShaderProgram);
    DrawableObject* foxObject = new DrawableObject(bearModel, forestShaderProgram);

    //catObject->addTransformation(new Translate(glm::vec3(0.0f, 0.2f, 0.0f)));
    catObject->addTransformation(new Scale(glm::vec3(0.005f, 0.005f, 0.005f)));
    catObject->addTransformation(new Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    foxObject->addTransformation(new Translate(glm::vec3(1.0f, 0.0f, 0.5f)));
    foxObject->addTransformation(new Scale(glm::vec3(0.0025f, 0.0025f, 0.0025f)));
    foxObject->addTransformation(new Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    DrawableObject* triangleObject = new DrawableObject(triangleModel, triangleShaderProgram);
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
    triangleObject->addTransformation(rotation2);

    // ZMÌNA: Vytváøení svìtel pro scénu 3
    Light* forestLight1_ptr = new Light(glm::vec3(2.0f, 0.2f, 0.0f), glm::vec3(0.1f, 0.5f, 0.1f));
    Light* forestLight2_ptr = new Light(glm::vec3(-2.0f, 0.2f, 1.0f), glm::vec3(0.1f, 0.5f, 0.1f));

    // ZMÌNA: Vytvoøíme baterku a uložíme ji do èlenské promìnné 'flashlight'
    // Používáme opravený konstruktor - posíláme úhel 50 stupòù
    flashlight = new SpotLight(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.7f, 0.7f, 0.7f),
        glm::radians(20.0f), glm::radians(30.0f));
    // PØIDÁNO: Uložíme si pùvodní barvy baterky
    flashlightDiffuseColor = flashlight->getDiffuse();
    flashlightSpecularColor = flashlight->getSpecular();


    // Pøidáme všechna svìtla do seznamu scény 3
    scene3Lights.push_back(forestLight1_ptr);
    scene3Lights.push_back(forestLight2_ptr);
    scene3Lights.push_back(flashlight); // Pøidáme baterku do seznamu
	


    DrawableObject* forestSphere1 = new DrawableObject(sphereModel, forestLightShaderProgram);
    DrawableObject* forestSphere2 = new DrawableObject(sphereModel, forestLightShaderProgram);

    // Používáme ukazatele
    Translate* forestSphere1Translate = new Translate(forestLight1_ptr->getPosition());
    forestSphere1->addTransformation(forestSphere1Translate);

    Translate* forestSphere2Translate = new Translate(forestLight2_ptr->getPosition());
    forestSphere2->addTransformation(forestSphere2Translate);

    forestSphere1->addTransformation(new Scale(glm::vec3(0.005f)));
    forestSphere2->addTransformation(new Scale(glm::vec3(0.005f)));

    // ZMÌNA: Vytváøení svìtel na haldì (new) a ukládání ukazatelù
    Light* light1_ptr = new Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Light* light2_ptr = new Light(glm::vec3(0.0f, -3.0f, 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // ZMÌNA: Plníme èlenský vektor scene2Lights
    scene2Lights.push_back(light1_ptr);
    scene2Lights.push_back(light2_ptr); // Pøidáme i druhé svìtlo, i když nebylo použito
    printf("Number of lights: %zu\n", scene2Lights.size());


    // ZMÌNA: Nastavení svìtel jednou pøi inicializaci
    sphereProgram1->setLightUniforms(scene2Lights);
    sphereProgram2->setLightUniforms(scene2Lights);
    sphereProgram3->setLightUniforms(scene2Lights);
    sphereProgram4->setLightUniforms(scene2Lights);

    // Toto je pro scénu 3
    forestShaderProgram->setLightUniforms(scene3Lights);
    groundShaderProgram->setLightUniforms(scene3Lights);


    // Registrace observerù pro kameru
    camera.addObserver(forestShaderProgram);
    camera.addObserver(groundShaderProgram);
    camera.addObserver(forestLightShaderProgram);
    camera.addObserver(sphereProgram1);
    camera.addObserver(sphereProgram2);
    camera.addObserver(sphereProgram3);
    camera.addObserver(sphereProgram4);
    camera.addObserver(solarProgram);
    camera.addObserver(triangleShaderProgram);

    // OPRAVA: Odkomentováno a upraveno pro nové vektory ukazatelù
    // Registrace observerù pro svìtla
    for (Light* light : scene2Lights) {
        light->addObserver(sphereProgram1);
        light->addObserver(sphereProgram2);
        light->addObserver(sphereProgram3);
        light->addObserver(sphereProgram4);
    }
    for (Light* light : scene3Lights) {
        light->addObserver(forestShaderProgram);
        light->addObserver(groundShaderProgram);
    }

    // PØIDÁNO: Musíme shaderùm øíct, který seznam mají sledovat
    sphereProgram1->setLightsPointer(&scene2Lights);
    sphereProgram2->setLightsPointer(&scene2Lights);
    sphereProgram3->setLightsPointer(&scene2Lights);
    sphereProgram4->setLightsPointer(&scene2Lights);
    forestShaderProgram->setLightsPointer(&scene3Lights);
    groundShaderProgram->setLightsPointer(&scene3Lights);


    float randomX, randomZ;
    float alpha = 0.0f;
    srand((unsigned int)time(NULL));

    // ... (generování scény 3 zùstává stejné) ...
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

    // ... (scéna 4 - sluneèní soustava) ...
    DrawableObject* slunce = new DrawableObject(sphereModel, sphereProgram1);
    DrawableObject* zeme = new DrawableObject(sphereModel, sphereProgram3);
    DrawableObject* mesic = new DrawableObject(sphereModel, sphereProgram3);

    slunce->addTransformation(new Scale(glm::vec3(0.5f, 0.5f, 0.5f)));

    // ZMÌNA: Uložíme do èlenské promìnné
    sunLight = new Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

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
    scene3->addObject(foxObject);
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

        // Zpracování pohybu kamery
        controller.processInput(deltaTime);

        // --- PØIDÁNO: Logika pro pøepínání baterky ---
        // Zjistíme, jestli je F stisknuto teï
        bool fKeyPressed = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);

        // Zareagujeme jen na "hranu" - když je klávesa stisknutá teï, ale nebyla minule
        if (fKeyPressed && !fKeyPressedLastFrame) {
            isFlashlightOn = !isFlashlightOn; // Pøepneme stav

            if (isFlashlightOn) {
                // Zapnout: vrátíme pùvodní barvy
                // (notifyObservers se zavolá uvnitø setDiffuse/setSpecular)
                flashlight->setDiffuse(flashlightDiffuseColor);
                flashlight->setSpecular(flashlightSpecularColor);
                printf("Baterka ZAPNUTA\n");
            }
            else {
                // Vypnout: nastavíme barvy na èernou
                flashlight->setDiffuse(glm::vec3(0.0f));
                flashlight->setSpecular(glm::vec3(0.0f));
                printf("Baterka VYPNUTA\n");
            }
        }
        fKeyPressedLastFrame = fKeyPressed; // Uložíme stav pro pøíští snímek
        // --- Konec logiky baterky ---


        alpha += 0.01f;
        rotation->setAngle(alpha);
        rotation2->setAngle(alpha);

        earthAngle += 0.005f;
        moonAngle += 0.01f;

        float dx = ((rand() / (float)RAND_MAX) - 0.5f) * 0.02f;
        float dy = ((rand() / (float)RAND_MAX) - 0.5f) * 0.01f;
        float dz = ((rand() / (float)RAND_MAX) - 0.5f) * 0.02f;


        glm::vec3 pos = forestLight1_ptr->getPosition();
        pos.x += dx;
        pos.y += dy;
        pos.z += dz;
        if (pos.y < 0.1f) pos.y = 0.1f;
        if (pos.y > 1.0f) pos.y = 1.0f;
        forestLight1_ptr->setPosition(pos);


        pos = forestLight2_ptr->getPosition();
        pos.x += dx;
        pos.y += dy;
        pos.z += dz;
        if (pos.y < 0.1f) pos.y = 0.1f;
        if (pos.y > 1.0f) pos.y = 1.0f;
        forestLight2_ptr->setPosition(pos); 


        forestSphere1Translate->setOffset(forestLight1_ptr->getPosition());
        forestSphere2Translate->setOffset(forestLight2_ptr->getPosition());

        flashlight->setPosition(camera.getCameraPosition());
        flashlight->setDirection(camera.getCameraFront());

        

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

    // Pamatuj, že 'flashlight' je také v 'scene3Lights',
    // takže bude smazán zde. Není potøeba ho mazat dvakrát.
    for (Light* light : scene2Lights) {
        delete light;
    }
    scene2Lights.clear();

    for (Light* light : scene3Lights) {
        delete light;
    }
    scene3Lights.clear();

    delete sunLight;
    sunLight = nullptr;

    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}