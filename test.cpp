#include <glad/glad.h> // 가장 먼저 include 필요
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{// 창 크기가 바뀔 때마다 viewport값 바꾸기 위해 호출되는 콜백함수
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window) // 인풋 받기 예제, 루프에 들어간다. 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 안 눌렸으면 GLFW_RELEASE return
        glfwSetWindowShouldClose(window, true);
}

float vertices[] = { // 정점배열
     0.5f,  0.5f, 0.0f,  // 우측 상단
     0.5f, -0.5f, 0.0f,  // 우측 하단
    -0.5f, -0.5f, 0.0f,  // 좌측 하단
    -0.5f,  0.5f, 0.0f   // 좌측 상단
};
unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
    0, 1, 3,   // 첫 번째 삼각형
    1, 2, 3    // 두 번째 삼각형
};   // ebo에서 사용할 배열

const char *vertexShaderSource = "#version 330 core\n" // opengl버전에 맞는 GLSL버전
    "layout (location = 0) in vec3 aPos;\n" // vec3 형식의 aPos 변수 만들기, 입력 변수 location = 0인 애들 aPos에 넣을게~
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // 아무것도 안 하고 gl_Position이라는 타입으로 return
    "}\0"; // vertexShader 코드, GLSL이라는 언어

const char *fragmentShaderSource = "#version 330 core\n" // 
    "out vec4 FragColor;\n" // 컬러값 벡터
    "void main()\n"
    "{\n"
   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 일단은 그냥 주황색으로, RGBA값. 0~1
    "}\0";

int main(){
    // glfw 창 초기화
    glfwInit();
    // glfwWindowHint는 GLFW의 설정을 하는 함수다, 첫 번쨰 파라미터가 변수명, 두 번쨰 파라미터가 값이다.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // opengl 버전 앞자리
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // opengl 버전 뒷자리, 즉 우리는 opengl버전 3.3을 사용할거라는 뜻이다
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 대충 코어에 접근할거라는 뜻, 다른 버전도 있나봄
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); //가로, 세로, 창이름, 일단무시1, 일단무시2
    // GLFWwindow의 포인터로서 window선언 glfwCreateWindow함수는 window를 만들고 포인터를 반환
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //window를 우리 메인 윈도우로 쓸거라고 알려주기

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // 대충 glad 초기화
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    



    // vertexShader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // 쉐이더 생성

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 소스코드 설정
    glCompileShader(vertexShader); // 컴파일(쉐이더도 코드니까)

    int  success; // 컴파일 에러로그를 위한 부분
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // vertexShader(정점 쉐이더) 끝

    // fragmentShader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "암튼 fragment 쉐이더 에러\n" << infoLog << std::endl;
    }
    // fragmentShader (픽셀 색상 쉐이더) 끝

    //shader progrem -> 쉐이더끼리 연결, 최종 결과 쉐이더
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
        //쉐이더 프로그램 컴파일 로그
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "암튼 쉐이더 프로그램 에러\n" << infoLog << std::endl;
    }
    // glUseProgram(shaderProgram); // 이제 쉐이더 이거 쓸게요~
        //shader progrem에 연결했으니 이제 필요없음
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);  
    //shader progrem 끝




    unsigned int VBO;// visual buffer object, 한 번에 많은 양의 정점 데이터를 GPU에 보내기 위한 배열(버퍼)
    glGenBuffers(1, &VBO); //임의의 버퍼 생성
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    // ..:: 초기화 코드 (한번만 실행됩니다(오브젝트가 자주 변경되지 않는 한)) :: ..
    // 1. Vertex Array Object 바인딩
    glBindVertexArray(VAO);
    // 2. OpenGL이 사용하기 위해 vertex 리스트를 복사
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 그런 다음 vertex 속성 포인터를 세팅
    unsigned int EBO; // 삼각형을 구성할 점들의 정점배열에서의 인덱스를 저장하는 배열 EBO, element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // 즉, VAO에 VBO의 설정들을 저장한 후 나중에는 VAO를 사용하면 거기서 설정과 연결된 VBO를 가져오니 VAO를 사용하면 된다는거고, 
    // 그렇게 VAO에 여러 개의 VBO설정,포인터 들을 저장할 수 있다는거
    // 말은 vertex array이지만 정확히는 설정값과 위치를 알고있는 주소록같은 느낌

    /*
    unsigned int EBO; // 삼각형을 구성할 점들의 정점배열에서의 인덱스를 저장하는 배열 EBO, element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    //나머지는 다른 배열들과 같고 여기서 바인딩하는 배열은 GL_ELEMENT_ARRAY_BUFFER라는것만 신경써주자.
    //VBO랑 마찬가지로 VAO에 저장해서 가져와 쓸 수 있다.
    */


    glViewport(0, 0, 800, 600); // 윈도우 설정이다, 처음 두 0,0은 왼쪽 아래 모서리의 위치이고 800,600은 가로 세로 넓이이다.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 창 사이즈 변경 콜백함수

    while(!glfwWindowShouldClose(window)) //render loop, 게임의 mainloop와 비슷한 개념
    {
        processInput(window); // 인풋 체크

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 초기화 컬러 설정
        glClear(GL_COLOR_BUFFER_BIT); // 버퍼 초기화, 여러 버퍼가 있지만 지금은 컬러 버퍼만

        glUseProgram(shaderProgram); // 사용할 쉐이더
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // glBindVertexArray(VAO); // VAO연결해서 우리의 VBO와 연결(VBO가 GL_ARRAY_BUFFER에 연결됨)
        // glDrawArrays(GL_TRIANGLES, 0, 3); // 그 GL_ARRAY_BUFFER을 어떻게 그릴지, 0번쨰 칸부터 3개의 정점을 삼각형으로 그릴거야!
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // EBO 연결
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // EBO써서 그리는 명령어(glDrawArrays의 EBO버전)
        // // (그릴 도형, 그릴 정점 개수(삼각형 2개니까 6개), 인덱스의 타입(왜 설정하는지 아직 모르겠음, 이터레이터가 있나?), 마지막은 offset(시작점)
        


        glfwSwapBuffers(window);
        // 그냥 버퍼값에 따라 화면 다시 그리는건데 화면을 그냥 다시 그리는게 아닌 백그라운드에서 다 그리고 원래 화면이랑 교채하는 방식이라 Swap인것
        glfwPollEvents(); // 이벤트 체크
    }


    glfwTerminate(); // 그냥 종료, 자원 정리 함수. free같은거임
    return 0;
}