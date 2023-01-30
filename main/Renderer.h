#pragma once

#include "GL/glew.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) ClearError();\
    x;\
    ASSERT(ErrorCall(#x, __FILE__, __LINE__));

void ClearError();
bool ErrorCall(const char* name, const char* dir, int line);