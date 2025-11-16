#pragma once
//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include "Translate.h"
#include "Scale.h"
#include "Rotate.h"
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
#include "Camera.h"
#include <Time.h>
#include "Controller.h"
#include "Light.h"
#include "SpotLight.h"
#include <vector>
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Material.h"
#include "Texture.h"
#include "Skybox.h" 

class Application {
public:
    Application();
    ~Application();
    void run();
    void switchScene(int sceneNumber);
    // --- PØIDAT TUTO METODU ---
    Scene* getActiveScene() { return activeScene; }

    int currentId;

private:
    GLFWwindow* window;
    Scene* activeScene;
    Scene* scene1;
    Scene* scene2;
    Scene* scene3;
    Scene* scene4;
    Skybox* skybox;

    std::vector<Light*> scene2Lights;
    std::vector<Light*> scene3Lights;
    std::vector<Light*> scene4Lights;

    Controller* controller;

    SpotLight* flashlight;
    glm::vec3 flashlightDiffuseColor;
    glm::vec3 flashlightSpecularColor;
    bool isFlashlightOn;
    bool fKeyPressedLastFrame;

    bool init();
    void cleanup();
};