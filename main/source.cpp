#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "iostream"
#include "fstream"
#include "sstream"
#include "string"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum shaderType
    {
        NONE = -1, VERTEX, FRAGMENT
    };

    std::string line;
    std::stringstream ss[2];
    shaderType type = shaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = shaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = shaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}


static unsigned CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id;
    GLCALL(id = glCreateShader(type));
    const char* src = source.c_str();
    //const char* src = &source[0];
    GLCALL(glShaderSource(id, 1, &src, nullptr));             //设置着色器源代码
    GLCALL(glCompileShader(id));                              //编译着色器

    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        //char* message = new [length * char];
        GLCALL(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "failed!" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

        GLCALL(glDeleteShader(id));

        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program;
    GLCALL(program = glCreateProgram());       //创建程序对象
    GLCALL(unsigned int vertex = CompileShader(vertexShader, GL_VERTEX_SHADER));
    GLCALL(unsigned int fragment = CompileShader(fragmentShader, GL_FRAGMENT_SHADER));

    GLCALL(glAttachShader(program, vertex));                //将着色器附加到程序对象
    GLCALL(glAttachShader(program, fragment));
    GLCALL(glLinkProgram(program));                         //链接程序对象
    GLCALL(glValidateProgram(program));                     //验证程序对象       
   
    GLCALL(glDeleteShader(vertex));
    GLCALL(glDeleteShader(fragment));

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);    //每帧刷新一次

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error!" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float position[] = {           //顶点数据
            -0.5f, -0.5f, // 0
            0.5f, -0.5f,  // 1
            0.5f, 0.5f,   // 2
            -0.5f, 0.5f,  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        /*
        unsigned int vao;       //顶点数组对象id
        glGenVertexArrays(1, &vao);     //生成
        glBindVertexArray(vao);     //绑定
        */

        VertexArray va;
        VertexBuffer vb(position, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //VertexBuffer v1(position, 10 * sizeof(float));           //缓冲区对象id

        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("Res/shaders/basic.sha");      //文件中搜索源码
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCALL(glUseProgram(shader));       //绑定着色器程序

        int location;
        GLCALL(location = glGetUniformLocation(shader, "u_color"));     //获取指定名称同一变量位置
        ASSERT(location != -1);
        GLCALL(glUniform4f(location, 0.0f, 1.0f, 0.5f, 1.0f));

        GLCALL(glBindVertexArray(0));
        GLCALL(glUseProgram(0));
        vb.UnBind();
        ib.UnBind();

        float var = 0.01f;
        float vec1 = 0.5f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            GLCALL(glUseProgram(shader));
            GLCALL(glUniform4f(location, 0.5f, vec1, 0.5f, 1.0f))
                va.Bind();
            ib.Bind();

            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (vec1 >= 1.0)
                var = -0.01f;
            else if (vec1 <= 0)
                var = 0.01f;
            vec1 += var;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        GLCALL(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}