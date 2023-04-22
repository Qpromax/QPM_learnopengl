#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void SetUniform4f(const std::string& nam, float f1, float f2, float f3, float f4);		//此处不可加const

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_RendererID;
	const std::string& m_file;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	ShaderProgramSource ParseShader(const std::string& filepath) const;
	unsigned CompileShader(const std::string& source, unsigned int type) const;
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const;

	int GetUniformLocation(const std::string nam);				//此处不可加const
};
