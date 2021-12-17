#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
//#include "stb_image.h"

// Константы
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
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "lab_7", NULL, NULL);

    glfwMakeContextCurrent(window);

    // glad: загрузка всех указателей на OpenGL-функции
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    // Компилирование нашей шейдерной программы

    // Вершинный шейдер
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Фрагментный шейдер
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    // Связывание шейдеров
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Указывание вершин (и буферов) и настройка вершинных атрибутов
    float vertices[] = {
        -0.3f, -0.3f, 0.0f, // левая вершина
         0.3f, -0.3f, 0.0f, // правая вершина
         0.0f,  0.3f, 0.0f  // верхняя вершина   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    // Цикл рендеринга
    while (!glfwWindowShouldClose(window)) {
        // Рендеринг

        // Очищаем цветовой буфер
        glClearColor(0.78f, 0.40f, 0.99f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Убеждаемся, что активировали шейдер
        glUseProgram(shaderProgram);

        // Обновляем для uniform-переменной значение color
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

        // Переходим к рендерингу треугольников
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Обмен содержимым front- и back-буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
    glfwTerminate();
    return 0;
}