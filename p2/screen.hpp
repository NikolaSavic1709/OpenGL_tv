#pragma once
#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include <string>
#include <vector>

#define CRES 30
using namespace std;

struct ScreenVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
};
struct ScreenElementVertex {
    // position
    glm::vec3 Position;
    // color
    glm::vec4 Color;
};
struct CartoonTexture {
    glm::vec3 Position;
    glm::vec2 TexCoordinates;
};

struct ScreenTexture {
    unsigned int id;
    string path;
};

class Screen {
public:
    // mesh Data
    vector<ScreenVertex>       vertices;
    vector<unsigned int> indices;
    vector<ScreenTexture>      textures;
    vector<ScreenElementVertex> circleVertices;
    vector<unsigned int> circleIndices;
    vector<CartoonTexture> cartoonTextures;
    vector<unsigned int> cartoonIndices;
    bool screenOn;
    int channel;
    float pointerStep;
    float speed;
    float movementMickey;
    float movementPopeye;
    unsigned int VAO[3];
    unsigned mickeyTexture;
    unsigned popeyeTexture;

    // constructor
    Screen() :  screenOn(false), channel(-1), pointerStep(0.0f),
        speed(1.0f), movementMickey(0.0f), movementPopeye(0.0f)
    {
        this -> vertices = getScreenVertices();
        this->indices = getScreenIndices();
        this->textures = getScreenTextures();
        this->circleVertices = getCircleVertices();
        this->circleIndices = getCircleIndices();
        this->cartoonTextures = getCartoonTextures();
        this->cartoonIndices = getCartoonIndices();
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        glGenVertexArrays(3, VAO);
        setupScreen();
        setupClock();
        setupCartoon();
    }

    // render the mesh
    void Draw(Shader& shader, Shader& floorShader, Shader& modelShader)
    {
        shader.use();
        if (screenOn)
        {
            shader.setVec3("material.kA", 1,1,1);
            shader.setVec3("material.kD", 1,1,1);
            shader.setVec3("material.kS", 0.0, 0.0, 0.0);
            shader.setVec3("objectColor", glm::vec3(0.8, 0.8, 0.8));
            shader.setFloat("screen", 1.0);
            floorShader.use();
            floorShader.setFloat("screen", 1.0);
            modelShader.use();
            modelShader.setFloat("screen", 1.0);
        }
        else {
            shader.setVec3("material.kA", 0.5 * 0.1, 0.5 * 0.1, 0.5 * 0.1);
            shader.setVec3("material.kD", 0.55 * 0.1, 0.55 * 0.1, 0.55 * 0.1);
            shader.setVec3("material.kS", 0.70, 0.70, 0.70);
            shader.setVec3("objectColor", glm::vec3(0.1, 0.1, 0.1));
            shader.setFloat("screen", 0.0);
            floorShader.use();
            floorShader.setFloat("screen", 0.0);
            modelShader.use();
            modelShader.setFloat("screen", 0.0);

        }
        shader.use();
        shader.setFloat("material.shine", 0.25);

        
        
        // bind appropriate textures
        unsigned int textureNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
                number = std::to_string(textureNr++); 
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

    }
    void DrawChannel1(Shader& clockShader, Shader& pointerShader)
    {
        screenOn = true;
        this->channel = 1;

        clockShader.use();
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CRES);

        glLineWidth(2.0);
        pointerShader.use();
        float pointerStep = glfwGetTime();
        pointerShader.setFloat("pRotation", pointerStep);
        glDrawArrays(GL_LINES, CRES, 2);
        glBindVertexArray(0);
    }
    void DrawChannel2(Shader& clockShader)
    {
        screenOn = true;
        this->channel = 2;

        clockShader.use();
        glBindVertexArray(VAO[0]);

        glPointSize(7.0);
        glDrawArrays(GL_POINTS, 0, CRES);
    }
    void DrawChannel3(Shader& characterShader)
    {
        screenOn = true;
        this->channel = 3;
        characterShader.use();
        glBindVertexArray(VAO[2]);

        characterShader.setInt("uTex", 1);
        characterShader.setFloat("tMovement", this->movementMickey);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mickeyTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
        glBindTexture(GL_TEXTURE_2D, 0);

        characterShader.setInt("uTex", 2);
        characterShader.setFloat("tMovement", this->movementPopeye);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, popeyeTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
        glBindTexture(GL_TEXTURE_2D, 0);

    }
    void turnOff() {
        screenOn = false;
        this->channel = -1;

    }
    void movePopeye(float step) {
        if ((step > 0 && movementPopeye < -step) || (step < 0 && movementPopeye > -3.5))
            movementPopeye += step;
    }
    void moveMickey(float step) {
        if ((step > 0 && movementMickey < 3.5) || (step < 0 && movementMickey > -step))
            movementMickey += step;
    }
    void resetPosition() {
        movementMickey = 0;
        movementPopeye = 0;
    }
        
private:
    // render data 
    unsigned int VBO, EBO, VBO_CLOCK, EBO_CLOCK, VBO_CARTOON, EBO_CARTOON;

    // initializes all the buffer objects/arrays
    void setupScreen()
    {
        // create buffers/arrays
        
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO[1]);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ScreenVertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)offsetof(ScreenVertex, Normal));
    }
    void setupClock()
    {
        // create buffers/arrays
        //glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO_CLOCK);
        glGenBuffers(1, &EBO_CLOCK);

        glBindVertexArray(VAO[0]);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO_CLOCK);
        glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(ScreenElementVertex), &circleVertices[0], GL_STATIC_DRAW); //TODO

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_CLOCK);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, circleIndices.size() * sizeof(unsigned int), &circleIndices[0], GL_STATIC_DRAW); //TODO

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ScreenElementVertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ScreenElementVertex), (void*)offsetof(ScreenElementVertex, Color));
    }
    void setupCartoon() {
        
        mickeyTexture = loadImageToTexture("./res/miki.png");
        setup2dTexture(mickeyTexture);

        popeyeTexture = loadImageToTexture("./res/popaj.png");
        setup2dTexture(popeyeTexture);

        glGenBuffers(1, &VBO_CARTOON);
        glGenBuffers(1, &EBO_CARTOON);

        glBindVertexArray(VAO[2]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_CARTOON);
        glBufferData(GL_ARRAY_BUFFER, cartoonTextures.size() * sizeof(CartoonTexture), &cartoonTextures[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_CLOCK);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cartoonIndices.size() * sizeof(unsigned int), &cartoonIndices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CartoonTexture), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CartoonTexture), (void*)offsetof(CartoonTexture, TexCoordinates));
        glEnableVertexAttribArray(1);

    }
    vector<ScreenVertex> getScreenVertices() {
        float screenVertices[] = {
           

           -6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f,
            6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f,
            6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f,
            6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f,
           -6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f,
           -6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f
        };

        size_t numScreenVertices = sizeof(screenVertices) / (sizeof(float) * 6); // Assuming each vertex has 6 float values (3 for position, 3 for normal)

        vector<ScreenVertex> vertices;
        for (size_t i = 0; i < numScreenVertices; ++i) {
            ScreenVertex vertex;
            vertex.Position = glm::vec3(screenVertices[i * 6], screenVertices[i * 6 + 1], screenVertices[i * 6 + 2]);
            vertex.Normal = glm::vec3(screenVertices[i * 6 + 3], screenVertices[i * 6 + 4], screenVertices[i * 6 + 5]);
            vertices.push_back(vertex);
        }
        return vertices;
    }
    vector<unsigned int> getScreenIndices() {
        vector<unsigned int> indices;
        int numScreenVertices = this->vertices.size();
        for (int i = 0; i < numScreenVertices; ++i) {
            indices.push_back(i);
        }
        return indices;
    }
    vector<ScreenTexture> getScreenTextures() {
        return vector<ScreenTexture>();
    }
    vector<ScreenElementVertex> getCircleVertices() {
        float circle[7 * (CRES+2)];
        float ry = 3.0;
        float rx = ry ;
        int i;
        for (i = 0; i < CRES; i++)
        {
            circle[7 * i] = rx * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
            circle[7 * i + 1] = 0.5+ry * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
            circle[7 * i + 2] = 3.01;
            circle[7 * i + 3] = 0.0;
            circle[7 * i + 4] = 0.0;
            circle[7 * i + 5] = 1.0;
            circle[7 * i + 6] = 0.2;
        }
        circle[7 * CRES] = 0.0; //Xi
        circle[7 * CRES + 1] = 0.5; //Yi
        circle[7 * CRES + 2] = 3.02;
        circle[7 * CRES + 3] = 0.0;
        circle[7 * CRES + 4] = 0.0;
        circle[7 * CRES + 5] = 1.0;
        circle[7 * CRES + 6] = 1.0;
        circle[7 * CRES + 7] = 2.5; //Xi
        circle[7 * CRES + 7 + 1] = 0.5; //Yi
        circle[7 * CRES + 7 + 2] = 3.01;
        circle[7 * CRES + 7 + 3] = 0.0;
        circle[7 * CRES + 7 + 4] = 0.0;
        circle[7 * CRES + 7 + 5] = 1.0;
        circle[7 * CRES + 7 + 6] = 1.0;

        int numCircleVertices = sizeof(circle) / (sizeof(float) * 7);

        vector<ScreenElementVertex> vertices;
        for (int i = 0; i < numCircleVertices; ++i) {
            ScreenElementVertex vertex;
            vertex.Position = glm::vec3(circle[i * 7], circle[i * 7 + 1], circle[i * 7 + 2]);
            vertex.Color = glm::vec4(circle[i * 7 + 3], circle[i * 7 + 4], circle[i * 7 + 5], circle[i * 7 + 6]);
            vertices.push_back(vertex);
        }
        return vertices;
        
    }
    vector<unsigned int> getCircleIndices() {
        vector<unsigned int> indices;
        int numCircleVertices = this->circleVertices.size();
        for (int i = 0; i < numCircleVertices; ++i) {
            indices.push_back(i);
        }
        return indices;
    }
    vector<CartoonTexture> getCartoonTextures() {
        float cartoonVertices[] =
        {   //X    Y      S    T 
            -6.0, 2.0,  3.01, 0.0, 1.0,
            -6.0, -3.0, 3.01, 0.0, 0.0,
            -3.5, -3.0, 3.01, 1.0, 0.0,
            -3.5, 2.0,  3.01, 1.0, 1.0,

            3.5,  2.0,  3.01,  0.0, 1.0,
            3.5, -3.0,  3.01,  0.0, 0.0,
            6.0, -3.0,  3.01,  1.0, 0.0,
            6.0,  2.0,  3.01,  1.0, 1.0,

        };
        

        size_t numCartoonVertices = sizeof(cartoonVertices) / (sizeof(float) * 5); // Assuming each vertex has 6 float values (3 for position, 3 for normal)

        vector<CartoonTexture> vertices;
        for (size_t i = 0; i < numCartoonVertices; ++i) {
            CartoonTexture vertex;
            vertex.Position = glm::vec3(cartoonVertices[i * 5], cartoonVertices[i * 5 + 1], cartoonVertices[i * 5 + 2]);
            vertex.TexCoordinates = glm::vec2(cartoonVertices[i * 5 + 3], cartoonVertices[i * 5 + 4]);
            vertices.push_back(vertex);
        }
        return vertices;
    }
    vector<unsigned int> getCartoonIndices() {
        vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(4);
        indices.push_back(6);
        indices.push_back(7);
        return indices;
    }
};