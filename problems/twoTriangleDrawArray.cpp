#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

const int SCR_SOURCE_WIDTH  = 1920;
const int SCR_SOURCE_HEIGHT = 1080;

const int SCR_WIDTH  = 16 * 80;
const int SCR_HEIGHT =  9 * 90;

int success;       // DEBUG VALUE
char infoLog[512]; // DEBUG BUFFER

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 fragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    fragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
                                   "}\0";

void frameBufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    printf("RESIZE: %4d | %4d\n", width, height);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processColorScreen(GLFWwindow *window, float *r, float *g, float *b){
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { *r += 0.01f; if (*r > 1) *r = 0; }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) { *g += 0.01f; if (*g > 1) *g = 0; }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) { *b += 0.01f; if (*b > 1) *b = 0; }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core-profile setting
   
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TESTING WINDOW", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    //glfwSetWindowPos(window, SCR_SOURCE_WIDTH / 4, SCR_SOURCE_HEIGHT / 4); // TODO : Set the window at the center 
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        printf("Failed to initialize GLAD");
        return -1;
    }
   
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    
    float twoTriangleVertices[] = {
         0.5f,  0.5f, 0.0f,
         1.0f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
        -1.0f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int twoTriagIndices[] = {
        0, 1, 2,
        2, 3, 4
    };

    float squareVertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int squareIndices[] = {
        0, 1, 3,
        1, 2, 3 
    };

    // FIRST WE CREATE THE VERTEX ARRAY OBJECT THAT WILL STORE ALL THE SETTINGS
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // CREATION OF THE VERTEX BUFFER OBJECT THAT WILL STORE ALL THE VERTEX DATA
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(twoTriangleVertices), twoTriangleVertices, GL_STATIC_DRAW);

    // CREATION OF THE ELEMENT BUFFER OBJECT THAT WILL STORE THE twoTriagIndices OF THE VERTICES
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(twoTriagIndices), twoTriagIndices, GL_STATIC_DRAW);
   
    // WE SPECIFY HOW THE GPU SHOULD READ THE DATA USING THE (VERTEX ATTRIBUTE POINTER) AND THE WE ENABLE IT
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    ///////////////////// LOG SHADER ////////////////////////
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }
    /////////////////////////////////////////////////////////

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram (shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ///////////////////// LOG SHADER ////////////////////////
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::LINKING::SHADER::FAILED\n%s\n", infoLog);
    }
    /////////////////////////////////////////////////////////

    float r = 0; float g = 0; float b = 0;

    while (!glfwWindowShouldClose(window)){
        processInput(window);
        processColorScreen(window, &r, &g, &b);
        
        glClearColor(r, g, b, 1.0f);  // This functions is a state-setting func for "glClear()"
        glClear(GL_COLOR_BUFFER_BIT); // State-using function 

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
