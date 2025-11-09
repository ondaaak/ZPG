#include "Application.h"
#include "Skybox.h" // PŘIDÁNO

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


float triangle[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

const float plain[] = {
    1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
   -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,

   -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
   -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f
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
    flashlight(nullptr), isFlashlightOn(true), fKeyPressedLastFrame(false), skybox(nullptr)
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

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

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


    ShaderProgram* phongShaderProgram = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_fragment_shader.glsl"));
    ShaderProgram* spheresProgram = new ShaderProgram(std::string("main_vertex_shader.glsl"), std::string("phong_simple.glsl"));

    // Nastavíme výchozí hodnoty pro phong shader
    phongShaderProgram->setShaderProgram();
    phongShaderProgram->SetUniform("textureSampler", 0);
    phongShaderProgram->SetUniform("useTexture", 0);

    Material white;
    white.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    white.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    white.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    white.shininess = 32.0f;

    Material mat_blue_plastic;
    mat_blue_plastic.ambient = glm::vec3(0.0f, 0.0f, 1.0f);
    mat_blue_plastic.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    mat_blue_plastic.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    mat_blue_plastic.shininess = 1.0f;

    Material mat_ground;
    mat_ground.ambient = glm::vec3(0.05f, 0.1f, 0.05f);
    mat_ground.diffuse = glm::vec3(0.1f, 0.4f, 0.1f);
    mat_ground.specular = glm::vec3(0.05f, 0.05f, 0.05f);
    mat_ground.shininess = 10.0f;

    Material green_forest;
    green_forest.ambient = glm::vec3(0.0f, 0.05f, 0.0f);
    green_forest.diffuse = glm::vec3(0.0f, 0.5f, 0.0f);
    green_forest.specular = glm::vec3(0.05f, 0.05f, 0.05f);
    green_forest.shininess = 8.0f;

    Material mat_firefly_white;
    mat_firefly_white.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    mat_firefly_white.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    mat_firefly_white.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    mat_firefly_white.shininess = 1.0f;

    Material sun;
    sun.ambient = glm::vec3(0.6f, 0.4f, 0.0f);
    sun.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.shininess = 1.0f;

    Material moon;
    moon.ambient = glm::vec3(0.4f, 0.4f, 0.4f);
    moon.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    moon.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    moon.shininess = 32.0f;

    Material earth;
    earth.ambient = glm::vec3(0.0f, 0.4f, 0.0f);
    earth.diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
    earth.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    earth.shininess = 32.0f;

    Material basic;
    basic.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    basic.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    basic.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    basic.shininess = 32.0f;


    Camera camera;
    Controller controller(&camera, window);
    glfwSetWindowUserPointer(window, &controller);


    Model* triangleModel = new Model(triangle, sizeof(triangle) / sizeof(float) / 3, 0);
    Model* sphereModel = new Model(sphere, sizeof(sphere) / sizeof(float) / 6, 1);
    Model* giftModel = new Model(gift, sizeof(gift) / sizeof(float) / 6, 1);
    Model* treeModel = new Model(tree, sizeof(tree) / sizeof(float) / 6, 1);
    Model* bushModel = new Model(bushes, sizeof(bushes) / sizeof(float) / 6, 1);
    Model* foxModel = new Model("13577_Tibetan_Hill_Fox_v1_L3.obj");
    Model* catModel = new Model("12221_Cat_v1_l3.obj");
    Model* shrekModel = new Model("shrek.obj");
    Model* fionaModel = new Model("fiona.obj");

    Texture* grassTexture = new Texture("../assets/grass.png");
    Texture* catTexture = new Texture("../assets/Cat_diffuse.jpg");
    Texture* foxTexture = new Texture("../assets"
        "/Tibetan_Hill_Fox_dif.jpg");
    Texture* shrekTexture = new Texture("../assets/shrek.png");
    Texture* fionaTexture = new Texture("../assets/fiona.png");

    std::vector<std::string> faces = {
        "../assets/posx.jpg",
        "../assets/negx.jpg",
        "../assets/posy.jpg",
        "../assets/negy.jpg",
        "../assets"
        "/posz.jpg",
        "../assets/negz.jpg"
    };
    skybox = new Skybox(faces);


    DrawableObject* catObject = new DrawableObject(catModel, phongShaderProgram, white, catTexture);
    DrawableObject* foxObject = new DrawableObject(foxModel, phongShaderProgram, white, foxTexture);
    DrawableObject* shrekObject = new DrawableObject(shrekModel, phongShaderProgram, white, shrekTexture);
    DrawableObject* fionaObject = new DrawableObject(fionaModel, phongShaderProgram, white, fionaTexture);


    catObject->addTransformation(new Scale(glm::vec3(0.005f, 0.005f, 0.005f)));
    catObject->addTransformation(new Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    catObject->addTransformation(new Translate(glm::vec3(-3.0f, 0.0f, 5.0f)));


    foxObject->addTransformation(new Translate(glm::vec3(1.0f, 0.0f, 0.5f)));
    foxObject->addTransformation(new Scale(glm::vec3(0.0025f, 0.0025f, 0.0025f)));
    foxObject->addTransformation(new Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    DrawableObject* triangleObject = new DrawableObject(triangleModel, phongShaderProgram, mat_blue_plastic);

    DrawableObject* sphere1 = new DrawableObject(sphereModel, spheresProgram, white);
    DrawableObject* sphere2 = new DrawableObject(sphereModel, spheresProgram, white);
    DrawableObject* sphere3 = new DrawableObject(sphereModel, spheresProgram, white);
    DrawableObject* sphere4 = new DrawableObject(sphereModel, spheresProgram, white);

    Rotate* rotation = new Rotate(0.0f, glm::vec3(0.0f, 1.0f, 1.0f));
    Rotate* rotation2 = new Rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));


    sphere1->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere2->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere3->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere4->addTransformation(new Scale(glm::vec3(0.2f, 0.2f, 0.2f)));
    sphere1->addTransformation(new Translate(glm::vec3(0.0f, 2.5f, 0.0f)));
    sphere2->addTransformation(new Translate(glm::vec3(2.5f, 0.0f, 0.0f)));
    sphere3->addTransformation(new Translate(glm::vec3(0.0f, -2.5f, 0.0f)));
    sphere4->addTransformation(new Translate(glm::vec3(-2.5f, 0.0f, 0.0f)));
    triangleObject->addTransformation(rotation2);


    Light* forestLight1_ptr = new Light(glm::vec3(2.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    Light* forestLight2_ptr = new Light(glm::vec3(-2.0f, 0.2f, 1.0f), glm::vec3(0.0f, 0.5f, 0.0f));

    //DirectionalLight* dirLight = new DirectionalLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(4.3f, 0.3f, 0.3f));

    flashlight = new SpotLight(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::radians(20.0f), glm::radians(30.0f));

    flashlightDiffuseColor = flashlight->getDiffuse();
    flashlightSpecularColor = flashlight->getSpecular();


    scene3Lights.push_back(forestLight1_ptr);
    scene3Lights.push_back(forestLight2_ptr);
    scene3Lights.push_back(flashlight);



    DrawableObject* firefly1 = new DrawableObject(sphereModel, phongShaderProgram, mat_firefly_white);
    DrawableObject* firefly2 = new DrawableObject(sphereModel, phongShaderProgram, mat_firefly_white);


    Translate* forestSphere1Translate = new Translate(forestLight1_ptr->getPosition());
    firefly1->addTransformation(forestSphere1Translate);

    Translate* forestSphere2Translate = new Translate(forestLight2_ptr->getPosition());
    firefly2->addTransformation(forestSphere2Translate);

    firefly1->addTransformation(new Scale(glm::vec3(0.005f)));
    firefly2->addTransformation(new Scale(glm::vec3(0.005f)));



    camera.addObserver(phongShaderProgram);
    camera.addObserver(spheresProgram);
    camera.addObserver(skybox->getShader());



    Light* light1_ptr = new Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    scene2Lights.push_back(light1_ptr);

    for (Light* light : scene2Lights) {
        light->addObserver(spheresProgram);
    }
    for (Light* light : scene3Lights) {
        light->addObserver(phongShaderProgram);
    }



    float randomX, randomZ;
    float alpha = 0.0f;
    srand((unsigned int)time(NULL));

    // trees
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
        randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(treeModel, phongShaderProgram, green_forest);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.1f)));
        obj->addTransformation(new Rotate(i * 0.2f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }

    //bushes
    for (int i = 0; i < 50; ++i) {
        randomX = rand() % (5 + 5 + 1) - 5;
        randomZ = rand() % (5 + 5 + 1) - 5;
        DrawableObject* obj = new DrawableObject(bushModel, phongShaderProgram, green_forest);
        obj->addTransformation(new Translate(glm::vec3(randomX, 0.0f, randomZ)));
        obj->addTransformation(new Scale(glm::vec3(0.5f)));
        obj->addTransformation(new Rotate(i * 0.7f, glm::vec3(0, 1, 0)));
        scene3->addObject(obj);
    }


    DrawableObject* slunce = new DrawableObject(sphereModel, phongShaderProgram, sun);
    DrawableObject* zeme = new DrawableObject(sphereModel, phongShaderProgram, earth);
    DrawableObject* mesic = new DrawableObject(sphereModel, phongShaderProgram, moon);

    slunce->addTransformation(new Scale(glm::vec3(0.5f, 0.5f, 0.5f)));

    Light* sunLight = new Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    scene4Lights.push_back(sunLight);
    for (Light* light : scene4Lights) {
        light->addObserver(phongShaderProgram);
    }

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


    Model* grassModel = new Model(plain, sizeof(plain) / sizeof(float) / 8, 2);

    DrawableObject* grassObject = new DrawableObject(grassModel, phongShaderProgram, basic, grassTexture);
    grassObject->addTransformation(new Scale(glm::vec3(5.5f, 1.0f, 5.5f)));

    shrekObject->addTransformation(new Scale(glm::vec3(0.3f, 0.3f, 0.3f)));
    fionaObject->addTransformation(new Scale(glm::vec3(0.3f, 0.3f, 0.3f)));

    shrekObject->addTransformation(new Translate(glm::vec3(1.0f, 0.0f, 1.2f)));
    fionaObject->addTransformation(new Translate(glm::vec3(2.8f, 0.0f, 1.5f)));


    scene3->addObject(grassObject);
    scene1->addObject(triangleObject);

    scene2->addObject(sphere1);
    scene2->addObject(sphere2);
    scene2->addObject(sphere3);
    scene2->addObject(sphere4);

    scene3->addObject(firefly1);
    scene3->addObject(firefly2);
    scene3->addObject(catObject);
    scene3->addObject(foxObject);
    scene3->addObject(shrekObject);
    scene3->addObject(fionaObject);

    scene4->addObject(slunce);
    scene4->addObject(zeme);
    scene4->addObject(mesic);

    scene3->setSkybox(skybox);

    float lastFrame = glfwGetTime();
    float earthAngle = 0.0f;
    float moonAngle = 0.0f;

    phongShaderProgram->setLightUniforms(scene3Lights);
    phongShaderProgram->setLightsPointer(&scene3Lights);
    spheresProgram->setLightsPointer(&scene2Lights);
    spheresProgram->setLightUniforms(scene2Lights);

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

        if (activeScene == scene1) {
            std::vector<Light*> emptyLights;
            phongShaderProgram->setLightsPointer(&emptyLights);
            phongShaderProgram->setLightUniforms(emptyLights);
        }
        else if (activeScene == scene2) {
            spheresProgram->setLightsPointer(&scene2Lights);
            spheresProgram->setLightUniforms(scene2Lights);
        }
        else if (activeScene == scene3) {
            phongShaderProgram->setLightsPointer(&scene3Lights);
            phongShaderProgram->setLightUniforms(scene3Lights);

            // Logika baterky
            bool fKeyPressed = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
            if (fKeyPressed && !fKeyPressedLastFrame) {
                isFlashlightOn = !isFlashlightOn;
                if (isFlashlightOn) {
                    flashlight->setDiffuse(flashlightDiffuseColor);
                    flashlight->setSpecular(flashlightSpecularColor);
                }
                else {
                    flashlight->setDiffuse(glm::vec3(0.0f));
                    flashlight->setSpecular(glm::vec3(0.0f));
                }
            }
            fKeyPressedLastFrame = fKeyPressed;

            // Pohyb světlušek
            float dx = ((rand() / (float)RAND_MAX) - 0.5f) * 0.02f;
            float dy = ((rand() / (float)RAND_MAX) - 0.5f) * 0.01f;
            float dz = ((rand() / (float)RAND_MAX) - 0.5f) * 0.02f;

            glm::vec3 pos = forestLight1_ptr->getPosition();
            pos.x += dx; pos.y += dy; pos.z += dz;
            if (pos.y < 0.1f) pos.y = 0.1f; if (pos.y > 1.0f) pos.y = 1.0f;
            forestLight1_ptr->setPosition(pos);

            pos = forestLight2_ptr->getPosition();
            pos.x += dx; pos.y += dy; pos.z += dz;
            if (pos.y < 0.1f) pos.y = 0.1f; if (pos.y > 1.0f) pos.y = 1.0f;
            forestLight2_ptr->setPosition(pos);

            forestSphere1Translate->setOffset(forestLight1_ptr->getPosition());
            forestSphere2Translate->setOffset(forestLight2_ptr->getPosition());

            flashlight->setPosition(camera.getCameraPosition());
            flashlight->setDirection(camera.getCameraFront());

        }
        else if (activeScene == scene4) {
            earthAngle += 0.005f;
            moonAngle += 0.01f;
            earthOrbitRotation->setAngle(earthAngle);
            moonOrbitRotation->setAngle(moonAngle);

            phongShaderProgram->setLightsPointer(&scene4Lights);
            phongShaderProgram->setLightUniforms(scene4Lights);
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        float ratio = width / (float)height;
        camera.setAspectRatio(ratio);

        // ZMĚNA POŘADÍ VYKRESLOVÁNÍ
        // 1. Vykreslíme scénu
        if (activeScene) activeScene->render();


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // --- TADY BYLA CHYBA (DOUBLE FREE) ---
    // Tyto delete příkazy jsou smazány, protože 
    // mazání probíhá ve funkci cleanup()
    //
    // delete grassTexture;
    // delete catTexture; 
    // delete foxTexture; 
    // delete shrekTexture; 
    // delete fionaTexture; 
    // delete skybox; 
    // ------------------------------------
}

void Application::cleanup() {
    // Tady je správné místo pro mazání

    // Nejprve smažeme scény, které mažou DrawableObjects
    delete scene1;
    delete scene2;
    delete scene3;
    delete scene4;

    // Smažeme skybox
    if (skybox) {
        delete skybox;
        skybox = nullptr;
    }

    // Pak smažeme světla (pokud je nevlastní scény, což nevlastní)
    for (Light* light : scene2Lights) {
        delete light;
    }
    scene2Lights.clear();

    for (Light* light : scene3Lights) {
        delete light;
    }
    scene3Lights.clear();

    for (Light* light : scene4Lights) {
        delete light;
    }
    scene4Lights.clear();

    // Textury a modely by se měly mazat ideálně zde,
    // ale tvůj kód je nevlastní v Application,
    // jsou vlastněny jednotlivými DrawableObjects (nebo by měly být)
    // Pro jednoduchost (a aby odpovídal tvému kódu) je zde nemažu,
    // ale pamatuj, že na konci run() je špatné místo.
    // Destruktor ~Application volá cleanup(), to je jediné správné místo.


    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}