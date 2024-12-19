#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

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

    // Get the number of extensions
    GLint numExtensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    if (numExtensions > 0) {
        std::vector<std::string> extensions;

        // Get extensions one by one
        for (GLint i = 0; i < numExtensions; i++) {
            const GLubyte* extension = glGetStringi(GL_EXTENSIONS, i);
            extensions.push_back(reinterpret_cast<const char*>(extension));
        }

        // Print all extensions
        for (const auto& ext : extensions) {
            std::cout << ext << std::endl;
        }
    }
    else {
        std::cout << "No extensions found." << std::endl;
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
