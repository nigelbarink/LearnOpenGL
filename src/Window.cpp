#include "Window.h"

bool firstMouse  = true;
float lastx = 400, lasty = 300;


Window::Window(int initialWidth, int initialHeight, const std::string& WindowName) 
: m_width(initialWidth), m_height(initialHeight), m_WindowName(WindowName)
{
    glfwInit();

    // setup the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Enable window based multisampling 
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(m_width, m_height, m_WindowName.c_str(), NULL, NULL);

    if(m_window == NULL)
    {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize GLAD!\n");
        return;
    }

    glViewport(0,0, m_width, m_height);

    // Setup Callbacks
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
}


Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse){
        lastx = xpos;
        lasty = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastx;
    float yoffset = ypos - lasty;

    lastx = xpos;
    lasty - ypos;

    // camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width,height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    // camera.ProcessMouseScroll(yoffset);
}