#define GLEW_STATIC
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include "mat33.cpp"

using namespace std;

int framecount =0; // vari�vel global

string readFile(string filename) {
    ifstream t(filename);
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void timer() {
    while (true) {
        cout << framecount<< endl;
        framecount = 0;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

struct vertex {
    float x, y, r, g, b;
};

void on_resize(GLFWwindow* window, int width, int height) {
    if (height<width) glViewport( (width-height)/2 , 0, height, height);
    else glViewport(0, (height-width)/2, width, width);
}

int main(void) {
    thread t1(timer); // cria e p�e pra rodar a thread
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, on_resize);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit()!= GLEW_OK) {
        std::cout << "Erro ao iniciar GLEW";
        return 0;
    }

    mat33 m1;
    m1.print();


    vertex v[] = {
        { 0.0f, 0.0f, 0.0f, 0.5f, 0.5f} ,
        // Top
        {-0.2f, 0.8f, 0.0f, 1.0f, 0.0f} ,
        { 0.2f, 0.8f, 0.0f, 0.0f, 1.0f} ,
        { 0.0f, 0.8f, 0.0f, 1.0f, 1.0f} ,
        { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f} ,
        // Bottom
        {-0.2f,-0.8f, 0.0f, 0.0f, 1.0f} ,
        { 0.2f,-0.8f, 0.0f, 1.0f, 0.0f} ,
        { 0.0f,-0.8f, 0.0f, 1.0f, 1.0f} ,
        { 0.0f,-1.0f, 1.0f, 0.0f, 0.0f} ,
        // Left
        {-0.8f,-0.2f, 0.0f, 1.0f, 0.0f} ,
        {-0.8f, 0.2f, 0.0f, 0.0f, 1.0f} ,
        {-0.8f, 0.0f, 0.0f, 1.0f, 1.0f} ,
        {-1.0f, 0.0f, 1.0f, 0.0f, 0.0f} ,
        // Right
        { 0.8f,-0.2f, 0.0f, 0.0f, 1.0f} ,
        { 0.8f, 0.2f, 0.0f, 1.0f, 0.0f} ,
        { 0.8f, 0.0f, 0.0f, 1.0f, 1.0f} ,
        { 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}
    };

    unsigned short ind[] = {
        // Top
        0, 1, 3,
        0, 3, 2,
        3, 1, 4,
        3, 4, 2,
        // Bottom
        0, 5, 7,
        0, 7, 6,
        7, 5, 8,
        7, 8, 6,
        // Left
        0, 9, 11,
        0, 11, 10,
        11, 9, 12,
        11, 12, 10,
        // Right
        0, 13, 15,
        0, 15, 14,
        15, 13, 16,
        15, 16, 14
    };

    GLuint VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)8);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint myIBO;
    glGenBuffers(1, &myIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

    string vss = readFile("vertex.sdr");
    string fss = readFile("fragment.sdr");
    const char *vertexShaderText = vss.c_str();
    const char *fragmentShaderText = fss.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderText, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    GLuint id_mt = glGetUniformLocation(shaderProgram, "mt");
    float rate = 0.02, control = 1.0;
    int graus = 3;
    bool crescendo = false;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniformMatrix3fv(id_mt, 1, GL_TRUE, m1.mptr);


        if(crescendo){
            m1.scale(1.0 + rate, 1.0 + rate);
            control *=( 1.0 + rate);
            m1.rotate(graus);
            if(control > 1.0)
                crescendo = false;
        }else{
            m1.scale(1.0 - rate, 1.0 - rate);
            control *= ( 1.0 - rate);
            m1.rotate(-graus);
            if(control < 0.05)
                crescendo = true;
        }
        /* C�digo de renderiza��o OpenGL vai aqui */
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_SHORT, (GLvoid*)0);
        /* Troca o buffer de fundo com o buffer de exibi��o */
        glfwSwapBuffers(window);
        framecount++;

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    t1.join();
    return 0;
}
