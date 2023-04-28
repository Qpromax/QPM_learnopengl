#include "glew.h"
#include "glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"



int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);    //刷新模式

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error!" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        float position[] = {           //顶点数据
            -0.5f, -0.5f, // 0
            0.5f, -0.5f,  // 1
            0.5f, 0.5f,   // 2
            -0.5f, 0.5f,  // 3
            0.0f, 0.0f
        };

        float position2[] = {
            0.25f, 0.25f,
            0.25f, -0.25f,
            -0.25f, 0.25f,
            -0.25f, -0.25f,
            0.0f, 0.0f
        };

        unsigned int indices[] = {
            2, 4, 1,
            0, 4, 3
        };

        unsigned int indices2[] = {
            2, 3, 4,
            4, 0, 1
        };

        VertexArray va;
        VertexBuffer vb(position, 5 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader sh("Res/shaders/basic.sha");

        va.UnBind();
        sh.UnBind();
        vb.UnBind();
        ib.UnBind();

        Renderer renderer;

        float var = 0.005f;
        float vec1 = 0.0f;
        float vec2 = 1.0f;

        VertexArray va2;
        VertexBuffer vb2(position2, 2*5*sizeof(float));
        VertexBufferLayout layout2;
        layout2.Push<float>(2);
        va2.AddBuffer(vb2, layout2);
        IndexBuffer ib2(indices2, 6);
        Shader sh2("Res/shaders/basic.sha");
        va2.UnBind();
        vb2.UnBind();
        ib2.UnBind();
        sh2.UnBind();


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))

        {
            /* Render here */

            sh.Bind();
            sh.SetUniform4f("u_color", 0.75f*vec1, 0.75f*vec2, 0.75f*vec1, 0.75f*vec2);
            
            sh2.Bind();
            sh2.SetUniform4f("u_color", vec1, vec2, vec1, vec2);

            renderer.Draw(va, ib, sh);
            renderer.Draw(va2, ib2, sh2);

            if (vec1 >= 1.0)
                var = -0.002f;
            else if (vec1 <= 0.0)
                var = 0.002f;
            vec1 += var;
            vec2 -= var;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}