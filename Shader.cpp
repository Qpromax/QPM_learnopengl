#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    :m_file(filepath),m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);      //文件中搜索源码
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererID));
}



void Shader::SetUniform4f(const std::string& nam, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(nam), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string nam)
{
    if (m_UniformLocationCache.find(nam) != m_UniformLocationCache.end())
        return m_UniformLocationCache[nam];

    GLCALL(int location = glGetUniformLocation(m_RendererID, nam.c_str()));     //获取指定名称同一变量位置
    if (location == -1)
        std::cout << "warning[Shader.cpp]\tuniform does not exist";

    m_UniformLocationCache[nam] = location;

    return location;
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath) const
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

unsigned Shader::CompileShader(const std::string& source, unsigned int type) const
{
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    //const char* src = &source[0];
    GLCALL(glShaderSource(id, 1, &src, nullptr));             //设置着色器源代码
    GLCALL(glCompileShader(id));                              //编译着色器

    /*   发出错误信息   */
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

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const
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

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID));       //绑定着色器程序
}

void Shader::UnBind() const
{
}
