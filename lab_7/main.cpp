#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
//#include "stb_image.h"

// ���������
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform vec2 ourBias;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + ourBias.x, -aPos.y + ourBias.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

int main() {
    // glfw: ������������� � ����������������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw: �������� ����
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "lab_7", NULL, NULL);

    glfwMakeContextCurrent(window);

    // glad: �������� ���� ���������� �� OpenGL-�������
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    // �������������� ����� ��������� ���������

    // ��������� ������
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ����������� ������
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    // ���������� ��������
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ���������� ������ (� �������) � ��������� ��������� ���������
    float vertices[] = {
        -0.3f, -0.3f, 0.0f, // ����� �������
         0.3f, -0.3f, 0.0f, // ������ �������
         0.0f,  0.3f, 0.0f  // ������� �������   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // ������� ��������� ������ ���������� �������, ����� ��������� � ������������� ��������� �����(�), � ����� ������������� ��������� �������(�)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    // ���� ����������
    while (!glfwWindowShouldClose(window)) {
        // ���������

        // ������� �������� �����
        glClearColor(0.78f, 0.40f, 0.99f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ����������, ��� ������������ ������
        glUseProgram(shaderProgram);

        // ��������� ��� uniform-���������� �������� color
        float timeValue = glfwGetTime();
        float redValue = cos(3 * timeValue) / 2.0f + 0.5f;
        float greenValue = cos(3 * timeValue) / 4.0f + 0.75f;
        float blueValue = cos(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

        float biasValueX = cos(3 * timeValue) / 2.0f;
        float biasValueY = sin(3 * timeValue) / 2.0f;
        int vertexBiasLocation = glGetUniformLocation(shaderProgram, "ourBias");
        glUniform2f(vertexBiasLocation, biasValueX, biasValueY);

        // ��������� � ���������� �������������
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ����� ���������� front- � back-�������. ������������ ������� �����/������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
    glfwTerminate();
    return 0;
}