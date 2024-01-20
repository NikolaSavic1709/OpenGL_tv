#pragma once
#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include <string>
#include <vector>

using namespace std;

struct FloorTexture {
    glm::vec3 Position;
    glm::vec2 TexCoordinates;
    glm::vec3 NormalCoordinates;
};


class Floor {
public:
    vector<FloorTexture> floorTextures;
    vector<unsigned int> floorIndices;

    unsigned int VAO[1];
    unsigned carpetTexture;
    unsigned laminateTexture;
    unsigned specularCarpetTexture;
    unsigned specularLaminateTexture;
    // constructor
    Floor()
    {
        this->floorTextures = getFloorTextures();
        this->floorIndices = getFloorIndices();
        glGenVertexArrays(1, VAO);
        setupFloor();
    }

    void Draw(Shader& floorShader)
    {
        floorShader.use();
        glBindVertexArray(VAO[0]);

        floorShader.setInt("material.diffuse", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, carpetTexture);
        floorShader.setInt("material.specular", 2);
        glActiveTexture(GL_TEXTURE2);
        floorShader.setFloat("material.shininess", 10.0);
        glBindTexture(GL_TEXTURE_2D, specularCarpetTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
        glBindTexture(GL_TEXTURE_2D, 0);

        floorShader.setInt("material.diffuse", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, laminateTexture);
        floorShader.setInt("material.specular", 4);
        glActiveTexture(GL_TEXTURE4);
        floorShader.setFloat("material.shininess", 50.0);
        glBindTexture(GL_TEXTURE_2D, specularLaminateTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
        
        glBindTexture(GL_TEXTURE_2D, 0);

        floorShader.setInt("material.diffuse", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, laminateTexture);
        floorShader.setInt("material.specular", 4);
        glActiveTexture(GL_TEXTURE4);
        floorShader.setFloat("material.shininess", 1000.0);
        glBindTexture(GL_TEXTURE_2D, specularLaminateTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, 0);

    }
    

private:
    // render data 
    unsigned int VBO_FLOOR, EBO_FLOOR;

    void setupFloor() {

       
        carpetTexture = loadImageToTexture("./res/carpet.png");
        setup2dTexture(carpetTexture);

        laminateTexture = loadImageToTexture("./res/laminate.png");
        setup2dTexture(laminateTexture);

        specularCarpetTexture = loadImageToTexture("./res/specMapCarpet1.png");
        setup2dTexture(specularCarpetTexture);

        specularLaminateTexture = loadImageToTexture("./res/specMapLaminate.png");
        setup2dTexture(specularLaminateTexture);

        glGenBuffers(1, &VBO_FLOOR);
        glGenBuffers(1, &EBO_FLOOR);

        glBindVertexArray(VAO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_FLOOR);
        glBufferData(GL_ARRAY_BUFFER, floorTextures.size() * sizeof(FloorTexture), &floorTextures[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_FLOOR);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, floorIndices.size() * sizeof(unsigned int), &floorIndices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FloorTexture), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FloorTexture), (void*)offsetof(FloorTexture, TexCoordinates));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FloorTexture), (void*)offsetof(FloorTexture, NormalCoordinates));
        glEnableVertexAttribArray(2);

    }
    vector<FloorTexture> getFloorTextures() {
        float floorVertices[] =
        {   //X        Y       Z    S   T   XN   YN   ZN
           -10.0,  -5.01, -10.0, 0.0, 0.0, 0.0, 1.0, 0.0,
           -10.0,  -5.01,  10.0, 0.0, 1.0, 0.0, 1.0, 0.0,
            10.0,  -5.01,  10.0, 1.0, 1.0, 0.0, 1.0, 0.0,
            10.0,  -5.01, -10.0, 1.0, 0.0, 0.0, 1.0, 0.0,

           -15.0,  -5.02, -15.0,  0.0, 0.0, 0.0, 1.0, 0.0,
           -15.0,  -5.02,  25.0,  0.0, 1.0, 0.0, 1.0, 0.0,
            15.0,  -5.02,  25.0,  1.0, 1.0, 0.0, 1.0, 0.0,
            15.0,  -5.02, -15.0,  1.0, 0.0, 0.0, 1.0, 0.0,

           -15.0,  -5.03,  25.0,  0.0, 1.0, 0.0, -1.0, 0.0,
            -15.0, -5.03, -15.0,  0.0, 0.0, 0.0, -1.0, 0.0,
            15.0,  -5.03, -15.0,  1.0, 0.0, 0.0, -1.0, 0.0,
            15.0,  -5.03,  25.0,  1.0, 1.0, 0.0, -1.0, 0.0,

        };


        size_t numFloorVertices = sizeof(floorVertices) / (sizeof(float) * 8); // Assuming each vertex has 6 float values (3 for position, 3 for normal)

        vector<FloorTexture> vertices;
        for (size_t i = 0; i < numFloorVertices; ++i) {
            FloorTexture vertex;
            vertex.Position = glm::vec3(floorVertices[i * 8], floorVertices[i * 8 + 1], floorVertices[i * 8 + 2]);
            vertex.TexCoordinates = glm::vec2(floorVertices[i * 8 + 3], floorVertices[i * 8 + 4]);
            vertex.NormalCoordinates = glm::vec3(floorVertices[i * 8+5], floorVertices[i * 8 + 6], floorVertices[i * 8 + 7]);

            vertices.push_back(vertex);
        }
        return vertices;
    }
    vector<unsigned int> getFloorIndices() {
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
        
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(8);
        indices.push_back(10);
        indices.push_back(11);
        return indices;
    }
};