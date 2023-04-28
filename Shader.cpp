#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    :m_file(filepath),m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);      //�ļ�������Դ��
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

    GLCALL(int location = glGetUniformLocation(m_RendererID, nam.c_str()));     //��ȡָ������ͬһ����λ��
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
    GLCALL(glShaderSource(id, 1, &src, nullptr));             //������ɫ��Դ����
    GLCALL(glCompileShader(id));                              //������ɫ��

    /*   ����������Ϣ   */
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
        GLCALL(program = glCreateProgram());       //�����������
        GLCALL(unsigned int vertex = CompileShader(vertexShader, GL_VERTEX_SHADER));
        GLCALL(unsigned int fragment = CompileShader(fragmentShader, GL_FRAGMENT_SHADER));

        GLCALL(glAttachShader(program, vertex));                //����ɫ�����ӵ��������
        GLCALL(glAttachShader(program, fragment));
        GLCALL(glLinkProgram(program));                         //���ӳ������
        GLCALL(glValidateProgram(program));                     //��֤�������       

        GLCALL(glDeleteShader(vertex));
        GLCALL(glDeleteShader(fragment));

        return program;
    }

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID));       //����ɫ������
}

void Shader::UnBind() const
{
}
