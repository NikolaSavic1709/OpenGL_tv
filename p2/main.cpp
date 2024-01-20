//Opis: Primjer ucitavanja modela upotrebom ASSIMP biblioteke
//Preuzeto sa learnOpenGL

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"
#include "tv.hpp"
#include "screen.hpp"
#include "button.hpp"
#include "floor.hpp"
#include "nameTexture.hpp"

const unsigned int wWidth = 800;
const unsigned int wHeight = 600;
Shader modelShader;
Shader modelShader1;
Shader basicShader;
Shader phongShader;
Shader phongShader1;
Shader pointerShader;
Shader charactersShader;
Shader floorShader;
Shader nameShader;
Shader gouraudShader;

void setBasicUniforms(string uName, glm::mat4 matrix) {
    modelShader.use();
    modelShader.setMat4(uName, matrix);
    phongShader.use();
    phongShader.setMat4(uName, matrix);
    basicShader.use();
    basicShader.setMat4(uName, matrix);
    pointerShader.use();
    pointerShader.setMat4(uName, matrix);
    charactersShader.use();
    charactersShader.setMat4(uName, matrix);
    floorShader.use();
    floorShader.setMat4(uName, matrix);
    gouraudShader.use();
    gouraudShader.setMat4(uName, matrix);
}

int main()
{
    GLFWwindow* window;
    {
        if (!glfwInit())
        {
            std::cout << "GLFW fail!\n" << std::endl;
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(wWidth, wHeight, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Window fail!\n" << std::endl;
            glfwTerminate();
            return -2;
        }
        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
            std::cout << "GLEW fail! :(\n" << std::endl;
            return -3;
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    Model remoteController("res/remote-controller/remote.obj");

    floorShader = Shader("texture.vert", "texture.frag");
    modelShader1 = Shader("model.vert", "model.frag");
    modelShader = Shader("model.vert", "texture.frag");
    phongShader1 = Shader("phong.vert", "phong.frag");
    phongShader = Shader("phong.vert", "phong1.frag");
    basicShader = Shader("basic.vert", "basic.frag");
    pointerShader = Shader("pointer.vert", "basic.frag");
    charactersShader = Shader("characters.vert", "characters.frag");
    gouraudShader = Shader("gouraud.vert", "gouraud.frag");
    nameShader = Shader("basic2d.vert", "basic2d.frag");



    float cameraRadius = 30.0f;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, cameraRadius);
    glm::vec3 cameraAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraAxis);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 15.0f));
    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    glm::mat4 projectionO = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)

    Button button;
    Screen screen;
    Floor floor;
    Name name;
    Tv tv(button, screen);

    glm::mat4 tvObj = glm::mat4(1.0f);
    glm::mat4 buttonModel = glm::mat4(1.0f);
    //model v1
    {

        modelShader1.use();
        modelShader1.setVec3("uViewPos", 0, 0, 5);
        modelShader1.setVec3("uLight.pos", 0, 1, 3);
        modelShader1.setVec3("uLight.kA", 1, 1, 1);
        modelShader1.setVec3("uLight.kD", 0, 0, 0);
        modelShader1.setVec3("uLight.kS", 0, 0, 0);
        modelShader1.setMat4("uV", view);
        modelShader1.setMat4("uM", model);
        modelShader1.setMat4("uP", projectionP);
    }
    //phong v1
    {
        phongShader1.use();
        phongShader1.setVec3("uViewPos", 0, 0, 5);
        phongShader1.setVec3("uLight.pos", 0, 1, 3);
        phongShader1.setVec3("uLight.kA", 1, 1, 1);
        phongShader1.setVec3("uLight.kD", 0, 0, 0);
        phongShader1.setVec3("uLight.kS", 0, 0, 0);

        phongShader1.setFloat("uMaterial.shine", 0.4);
        phongShader1.setVec3("uMaterial.kA", 0.19225, 0.19225, 0.19225);
        phongShader1.setVec3("uMaterial.kD", 0.50754, 0.50754, 0.50754);
        phongShader1.setVec3("uMaterial.kS", 0.508273, 0.508273, 0.508273);

        phongShader1.setMat4("uV", view);
        phongShader1.setMat4("uM", glm::mat4(1.0f));
        phongShader1.setMat4("uButtonM", buttonModel);
        phongShader1.setMat4("uP", projectionP);
    }
    //model shader
    {
        modelShader.use();
        modelShader.setMat4("uV", view);
        modelShader.setMat4("uM", glm::mat4(1.0f));
        modelShader.setMat4("uP", projectionP);
        modelShader.setVec3("uViewPos", cameraPos);

        modelShader.setVec3("pointLight.position", glm::vec3(0.0, -4.8, 3.1));
        modelShader.setVec3("pointLight.ambient", glm::vec3(1.0, 0.0, 0.0));
        modelShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 0.0, 0.0));
        modelShader.setVec3("pointLight.specular", glm::vec3(1.0, 0.0, 0.0));
        modelShader.setFloat("pointLight.constant", 1.0f);
        modelShader.setFloat("pointLight.linear", 1.0f);
        modelShader.setFloat("pointLight.quadratic", 1.0f);

        modelShader.setVec3("ambientLight.position", glm::vec3(0.0, 10.0, 0.0));
        modelShader.setVec3("ambientLight.ambient", glm::vec3(0.4, 0.4, 0.4));
        modelShader.setVec3("ambientLight.diffuse", glm::vec3(0.0, 0.0, 0.0));
        modelShader.setVec3("ambientLight.specular", glm::vec3(0.0, 0.0, 0.0));
        modelShader.setFloat("ambientLight.constant", 0.01f);
        modelShader.setFloat("ambientLight.linear", 0.01f);
        modelShader.setFloat("ambientLight.quadratic", 0.01f);

        modelShader.setVec3("spotLight.position", glm::vec3(0.0, 1.0f, -2.99f));
        modelShader.setVec3("spotLight.ambient", glm::vec3(1.0, 1.0, 1.0));
        modelShader.setVec3("spotLight.diffuse", glm::vec3(1.0, 0.8, 0.6));
        modelShader.setVec3("spotLight.specular", glm::vec3(1.0, 1.0, 1.0));
        modelShader.setFloat("spotLight.constant", 0.1f);
        modelShader.setFloat("spotLight.linear", 0.01f);
        modelShader.setFloat("spotLight.quadratic", 0.01f);
        modelShader.setVec3("spotLight.direction", glm::vec3(0.0f, 0.0f, 1.0f));
        modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(40.0f)));
        modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(60.0f)));

    }
    //phong shader (tv)
    {
        phongShader.use();
        phongShader.setMat4("uV", view);
        phongShader.setMat4("uM", glm::mat4(1.0f));
        phongShader.setMat4("uButtonM", buttonModel);
        phongShader.setMat4("uP", projectionP);
        phongShader.setVec3("uViewPos", cameraPos);

        phongShader.setVec3("pointLight.position", glm::vec3(0.0, -4.8, 3.1));
        phongShader.setVec3("pointLight.ambient", glm::vec3(1.0, 0.0, 0.0));
        phongShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 0.0, 0.0));
        phongShader.setVec3("pointLight.specular", glm::vec3(1.0, 0.0, 0.0));
        phongShader.setFloat("pointLight.constant", 1.0f);
        phongShader.setFloat("pointLight.linear", 1.0f);
        phongShader.setFloat("pointLight.quadratic", 1.0f);

        phongShader.setVec3("ambientLight.position", glm::vec3(0.0, 10.0, 0));
        phongShader.setVec3("ambientLight.ambient", glm::vec3(0.4, 0.4, 0.4));
        phongShader.setVec3("ambientLight.diffuse", glm::vec3(0.0, 0.0, 0.0));
        phongShader.setVec3("ambientLight.specular", glm::vec3(0.0, 0.0, 0.0));
        phongShader.setFloat("ambientLight.constant", 0.01f);
        phongShader.setFloat("ambientLight.linear", 0.01f);
        phongShader.setFloat("ambientLight.quadratic", 0.01f);

        phongShader.setVec3("spotLight.position", glm::vec3(0.0, 1.0f, -2.99f));
        phongShader.setVec3("spotLight.ambient", glm::vec3(1.0, 1.0, 1.0));
        phongShader.setVec3("spotLight.diffuse", glm::vec3(1.0, 0.8, 0.6));
        phongShader.setVec3("spotLight.specular", glm::vec3(1.0, 1.0, 1.0));
        phongShader.setFloat("spotLight.constant", 0.1f);
        phongShader.setFloat("spotLight.linear", 0.01f);
        phongShader.setFloat("spotLight.quadratic", 0.01f);
        phongShader.setVec3("spotLight.direction", glm::vec3(0.0f, 0.0f, 1.0f));
        phongShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(60.0f)));
        phongShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(60.0f)));

        phongShader.setFloat("material.shine", 0.4);
        phongShader.setVec3("material.kA", 0.19225, 0.19225, 0.19225);
        phongShader.setVec3("material.kD", 0.50754, 0.50754, 0.50754);
        phongShader.setVec3("material.kS", 0.508273, 0.508273, 0.508273);
    }
    //gouraud
    {
        gouraudShader.use();
        gouraudShader.setMat4("uV", view);
        gouraudShader.setMat4("uM", glm::mat4(1.0f));
        gouraudShader.setMat4("uP", projectionP);

        gouraudShader.setVec3("viewPos", cameraPos);
        gouraudShader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
        gouraudShader.setVec3("lightPos", glm::vec3(0.0, 10.0, 0.0));

    }
    //basic shaders
    {
        basicShader.use();
        basicShader.setMat4("uV", view);
        basicShader.setMat4("uM", glm::mat4(1.0f));
        basicShader.setMat4("uP", projectionP);

        pointerShader.use();
        pointerShader.setMat4("uV", view);
        pointerShader.setMat4("uM", glm::mat4(1.0f));
        pointerShader.setMat4("uP", projectionP);

        charactersShader.use();
        charactersShader.setMat4("uV", view);
        charactersShader.setMat4("uM", glm::mat4(1.0f));
        charactersShader.setMat4("uP", projectionP);
    }
    //floor shader
    {
        floorShader.use();
        floorShader.setMat4("uV", view);
        floorShader.setMat4("uM", glm::mat4(1.0f));
        floorShader.setMat4("uP", projectionP);
        floorShader.setVec3("uViewPos", cameraPos);

        floorShader.setVec3("pointLight.position", glm::vec3(0.0, -4.8, 3.1));
        floorShader.setVec3("pointLight.ambient", glm::vec3(1.0, 0.0, 0.0));
        floorShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 0.0, 0.0));
        floorShader.setVec3("pointLight.specular", glm::vec3(1.0, 0.0, 0.0));
        floorShader.setFloat("pointLight.constant", 1.0f);
        floorShader.setFloat("pointLight.linear", 1.0f);
        floorShader.setFloat("pointLight.quadratic", 1.0f);

        floorShader.setVec3("ambientLight.position", glm::vec3(0.0, 10.0, 0));
        floorShader.setVec3("ambientLight.ambient", glm::vec3(0.4, 0.4, 0.4));
        floorShader.setVec3("ambientLight.diffuse", glm::vec3(0.0, 0.0, 0.0));
        floorShader.setVec3("ambientLight.specular", glm::vec3(0.0, 0.0, 0.0));
        floorShader.setFloat("ambientLight.constant", 0.01f);
        floorShader.setFloat("ambientLight.linear", 0.01f);
        floorShader.setFloat("ambientLight.quadratic", 0.01f);

        floorShader.setVec3("spotLight.position", glm::vec3(0.0, 1.0f, -2.99f));
        floorShader.setVec3("spotLight.ambient", glm::vec3(1.0, 1.0, 1.0));
        floorShader.setVec3("spotLight.diffuse", glm::vec3(1.0, 0.8, 0.6));
        floorShader.setVec3("spotLight.specular", glm::vec3(1.0, 1.0, 1.0));
        floorShader.setFloat("spotLight.constant", 0.1f);
        floorShader.setFloat("spotLight.linear", 0.01f);
        floorShader.setFloat("spotLight.quadratic", 0.01f);
        floorShader.setVec3("spotLight.direction", glm::vec3(0.0f, 0.0f, 1.0f));
        floorShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(40.0f)));
        floorShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(60.0f)));
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glCullFace(GL_BACK);
    bool perspective = true;
    int rotationXMul = 1;
    int rotationZMul = 1;
    float currentButtonTranslation = 0.0;
    int channel = -1;
    glm::vec3 v1=glm::vec3(0,2,-15);
    glm::vec3 v2 = glm::vec3(0, 0, -1);
    float remoteControllerAngle = 0.0;
    int buttonMoving=0; //0 false, -1 turn on, 1 turn off
    double startTime = glfwGetTime(), currentTime;
    bool tempState = false;
    string pressedKey = "";
    float constantDistance = 0.1;
    Shader selectedShader = phongShader;
    while (!glfwWindowShouldClose(window))
    {
     
        modelShader.use();
        //keys
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            //modeli sencenja
            {
                if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
                {
                    selectedShader = gouraudShader;
                }
                if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
                {
                    selectedShader = phongShader;
                }
            }
            
            //projekcije
            {
                if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
                {
                    perspective = true;
                    setBasicUniforms("uP", projectionP);
                }
                if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
                {
                    perspective = false;
                    setBasicUniforms("uP", projectionO);
                }
            }
            //wireframe
            {
                if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            //daljinski
            {
                if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
                {
                    model = glm::rotate(model, glm::radians(0.01f), glm::vec3(0.0f, 1.0f, 0.0f));
                    modelShader.setMat4("uM", model);
                    v2 = glm::vec3(-model[2][0], -model[2][1], -model[2][2]);
                }
                if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
                {
                    model = glm::rotate(model, glm::radians(-0.01f), glm::vec3(0.0f, 1.0f, 0.0f));
                    modelShader.setMat4("uM", model);
                    v2 = glm::vec3(-model[2][0], -model[2][1], -model[2][2]);
                }
                if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
                {
                    model = glm::rotate(model, glm::radians(0.01f), glm::vec3(1.0f, 0.0f, 0.0f));
                    modelShader.setMat4("uM", model);
                    v2 = glm::vec3(-model[2][0], -model[2][1], -model[2][2]);
                }
                if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
                {
                    model = glm::rotate(model, glm::radians(-0.01f), glm::vec3(1.0f, 0.0f, 0.0f));
                    modelShader.setMat4("uM", model);
                    v2 = glm::vec3(-model[2][0], -model[2][1], -model[2][2]);
                }
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                {
                    v1[0] += 0.01;
                    model = glm::translate(model, glm::vec3(-0.01f, 0.0f, 0.0f));
                    modelShader.setMat4("uM", model);
                }
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                {
                    v1[0] -= 0.01;
                    model = glm::translate(model, glm::vec3(0.01f, 0.0f, 0.0f));
                    
                    modelShader.setMat4("uM", model);
                }
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                {
                    v1[1] -= 0.01;
                    model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
                    
                    modelShader.setMat4("uM", model);
                }
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                {
                    v1[1] += 0.01;
                    model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
                    modelShader.setMat4("uM", model);
                }
            }
            //kamera
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                {
                    float angle = 2 * M_PI / 18000;
                    //cameraPos = glm::mat3(cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle)) * cameraPos;
                    cameraPos = glm::vec3(cameraPos[0]-0.01, cameraPos[1], cameraPos[2]);

                    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraAxis);
                    setBasicUniforms("uV", view);
                    floorShader.use();
                    floorShader.setVec3("uViewPos", cameraPos);
                    phongShader.use();
                    phongShader.setVec3("uViewPos", cameraPos);
                    modelShader.use();
                    modelShader.setVec3("uViewPos", cameraPos);
                    gouraudShader.use();
                    gouraudShader.setVec3("viewPos", cameraPos);
                    
                }
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                {
                    float angle = -2 * M_PI / 18000;
                    //cameraPos = glm::mat3(cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle)) * cameraPos;
                    cameraPos = glm::vec3(cameraPos[0] + 0.01, cameraPos[1], cameraPos[2]);
                    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraAxis);
                    setBasicUniforms("uV", view);
                    floorShader.use();
                    floorShader.setVec3("uViewPos", cameraPos);
                    phongShader.use();
                    phongShader.setVec3("uViewPos", cameraPos);
                    modelShader.use();
                    modelShader.setVec3("uViewPos", cameraPos);
                    gouraudShader.use();
                    gouraudShader.setVec3("viewPos", cameraPos);

                }
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                {
                    float angle = 2 * M_PI / 18000;
                    //cameraPos = glm::mat3(1, 0, 0, 0, cos(angle), -sin(angle), 0, sin(angle), cos(angle)) * cameraPos;
                    cameraPos = glm::vec3(cameraPos[0], cameraPos[1] + 0.01, cameraPos[2]);
                    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraAxis);
                    setBasicUniforms("uV", view);
                    floorShader.use();
                    floorShader.setVec3("uViewPos", cameraPos);
                    phongShader.use();
                    phongShader.setVec3("uViewPos", cameraPos);
                    modelShader.use();
                    modelShader.setVec3("uViewPos", cameraPos);
                    gouraudShader.use();
                    gouraudShader.setVec3("viewPos", cameraPos);

                }
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                {
                    float angle = -2 * M_PI / 18000;
                    //cameraPos = glm::mat3(1, 0, 0, 0, cos(angle), -sin(angle), 0, sin(angle), cos(angle)) * cameraPos;
                    cameraPos = glm::vec3(cameraPos[0], cameraPos[1] - 0.01, cameraPos[2]);
                    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraAxis);
                    setBasicUniforms("uV", view);
                    floorShader.use();
                    floorShader.setVec3("uViewPos", cameraPos);
                    phongShader.use();
                    phongShader.setVec3("uViewPos", cameraPos);
                    modelShader.use();
                    modelShader.setVec3("uViewPos", cameraPos);
                    gouraudShader.use();
                    gouraudShader.setVec3("viewPos", cameraPos);
                }
            }
            //zoom
            {
                if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
                {

                    cameraPos = glm::mat3(1 / 1.002, 0, 0, 0, 1 / 1.002, 0, 0, 0, 1 / 1.002) * cameraPos;
                    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraAxis);
                    setBasicUniforms("uV", view);
                    floorShader.use();
                    floorShader.setVec3("uViewPos", cameraPos);
                    phongShader.use();
                    phongShader.setVec3("uViewPos", cameraPos);
                    modelShader.use();
                    modelShader.setVec3("uViewPos", cameraPos);
                    gouraudShader.use();
                    gouraudShader.setVec3("viewPos", cameraPos);

                }
                if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
                {
                    cameraPos = glm::mat3(1.002, 0, 0, 0, 1.002, 0, 0, 0, 1.002) * cameraPos;
                    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraAxis);
                    setBasicUniforms("uV", view);
                    floorShader.use();
                    floorShader.setVec3("uViewPos", cameraPos);
                    phongShader.use();
                    phongShader.setVec3("uViewPos", cameraPos);
                    modelShader.use();
                    modelShader.setVec3("uViewPos", cameraPos);
                    gouraudShader.use();
                    gouraudShader.setVec3("viewPos", cameraPos);
                }
            }
            //ekran
            {
                float cosTheta = (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
                    / (sqrt(v1[0]* v1[0]+ v1[1] * v1[1]+ v1[2] * v1[2]) * sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]));

                float thetaRad = std::acos(cosTheta);
                if (thetaRad < 3.14159265 / 4)
                {
                    
                    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
                    {
                        if (screen.screenOn && screen.channel!=1)
                        {
                            pressedKey = "1";
                            screen.screenOn = false;
                            floorShader.use();
                            floorShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("spotLight.diffuse", glm::vec3(0.3, 0.8, 1.0));
                            constantDistance = 0.1;
                            phongShader.use();
                            phongShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            startTime = glfwGetTime();
                        }
                    }
                    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
                    {
                        if (screen.screenOn && screen.channel != 2)
                        {
                            pressedKey = "2";
                            screen.screenOn = false;
                            floorShader.use();
                            floorShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("spotLight.diffuse", glm::vec3(0.6, 0.8, 1.0));
                            phongShader.use();
                            phongShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            constantDistance = 0.1;
                            startTime = glfwGetTime();
                        }
                    }
                    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
                    {
                        if (screen.screenOn && screen.channel != 3)
                        {
                            pressedKey = "3";
                            screen.screenOn = false;
                            floorShader.use();
                            floorShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("spotLight.diffuse", glm::vec3(1.0, 0.8, 0.6));

                            phongShader.use();
                            phongShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            startTime = glfwGetTime();
                        }
                    }
                    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
                    {
                        if (!screen.screenOn)
                        {
                            pressedKey = "T";
                            screen.screenOn = false;
                            floorShader.use();
                            floorShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("spotLight.diffuse", glm::vec3(0.3, 0.8, 1.0));
                            phongShader.use();
                            phongShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            constantDistance = 0.1;
                            startTime = glfwGetTime();
                        }
                        
                    }
                    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
                    {
                        if(screen.screenOn)
                        {
                            pressedKey = "Y";
                            screen.screenOn = false;
                            floorShader.use();
                            floorShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            floorShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.use();
                            phongShader.setVec3("pointLight.ambient", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 1.0, 0.0));
                            phongShader.setVec3("pointLight.specular", glm::vec3(1.0, 1.0, 0.0));
                            startTime = glfwGetTime();
                        }
                        
                    }
                    //karakteri
                    {
                        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
                        {
                            screen.moveMickey(-0.005);
                            constantDistance = max(0.01f, constantDistance / 1.005f);

                        }
                        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
                        {
                            screen.moveMickey(0.005);
                            constantDistance = min(5.0f, constantDistance * 1.005f);
                            
                        }
                        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
                        {
                            screen.movePopeye(-0.005);
                            constantDistance = min(5.0f, constantDistance * 1.005f);

                        }
                        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
                        {
                            screen.movePopeye(0.005);
                            constantDistance = max(0.01f, constantDistance / 1.005f);

                        }
                    }
                }
                
            }
            //dugme
            {
                /*if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                    buttonMoving = -1;
                if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
                    buttonMoving = 1;*/
            }
            
            
        }
        
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        currentTime = glfwGetTime();
        //channel change animation
        if (pressedKey!="" && currentTime - startTime > 0.5) {
            
            
            if (pressedKey == "1")
            {
                floorShader.use();
                floorShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));
                phongShader.use();
                phongShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));

                channel = 1;
                screen.screenOn = true;
            }
                
            if (pressedKey == "2")
            {
                floorShader.use();
                floorShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));
                phongShader.use();
                phongShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));

                channel = 2;
                screen.screenOn = true;
            }
            if (pressedKey == "3")
            {
                floorShader.use();
                floorShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));
                phongShader.use();
                phongShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));

                channel = 3;
                screen.screenOn = true;
            }
            if (pressedKey == "T")
            {
                floorShader.use();
                floorShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                floorShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));
                phongShader.use();
                phongShader.setVec3("pointLight.ambient", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.diffuse", glm::vec3(0.0, 1.0, 0.0));
                phongShader.setVec3("pointLight.specular", glm::vec3(0.0, 1.0, 0.0));

                channel = 1;
                screen.resetPosition();
                tv.turnOn(basicShader, pointerShader);
                buttonMoving = -1;
            }
            if (pressedKey == "Y")
            {
                floorShader.use();
                floorShader.setVec3("pointLight.ambient", glm::vec3(1.0, 0.0, 0.0));
                floorShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 0.0, 0.0));
                floorShader.setVec3("pointLight.specular", glm::vec3(1.0, 0.0, 0.0));
                phongShader.use();
                phongShader.setVec3("pointLight.ambient", glm::vec3(1.0, 0.0, 0.0));
                phongShader.setVec3("pointLight.diffuse", glm::vec3(1.0, 0.0, 0.0));
                phongShader.setVec3("pointLight.specular", glm::vec3(1.0, 0.0, 0.0));

                tv.turnOff();
                buttonMoving = 1;
            }
            

            pressedKey = "";
        }

        if ((currentButtonTranslation - 0.0002f >= 0.0 || buttonMoving == -1) && (currentButtonTranslation + 0.0002f <= 0.5 || buttonMoving == 1))
        {
            currentButtonTranslation -= 0.0002f * buttonMoving;
            buttonModel = glm::translate(buttonModel, glm::vec3(0.0f, 0.0f, 0.0002f * buttonMoving));
        }
        else
            buttonMoving = 0;
        

        name.Draw(nameShader);
        
        modelShader.use();
        modelShader.setMat4("uM", model);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("uM", model);
        remoteController.Draw(modelShader);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        phongShader.use();
        phongShader.setFloat("spotLight.constant", constantDistance);

        floorShader.use();
        floorShader.setFloat("spotLight.constant", constantDistance);
        floor.Draw(floorShader);


        /*phongShader.use();
        phongShader.setMat4("uButtonM", buttonModel);
        button.Draw(phongShader);
        phongShader.setMat4("uButtonM", glm::mat4(1.0f));
        
        screen.Draw(phongShader, floorShader, modelShader);
        
        tv.Draw(phongShader);
        tv.setChannel(channel, basicShader, pointerShader, charactersShader);
        */
        selectedShader.use();
        selectedShader.setMat4("uButtonM", buttonModel);
        button.Draw(selectedShader);
        selectedShader.setMat4("uButtonM", glm::mat4(1.0f));

        screen.Draw(selectedShader, floorShader, modelShader);

        tv.Draw(selectedShader);
        tv.setChannel(channel, basicShader, pointerShader, charactersShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


