#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 310 es
#extension GL_EXT_clip_cull_distance : require

layout(location = 0) in vec3 aPos;

out float gl_ClipDistance[1];

void main()
{
    gl_Position = vec4(aPos, 1.0);

    // Define clip distances for demonstration
    gl_ClipDistance[0] = aPos.x + 0.5;

}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 310 es

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
}
)";

// Function to compile a shader
GLuint compileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// Function to create a shader program
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process all input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Function to get OpenGL function pointers
void* GetGLProc(const char* name) {
    void* proc = (void*)glfwGetProcAddress(name);
    if (!proc) {
        std::cerr << "Failed to get OpenGL function: " << name << std::endl;
    }
    return proc;
}

// Function to check if an extension is supported
bool isExtensionSupported(const char* extension) { 
    if (glfwExtensionSupported(extension)) { 
        return true; 
    } 
    return false; 
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW context version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

   

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Retrieve the glGetStringi function pointer
    PFNGLGETSTRINGIPROC glGetStringi = (PFNGLGETSTRINGIPROC)GetGLProc("glGetStringi");
    if (!glGetStringi) {
        std::cerr << "Cannot get glGetStringi proc" << std::endl;
        return -1;
    }

    // Check if GL_EXT_clip_cull_distance is supported 
    if (isExtensionSupported("GL_EXT_clip_cull_distance")) 
    { 
        std::cout << "GL_EXT_clip_cull_distance is supported" << std::endl; 
    } else { 
        std::cout << "GL_EXT_clip_cull_distance is NOT supported" << std::endl; 
    }


    // Set the viewport
    glViewport(0, 0, 800, 600);

    // Compile and link shaders
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Set the callback function for window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color
        glClear(GL_COLOR_BUFFER_BIT);          // Clear the color buffer

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
