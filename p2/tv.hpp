
#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include <string>
#include <vector>
#include "button.hpp"
#include "screen.hpp"
using namespace std;

struct TvVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
};

struct TvTexture {
    unsigned int id;
    string path;
};

class Tv {
public:
    // mesh Data
    vector<TvVertex>       vertices;
    vector<unsigned int> indices;
    vector<TvTexture>      textures;
    Button& button;
    Screen& screen;
    int channel;
    unsigned int VAO;

    // constructor
    Tv(Button& button, Screen& screen) :  button(button), screen(screen), channel(-1), VAO(0)
    {
        this -> vertices = getTvVertices();
        this->indices = getTvIndices();
        this->textures = getTvTextures();
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupTv();
    }

    // render the mesh
    void Draw(Shader& shader)
    {
        shader.setFloat("material.shine", 0.4);
        shader.setVec3("material.kA", 0.19225, 0.19225, 0.19225);
        shader.setVec3("material.kD", 0.50754, 0.50754, 0.50754);
        shader.setVec3("material.kS", 0.508273, 0.508273, 0.508273);
        shader.setVec3("objectColor", glm::vec3(0.25, 0.25, 0.25));
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
        //button.Draw(shader);
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

    }
    void turnOn(Shader& basicShader, Shader& pointerShader) {
        button.tvOn = true;
        screen.DrawChannel1(basicShader, pointerShader);
    }
    void turnOff() {
        button.tvOn = false;
        screen.turnOff();
    }
    void setChannel(int channel, Shader& shader, Shader& pointerShader, Shader& textureShader) {
        if (screen.screenOn && button.tvOn == true)
        {
            if (channel == 1)
                screen.DrawChannel1(shader, pointerShader);
            if (channel == 2)
                screen.DrawChannel2(shader);
            if (channel == 3)
                screen.DrawChannel3(textureShader);
        }
    }
        
private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupTv()
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TvVertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TvVertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TvVertex), (void*)offsetof(TvVertex, Normal));
    }
    vector<TvVertex> getTvVertices() {
        float tvVertices[] = {
            // X      Y       Z       NX     NY      NZ
           -7.0f, -5.0f, -3.0f,  0.0f,  0.0f, -1.0f,
            7.0f,  5.0f, -3.0f,  0.0f,  0.0f, -1.0f,
            7.0f, -5.0f, -3.0f,  0.0f,  0.0f, -1.0f,
            7.0f,  5.0f, -3.0f,  0.0f,  0.0f, -1.0f,
           -7.0f, -5.0f, -3.0f,  0.0f,  0.0f, -1.0f,
            -7.0f,  5.0f, -3.0f,  0.0f,  0.0f, -1.0f,

           -7.0f, -5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //A
            7.0f, -5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //B
            6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f, //F
           -7.0f, -5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //A
            6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f, //F
           -6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f, //E

            7.0f, -5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //B
            7.0f,  5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //C
            6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f, //F
            7.0f,  5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //C
            6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f, //G
            6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f, //F

            7.0f,  5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //C
           -6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f, //H
            6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f, //G
            7.0f,  5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //C
           -7.0f,  5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //D
           -6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f, //H

           -7.0f,  5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //D
           -7.0f, -5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //A
           -6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f, //H
           -7.0f, -5.0f,  3.0f,  0.0f,  0.0f,  1.0f, //A
           -6.5f, -3.5f,  3.0f,  0.0f,  0.0f,  1.0f, //E
           -6.5f,  4.5f,  3.0f,  0.0f,  0.0f,  1.0f, //H

           -7.0f,  5.0f,  3.0f, -1.0f,  0.0f,  0.0f,
           -7.0f,  5.0f, -3.0f, -1.0f,  0.0f,  0.0f,
           -7.0f, -5.0f, -3.0f, -1.0f,  0.0f,  0.0f,
           -7.0f, -5.0f, -3.0f, -1.0f,  0.0f,  0.0f,
           -7.0f, -5.0f,  3.0f, -1.0f,  0.0f,  0.0f,
           -7.0f,  5.0f,  3.0f, -1.0f,  0.0f,  0.0f,

            7.0f,  5.0f,  3.0f,  1.0f,  0.0f,  0.0f,
            7.0f, -5.0f, -3.0f,  1.0f,  0.0f,  0.0f,
            7.0f,  5.0f, -3.0f,  1.0f,  0.0f,  0.0f,
            7.0f, -5.0f, -3.0f,  1.0f,  0.0f,  0.0f,
            7.0f,  5.0f,  3.0f,  1.0f,  0.0f,  0.0f,
            7.0f, -5.0f,  3.0f,  1.0f,  0.0f,  0.0f,
            

           -7.0f, -5.0f, -3.0f,  0.0f, -1.0f,  0.0f,
            7.0f, -5.0f, -3.0f,  0.0f, -1.0f,  0.0f,
            7.0f, -5.0f,  3.0f,  0.0f, -1.0f,  0.0f,
            7.0f, -5.0f,  3.0f,  0.0f, -1.0f,  0.0f,
           -7.0f, -5.0f,  3.0f,  0.0f, -1.0f,  0.0f,
           -7.0f, -5.0f, -3.0f,  0.0f, -1.0f,  0.0f,

            -7.0f,  5.0f, -3.0f,  0.0f,  1.0f,  0.0f,
            7.0f,  5.0f,  3.0f,  0.0f,  1.0f,  0.0f,
            7.0f,  5.0f, -3.0f,  0.0f,  1.0f,  0.0f,
            7.0f,  5.0f,  3.0f,  0.0f,  1.0f,  0.0f,
           -7.0f,  5.0f, -3.0f,  0.0f,  1.0f,  0.0f,
           -7.0f,  5.0f,  3.0f,  0.0f,  1.0f,  0.0f
        };

        size_t numTvVertices = sizeof(tvVertices) / (sizeof(float) * 6); // Assuming each vertex has 6 float values (3 for position, 3 for normal)

        vector<TvVertex> vertices;
        for (size_t i = 0; i < numTvVertices; ++i) {
            TvVertex vertex;
            vertex.Position = glm::vec3(tvVertices[i * 6], tvVertices[i * 6 + 1], tvVertices[i * 6 + 2]);
            vertex.Normal = glm::vec3(tvVertices[i * 6 + 3], tvVertices[i * 6 + 4], tvVertices[i * 6 + 5]);
            vertices.push_back(vertex);
        }
        return vertices;
    }
    vector<unsigned int> getTvIndices() {
        vector<unsigned int> indices;
        int numTvVertices = this->vertices.size();
        vector<TvVertex> vertices;
        for (int i = 0; i < numTvVertices; ++i) {
            indices.push_back(i);
        }
        return indices;
    }
    vector<TvTexture> getTvTextures() {
        return vector<TvTexture>();
    }
};