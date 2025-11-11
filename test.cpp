#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = {
        // 위치              // 컬러
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 우측 하단
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 좌측 하단
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 위 
    };    
    Shader ourShader("shader.vs", "shader.fs");

    /*
    //행렬 조합해서 이동 시 스케일 -> 회전 -> 이동 순서로 하기를 권고. 왜냐하면 마지막에 스케일할 경우 이동,회전까지 스케일 되는 등 문제 발생
    //행렬 연산은 순서 꼭 생각하자 
    glm::mat4 trans; // 그대로 나오는 단위행렬
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); // 여기서 (0,0,1)을 축으로 90도 회전
    // trans = R * trans
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  // 그걸 또 받아와서 0.5배 스케일링
    //trans = S * (R * trans)
    //결국 벡터에 적용할 때에는 (S * R * trans) * V가 되므로 스케일 -> 회전을 담은 변환행렬과 곱하게 됨.
    // --> 순서 뒤집힘

    

    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    ourShader.use();
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); 
    
    // 두 번쨰 파라미터 : 행렬의 개수
    // 세 번째 파라미터 : 행과 열을 바꿀건지(GLM기본인 column-major ordering을 사용하므로 바꿀 필요 x)
    // 네 번째 파라미터 : 우리의 변환 행렬을 맞는 타입으로 변환해서 넣어주기

    */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);  

    glBindVertexArray(VAO);


    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glm::mat4 trans;
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f)); // 이동 
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}