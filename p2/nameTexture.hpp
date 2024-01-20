#pragma once
#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include <string>
#include <vector>

using namespace std;

struct NameTexture {
    glm::vec3 Position;
    glm::vec2 TexCoordinates;
};


class Name {
public:
    vector<NameTexture> nameTextures;
    vector<unsigned int> nameIndices;

    unsigned int VAO[1];
    unsigned nameTexture;
    // constructor
    Name()
    {
        this->nameTextures = getNameTextures();
        this->nameIndices = getNameIndices();
        glGenVertexArrays(1, VAO);
        setupName();
    }

    void Draw(Shader& nameShader)
    {
        nameShader.use();
        glBindVertexArray(VAO[0]);

        nameShader.setInt("uTex", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
        glBindTexture(GL_TEXTURE_2D, 0);


    }


private:
    // render data 
    unsigned int VBO_Name, EBO_Name;

    void setupName() {


        nameTexture = loadImageToTexture("./res/imel.png");
        setup2dTexture(nameTexture);

        glGenBuffers(1, &VBO_Name);
        glGenBuffers(1, &EBO_Name);

        glBindVertexArray(VAO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Name);
        glBufferData(GL_ARRAY_BUFFER, nameTextures.size() * sizeof(NameTexture), &nameTextures[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Name);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nameIndices.size() * sizeof(unsigned int), &nameIndices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NameTexture), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NameTexture), (void*)offsetof(NameTexture, TexCoordinates));
        glEnableVertexAttribArray(1);

    }
    vector<NameTexture> getNameTextures() {
        float NameVertices[] =
        {   //X        Y       Z    S   T  
             2.0f, -0.9f, 0.0f,      1.0f,  1.0f, // Top Right
             2.0f, -1.0f, 0.0f,      1.0f,  0.0f,// Bottom Right
            -1.0f, -1.0f, 0.0f,      0.0f,  0.0f,// Bottom Left
            -1.0f, -0.9f, 0.0f,      0.0f,  1.0f,// Top Left

        };


        size_t numNameVertices = sizeof(NameVertices) / (sizeof(float) * 5); // Assuming each vertex has 6 float values (3 for position, 3 for normal)

        vector<NameTexture> vertices;
        for (size_t i = 0; i < numNameVertices; ++i) {
            NameTexture vertex;
            vertex.Position = glm::vec3(NameVertices[i * 5], NameVertices[i * 5 + 1], NameVertices[i * 5 + 2]);
            vertex.TexCoordinates = glm::vec2(NameVertices[i * 5 + 3], NameVertices[i * 5 + 4]);

            vertices.push_back(vertex);
        }
        return vertices;
    }
    vector<unsigned int> getNameIndices() {
        vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(1);
        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(2);

        return indices;
    }
};