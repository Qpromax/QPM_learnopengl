
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