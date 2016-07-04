
#include <iostream>

// C++ library for creating windows with OpenGL contexts and receiving 
// input and events http://www.glfw.org/ 
#include <GLFW/glfw3.h> 

// C++ binding for the OpenGL API. 
// https://github.com/cginternals/glbinding
#include <glbinding/Binding.h>

#include <cgutils/common.h>

#include "particles.h"


// From http://en.cppreference.com/w/cpp/language/namespace:
// "Unnamed namespace definition. Its members have potential scope 
// from their point of declaration to the end of the translation
// unit, and have internal linkage."
namespace
{

auto example = Particles();

const auto canvasWidth = 1440; // in pixel
const auto canvasHeight = 900; // in pixel

// "The size callback ... which is called when the window is resized."
// http://www.glfw.org/docs/latest/group__window.html#gaa40cd24840daa8c62f36cafc847c72b6
void resizeCallback(GLFWwindow * /*window*/, int width, int height)
{
    example.resize(width, height);
}

// "The key callback ... which is called when a key is pressed, repeated or released."
// http://www.glfw.org/docs/latest/group__input.html#ga7e496507126f35ea72f01b2e6ef6d155
void keyCallback(GLFWwindow * /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{

    switch (key)
    {
    case GLFW_KEY_LEFT:
        example.rotate(example.angle() + 0.02f);
        break;

    case GLFW_KEY_RIGHT:
        example.rotate(example.angle() - 0.02f);
        break;
    }

    if (action != GLFW_RELEASE)
        return;

    switch (key)
    {
    case GLFW_KEY_F5:
        example.loadShaders();
        break;

    case GLFW_KEY_SPACE:
        example.pause();
        break;
    }
}


// "In case a GLFW function fails, an error is reported to the GLFW 
// error callback. You can receive these reports with an error
// callback." http://www.glfw.org/docs/latest/quick.html#quick_capture_error
void errorCallback(int errnum, const char * errmsg)
{
    std::cerr << errnum << ": " << errmsg << std::endl;
}


}


int main(int /*argc*/, char ** /*argv*/)
{
    if (!glfwInit())
    {
        return 1;
    }

    glfwSetErrorCallback(errorCallback);

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(canvasWidth, canvasHeight, "", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return 2;
    }

    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    std::cout << "Particles (CPU/GPU)" << std::endl << std::endl;

    std::cout << "Key Binding: " << std::endl
        << "  [F5] reload shaders" << std::endl
        << "  [Space] pause processing (toggle)" << std::endl
        << "  [Left] rotate left" << std::endl
        << "  [Right] rotate right" << std::endl
        << std::endl;

    glfwMakeContextCurrent(window);

    glbinding::Binding::initialize(false);

    example.resize(canvasWidth, canvasHeight);
    example.initialize();

    while (!glfwWindowShouldClose(window)) // main loop
    {
        glfwPollEvents();

        example.render();

        glfwSwapBuffers(window);
    }

    example.cleanup();

    glfwMakeContextCurrent(nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
