#pragma once

#include <iostream>
#include "glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) ClearError();\
    x;\
    ASSERT(ErrorCall(#x, __FILE__, __LINE__));

void ClearError();
bool ErrorCall(const char* name, const char* dir, int line);


class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh) const;
private:

};