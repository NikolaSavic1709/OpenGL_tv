// Autor: Nikola Savic
// Opis: Projektni zadatak br. 5

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CRES 30

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);


void renderTv(int VAO, int VBO, int EBO) {
    float vertices[] = //Tjemena trougla koja sadrze sve informacije o njemu
    {
        // frame
        // X      Y      R    G    B    A
        -0.9,   0.9,    0.5, 0.5, 0.5, 1.0,
        0.9,    0.9,    0.5, 0.5, 0.5, 1.0,
        -0.9,  -0.9,    0.5, 0.5, 0.5, 1.0,
        0.9,   -0.9,    0.5, 0.5, 0.5, 1.0,

        // border
        // X      Y      R    G    B    A
        -0.9,   0.9,    0.3, 0.3, 0.3, 1.0,
        0.9,    0.9,    0.3, 0.3, 0.3, 1.0,
        -0.9,  -0.9,    0.3, 0.3, 0.3, 1.0,
        0.9,   -0.9,    0.3, 0.3, 0.3, 1.0,

        -0.92,  0.92,   1.0, 1.0, 1.0, 1.0,
        0.92,   0.92,   1.0, 1.0, 1.0, 1.0,
        -0.92, -0.92,    1.0, 1.0, 1.0, 1.0,
        0.92,  -0.92,    1.0, 1.0, 1.0, 1.0,

        // Screen
        // X      Y      R    G    B    A
        -0.85,   0.85,    0.0, 0.0, 0.0, 1.0,
        0.85,    0.85,    0.0, 0.0, 0.0, 1.0,
        -0.85,  -0.60,    0.0, 0.0, 0.0, 1.0,
        0.85,   -0.60,    0.0, 0.0, 0.0, 1.0
    };
    unsigned int strideTv = (2 + 4) * sizeof(float);

    unsigned int indices[] = // Indeksi tjemena
    {
        0, 1, 2,
        1, 2, 3,

        4,5,8,
        5,8,9,

        5,7,9,
        7,9,11,

        6,7,10,
        7,10,11,

        6,4,10,
        4,10,8,

        12, 13, 14,
        13, 14, 15
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTv, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, strideTv, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void renderButton(int VAO, int VBO) {
    float circle[2*CRES+4];
    float rx = 0.06; //poluprecnik
    float ry = 0.06 * 900 / 700;
    circle[0] = 0.75; //Centar X0
    circle[1] = -0.75; //Centar Y0
    int i;
    for (i = 0; i <= CRES; i++)
    {
        circle[2 + 2 * i] = 0.75 + rx * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
        circle[2 + 2 * i + 1] = -0.75 + ry * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void renderCartoon(int VAO, int VBO, int EBO) {
    float vertices[] =
    {   //X    Y      S    T 
        -0.98, 0.0,   0.0, 1.0,
        -0.98, -0.98,  0.0, 0.0,
        -0.5, -0.98,    1.0, 0.0,
        -0.5, 0.0,  1.0, 1.0,

        0.5, 0.0,   0.0, 1.0,
        0.5, -0.98,  0.0, 0.0,
        0.98, -0.98,    1.0, 0.0,
        0.98, 0.0,  1.0, 1.0,
        
    };
    unsigned int indices[] = // Indeksi tjemena
    {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7
    };
    unsigned int stride = (2 + 2) * sizeof(float);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}
void renderName(int VAO, int VBO, int EBO) {
    float vertices[] =
    {   //X    Y      S    T 
        -0.40, -0.65,   0.0, 1.0,
        -0.40, -0.85,  0.0, 0.0,
        0.40, -0.85,    1.0, 0.0,
        0.40, -0.65,  1.0, 1.0,

    };
    unsigned int indices[] = // Indeksi tjemena
    {
        0, 1, 2,
        0, 2, 3,
    };
    // notacija koordinata za teksture je STPQ u GLSL-u (ali se cesto koristi UV za 2D teksture i STR za 3D)
    //ST koordinate u nizu tjemena su koordinate za teksturu i krecu se od 0 do 1, gdje je 0, 0 donji lijevi ugao teksture
    //Npr. drugi red u nizu tjemena ce da mapira boje donjeg lijevog ugla teksture na drugo tjeme
    unsigned int stride = (2 + 2) * sizeof(float);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
void renderBackgroundAndClock(int VAO, int VBO, int EBO) {
    float vertices[] = //Tjemena trougla koja sadrze sve informacije o njemu
    {
        // frame
        // X      Y      R    G    B    A
        -1.0,   1.0,    1.0, 1.0, 1.0, 1.0,
        1.0,    1.0,    1.0, 1.0, 1.0, 1.0,
        -1.0,  -1.0,    1.0, 1.0, 1.0, 1.0,
        1.0,   -1.0,    1.0, 1.0, 1.0, 1.0,

    };
    unsigned int stride = (2 + 4) * sizeof(float);

    unsigned int indices[] = // Indeksi tjemena
    {
        0, 1, 2,
        1, 2, 3,
    };

    float circle[6 * CRES];
    float rx = 0.6; //poluprecnik
    float ry = 0.6 * 900 / 700;

    int i;
    for (i = 0; i < CRES; i++)
    {
        circle[6 * i] = rx * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
        circle[6 * i + 1] = ry * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
        circle[6 * i + 2] = 0.0;
        circle[6 * i + 3] = 0.0;
        circle[6 * i + 4] = 1.0;
        circle[6 * i + 5] = 0.2;
    }

    float* result = new float[24 + 6 * CRES + 12];
    std::copy(vertices, vertices + 24, result);
    std::copy(circle, circle + 6 * CRES, result + 24);

    // pointer
    result[24 + 6 * CRES ] = 0; //Xi
    result[24 + 6 * CRES + 1] = 0; //Yi
    result[24 + 6 * CRES + 2] = 0.0;
    result[24 + 6 * CRES + 3] = 0.0;
    result[24 + 6 * CRES + 4] = 1.0;
    result[24 + 6 * CRES + 5] = 1.0;
    result[24 + 6 * CRES + 6] = 0.5; //Xi
    result[24 + 6 * CRES + 6 + 1] = 0; //Yi
    result[24 + 6 * CRES + 6 + 2] = 0.0;
    result[24 + 6 * CRES + 6 + 3] = 0.0;
    result[24 + 6 * CRES + 6 + 4] = 1.0;
    result[24 + 6 * CRES + 6 + 5] = 1.0;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle)+sizeof(vertices)+12*sizeof(float), result, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void setup2dTexture(unsigned texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    // kako se ponasa kada je model veci od teksture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T = V = Y
    // kako se ponasaju pikseli ako se tekstura prikazuje u manjoj i vecoj rezoluciji
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(void)
{
    GLFWwindow* window;
    unsigned int wWidth = 900;
    unsigned int wHeight = 700;
    {
        if (!glfwInit())
        {
            std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
            return 1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



        const char wTitle[] = "Nikola Savic sv21/2020";
        window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

        if (window == NULL)
        {
            std::cout << "Prozor nije napravljen! :(\n";
            glfwTerminate();
            return 2;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (glewInit() != GLEW_OK)
        {
            std::cout << "GLEW nije mogao da se ucita! :'(\n";
            return 3;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    

    unsigned int basicShader = createShader("basic.vert", "basic.frag");
    unsigned int pulseShader = createShader("basic.vert", "pulse.frag");

    unsigned int textureShader = createShader("texture.vert", "texture.frag");
    unsigned int pointerShader = createShader("pointer.vert", "basic.frag");

    int pColLoc = glGetUniformLocation(pulseShader, "pCol");
    int pRotationLoc = glGetUniformLocation(pointerShader, "pRotation");
    int resolutionLoc = glGetUniformLocation(pointerShader, "resolution");
    int uTextureLoc = glGetUniformLocation(textureShader, "uTex");
    int tMovementLoc = glGetUniformLocation(textureShader, "tMovement");

    unsigned int VAO[6];
    glGenVertexArrays(6, VAO); 

    unsigned int VBO[6];
    glGenBuffers(6, VBO);

    unsigned int EBO[5];
    glGenBuffers(5, EBO);


    renderTv(VAO[0], VBO[0], EBO[0]);
    renderButton(VAO[1], VBO[1]);
    renderName(VAO[2], VBO[2], EBO[1]);
    renderBackgroundAndClock(VAO[3], VBO[3], EBO[2]);
    
    renderCartoon(VAO[4], VBO[4], EBO[3]);
    renderCartoon(VAO[5], VBO[5], EBO[4]);
    
    unsigned nameTexture = loadImageToTexture("./res/ime.png");
    setup2dTexture(nameTexture);
    
    unsigned mickeyTexture = loadImageToTexture("./res/miki.png");
    setup2dTexture(mickeyTexture);

    unsigned popeyeTexture = loadImageToTexture("./res/popaj.png");
    setup2dTexture(popeyeTexture);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    bool tvOn = false;
    int channel = -1;
    float grayValue;
    float pointerStep;
    float speed = 1;
    float movementMickey = 0;
    float movementPopeye = 0;

    while (!glfwWindowShouldClose(window)) 
    {
        grayValue = abs(sin(glfwGetTime() / 2));
        pointerStep = glfwGetTime()*speed;

        //keys
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !tvOn)
            {
                tvOn = true;
                channel = 3;
                movementMickey = 0;
                movementPopeye = 0;
            }
            if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && tvOn)
            {
                tvOn = false;
                channel = -1;
            }
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && tvOn)
            {
                channel = 1;
                speed = 1;

            }
            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && tvOn)
                channel = 2;
            if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && tvOn)
                channel = 3;
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && channel == 1)
                speed *= 1.00;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && channel == 1)
                speed /= 1.00;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && channel == 3)
                if (movementPopeye > -0.5)
                    movementPopeye -= 0.05;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && channel == 3)
                if (movementPopeye < -0.05)
                    movementPopeye += 0.05;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && channel == 3)
                if (movementMickey >0.05)
                    movementMickey -= 0.05;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && channel == 3)
                if (movementMickey < 0.5)
                    movementMickey += 0.05;
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && channel == 3)
            {
                movementMickey = 0;
                movementPopeye = 0;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, wWidth, wHeight);

        // frame
        glUseProgram(basicShader);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
       

        // button
        glUseProgram(pulseShader);
        glBindVertexArray(VAO[1]);

        if(tvOn)
            glUniform3f(pColLoc, 0.9, 0.5, 0.8);
        else
            glUniform3f(pColLoc, grayValue, grayValue, grayValue);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CRES+2);


        //name
        glUseProgram(textureShader);
        glBindVertexArray(VAO[2]);
        glUniform1i(uTextureLoc, 0);
        glUniform1f(tMovementLoc, 0);
        glActiveTexture(GL_TEXTURE0); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
        glBindTexture(GL_TEXTURE_2D, 0);



        glViewport(0.075 * wWidth, 0.2 * wHeight, 0.85 * wWidth, 0.725 * wHeight);
        // cartoon
        if (channel == 3)
        {
            glUseProgram(basicShader);
            glBindVertexArray(VAO[3]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

            glUseProgram(textureShader);
            glBindVertexArray(VAO[4]);
            glUniform1i(uTextureLoc, 1);
            glUniform1f(tMovementLoc, movementMickey);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mickeyTexture);
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
            glBindTexture(GL_TEXTURE_2D, 0);

            
            glUniform1i(uTextureLoc, 2);
            glActiveTexture(GL_TEXTURE2);
            glUniform1f(tMovementLoc, movementPopeye);
            glBindTexture(GL_TEXTURE_2D, popeyeTexture);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // clock
        else if (channel == 1) 
        {
            glUseProgram(basicShader);
            glBindVertexArray(VAO[3]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

            glLineWidth(2.0);
            glDrawArrays(GL_TRIANGLE_FAN, 4, CRES );

            glUseProgram(pointerShader);
            glUniform1f(pRotationLoc, pointerStep);
            glUniform1f(resolutionLoc, wWidth*1.0/wHeight);
            glDrawArrays(GL_LINES, CRES + 4, 2);
        }
        // triangle
        else if (channel == 2)
        {
            glUseProgram(basicShader);
            glBindVertexArray(VAO[3]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

            glPointSize(10.0);
            glDrawArrays(GL_POINTS, 4, CRES);
        }
        

        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    glDeleteTextures(1, &mickeyTexture);
    glDeleteTextures(1, &popeyeTexture);
    glDeleteTextures(1, &nameTexture);
    glDeleteBuffers(6, VBO);
    glDeleteVertexArrays(6, VAO);
    glDeleteProgram(basicShader);
    glDeleteProgram(pulseShader);
    glDeleteProgram(pointerShader);
    glDeleteProgram(textureShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}