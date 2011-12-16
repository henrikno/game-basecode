#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cassert>
#include <stdexcept>
#ifdef _WIN32
#include "stdint.h"
#include <windows.h>
#endif

using namespace std;
using namespace sf;

#define glError() checkGL(__FILE__, __LINE__)
inline void checkGL(const char* method, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error @" << method << ":" << line << " : " << gluErrorString(err) << std::endl;
        assert(false);
    }
}
