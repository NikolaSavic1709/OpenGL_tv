#pragma once
#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include <string>
#include <vector>
using namespace std;

struct ButtonVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
};

struct ButtonTexture {
    unsigned int id;
    string path;
};

class Button {
public:
    // mesh Data
    vector<ButtonVertex>       vertices;
    vector<unsigned int> indices;
    vector<ButtonTexture>      textures;
    bool tvOn;
    unsigned int VAO;

    // constructor
    Button() :  tvOn(false), VAO(0)
    {
        this -> vertices = getButtonVertices();
        this->indices = getButtonIndices();
        this->textures = getButtonTextures();
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupButton();
    }

    // render the mesh
    void Draw(Shader& shader)
    {
        shader.setFloat("material.shine", 0.4);
        shader.setVec3("material.kA", 0.5, 0.5, 0.5);
        shader.setVec3("material.kD", 0.55, 0.55, 0.55);
        shader.setVec3("material.kS", 0.70, 0.70, 0.70);
        shader.setVec3("objectColor", glm::vec3(0.5, 0.5, 0.5));
        shader.setFloat("screen", 0.0);
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
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

    }
    void turnOn() {
    }
    void turnOff() {

    }
    
        
private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupButton()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ButtonVertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, Normal));
    }
    vector<ButtonVertex> getButtonVertices() {
        float buttonVertices[] = {
            // X      Y       Z       NX     NY      NZ
            5.0f, -4.5f, 3.0f,  0.0f,  0.0f, -1.0f,
            5.5f,  -4.0f, 3.0f,  0.0f,  0.0f, -1.0f,
            5.5f, -4.5f, 3.0f,  0.0f,  0.0f, -1.0f,
            5.5f,  -4.0f, 3.0f,  0.0f,  0.0f, -1.0f,
            5.0f, -4.5f, 3.0f,  0.0f,  0.0f, -1.0f,
            5.0f,  -4.0f, 3.0f,  0.0f,  0.0f, -1.0f,
           

            5.0f, -4.5f,  3.5f,  0.0f,  0.0f,  1.0f,
            5.5f, -4.5f,  3.5f,  0.0f,  0.0f,  1.0f,
            5.5f,  -4.0f,  3.5f,  0.0f,  0.0f,  1.0f,
            5.5f,  -4.0f,  3.5f,  0.0f,  0.0f,  1.0f,
            5.0f,  -4.0f,  3.5f,  0.0f,  0.0f,  1.0f,
            5.0f, -4.5f,  3.5f,  0.0f,  0.0f,  1.0f,

            5.0f, -4.0f,  3.5f, -1.0f,  0.0f,  0.0f,
            5.0f, -4.0f,  3.0f, -1.0f,  0.0f,  0.0f,
            5.0f, -4.5f,  3.0f, -1.0f,  0.0f,  0.0f,
            5.0f, -4.5f,  3.0f, -1.0f,  0.0f,  0.0f,
            5.0f, -4.5f,  3.5f, -1.0f,  0.0f,  0.0f,
            5.0f, -4.0f,  3.5f, -1.0f,  0.0f,  0.0f,

            5.5f, -4.0f,  3.5f,  1.0f,  0.0f,  0.0f,
            5.5f, -4.5f,  3.0f,  1.0f,  0.0f,  0.0f,
            5.5f, -4.0f,  3.0f,  1.0f,  0.0f,  0.0f,
            5.5f, -4.5f,  3.0f,  1.0f,  0.0f,  0.0f,
            5.5f, -4.0f,  3.5f,  1.0f,  0.0f,  0.0f,
            5.5f, -4.5f,  3.5f,  1.0f,  0.0f,  0.0f,
            

            5.0f, -4.5f,  3.0f,  0.0f, -1.0f,  0.0f,
            5.5f, -4.5f,  3.0f,  0.0f, -1.0f,  0.0f,
            5.5f, -4.5f,  3.5f,  0.0f, -1.0f,  0.0f,
            5.5f, -4.5f,  3.5f,  0.0f, -1.0f,  0.0f,
            5.0f, -4.5f,  3.5f,  0.0f, -1.0f,  0.0f,
            5.0f, -4.5f,  3.0f,  0.0f, -1.0f,  0.0f,

            5.0f, -4.0f, 3.0f,  0.0f,  1.0f,  0.0f,
            5.5f, -4.0f,  3.5f,  0.0f,  1.0f,  0.0f,
            5.5f, -4.0f,  3.0f,  0.0f,  1.0f,  0.0f,
            5.5f, -4.0f,  3.5f,  0.0f,  1.0f,  0.0f,
            5.0f, -4.0f,  3.0f,  0.0f,  1.0f,  0.0f,
            5.0f, -4.0f,  3.5f,  0.0f,  1.0f,  0.0f
        };

        size_t numButtonVertices = sizeof(buttonVertices) / (sizeof(float) * 6); // Assuming each vertex has 6 float values (3 for position, 3 for normal)

        vector<ButtonVertex> vertices;
        for (size_t i = 0; i < numButtonVertices; ++i) {
            ButtonVertex vertex;
            vertex.Position = glm::vec3(buttonVertices[i * 6], buttonVertices[i * 6 + 1], buttonVertices[i * 6 + 2]);
            vertex.Normal = glm::vec3(buttonVertices[i * 6 + 3], buttonVertices[i * 6 + 4], buttonVertices[i * 6 + 5]);
            vertices.push_back(vertex);
        }
        return vertices;
    }
    vector<unsigned int> getButtonIndices() {
        vector<unsigned int> indices;
        int numButtonVertices = this->vertices.size();
        vector<ButtonVertex> vertices;
        for (int i = 0; i < numButtonVertices; ++i) {
            indices.push_back(i);
        }
        return indices;
    }
    vector<ButtonTexture> getButtonTextures() {
        return vector<ButtonTexture>();
    }
};