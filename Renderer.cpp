
#include "Renderer.h"
#include<iostream>

void ClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool ErrorCall(const char* name, const char* dir, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[opengl error!]\t(" << error << "):" << name
            << "\t" << dir << ":" << line << std::endl;

        return false;
    }

    return true;
}

void Renderer::Clear() const
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh) const
{
    sh.Bind();
    va.Bind();
    ib.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
