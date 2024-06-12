#include "WindowsWindow.h"
#include "core/Event/KeyEvent.h"
#include "core/Event/ApplicationEvent.h"
#include "core/Event/MouseEvent.h"
#include "Renderer/Renderer.h"
#include <iostream>

namespace Engine {
    std::unique_ptr<Window> Window::Create(const WindowProps & props) {
        return std::unique_ptr<Window>(new Platform::Windows::WindowsWindow(props));
    }

    namespace Platform::Windows {
        static bool s_GLFWInitialized = false;

        static void GLFWErrorCallback(int error, const char* desc) {
            std::cout << "GLFW Error ( " << error << " ): " << desc << "\n";
        }

        WindowsWindow::WindowsWindow(const WindowProps & props)
        {
            m_Data.width = props.width;
            m_Data.height = props.height;
            m_Data.Title = props.Title;

            if (!s_GLFWInitialized) {
                int success = glfwInit();
                if (!success) {
                    std::cout << "Failed to Initialize GLFW\n";
                }
                glfwSetErrorCallback(GLFWErrorCallback);
                s_GLFWInitialized = true;
            }

            //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            m_Window = glfwCreateWindow((int)m_Data.width, (int)m_Data.height, m_Data.Title.c_str(), nullptr, nullptr);

            m_Context = std::unique_ptr<Renderer::OpenGL::OpenGLContext>(new Renderer::OpenGL::OpenGLContext(m_Window));
            m_Context->Init();
            //Renderer::Renderer::OnWindowResize(m_Data.width, m_Data.height);

            glfwSetWindowUserPointer(m_Window, &m_Data);
            SetVSync(true);

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent e;
                data.callback(e);
                });

            glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowResizeEvent e(width, height);
                data.callback(e);

                data.width = width;
                data.height = height;
                });

            glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focus) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                if (focus == 0) {
                    WindowLostFocusEvent e;
                    data.callback(e);
                    return;
                }
                else {
                    WindowFocusEvent e;
                    data.callback(e);
                    return;
                }
                });

            glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.callback(event);
                    break;
                }
                }
                });

            glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.callback(event);
                    break;
                }
                }
                });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseMovedEvent e(x, y);
                data.callback(e);
                });

            glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseScolledEvent e(x, y);
                data.callback(e);
                });
        }

        void WindowsWindow::OnUpdate()
        {
            glfwPollEvents();
            m_Context->SwapBuffers();
        }

        unsigned int WindowsWindow::GetWidth() const
        {
            return m_Data.width;
        }
        unsigned int WindowsWindow::GetHeight() const
        {
            return m_Data.height;
        }

        float WindowsWindow::GetAspectRatio() const
        {
            return (float)m_Data.width / (float)m_Data.height;
        }

        void WindowsWindow::SetEventCallback(const EventCallBackFn & callback)
        {
            m_Data.callback = callback;
        }

        void WindowsWindow::SetVSync(bool enabled)
        {
            if (enabled)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);

            m_Data.vsync = enabled;
        }
        bool WindowsWindow::IsVSync() const
        {
            return m_Data.vsync;
        }

        void * WindowsWindow::GetNativeWindow() const
        {
            return (void*)m_Window;
        }
    }
}