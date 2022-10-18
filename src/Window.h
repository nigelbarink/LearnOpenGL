#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdio.h>

class Window 
{
    public:
        Window (int initialWidth, int initialHeight, const std::string& WindowName);
        ~Window();

        void setWidth(int width){this->m_width = width;}
        void setHeight(int height){this->m_height = height;}

        int getWidth(){return this->m_width;}
        int getHeight(){return this->m_height;}
        double getRatio(){return (this->m_width / this->m_height);}

        bool shouldClose(){ return glfwWindowShouldClose(m_window);}
        //void GetInput(){ processInput(m_window);}
        void SwapBuffers(){ glfwSwapBuffers(m_window);}
    private:
        int m_width, m_height;
        const std::string& m_WindowName;
        GLFWwindow* m_window;
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);