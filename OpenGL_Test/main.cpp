#include <GLFW/glfw3.h>

float x = -0.9, y = 0.0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT) x = x - 0.01;
    if (key == GLFW_KEY_RIGHT) x = x + 0.01;
    if (key == GLFW_KEY_UP) y = y + 0.01;
    if (key == GLFW_KEY_DOWN) y = y - 0.01;
}

int main(void) {
    GLFWwindow* window;
    bool north = true;
    bool east = true;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Protecao de Tela", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        // Definimos o estado de montagem de triângulos
        glBegin(GL_TRIANGLES);
            glColor3f(0, 0, 1);
            glVertex2f(x+0.0f, y+0.1f);
            glColor3f(0, 1, 1);
            glVertex2f(x+0.1f, y-0.1f);
            glColor3f(1, 0, 1);
            glVertex2f(x-0.1f, y-0.1f);
            /* Estando no estado de criação de triângulos,
            a cada três vértices definidos, o OpenGL
            monta um triângulo */
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        if(north){
            y = y + 0.0001;
            if(y + 0.1 > 1)
                north = false;
        }else{
            y = y - 0.0001;
            if(y - 0.1 < -1)
                north = true;
        }

        if(east){
            x = x + 0.0001;
            if(x + 0.1 > 1)
                east = false;
        }else{
            x = x - 0.0001;
            if(x - 0.1 < -1)
               east = true;
        }

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
