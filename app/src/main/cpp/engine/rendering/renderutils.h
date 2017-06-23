#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include <string>
#include <engine/core/vector3f.h>
#include <glwrapper.h>

using namespace std;

namespace RenderUtils
{

GLint glGetShader(GLuint shader, GLenum pname);
GLint glGetProgram(GLuint program, GLenum pname);
string _glGetProgramInfoLog(GLuint program);
string _glGetShaderInfoLog(GLuint shaderReference);
void printUniforms(GLuint program);

}

#endif // RENDERUTILS_H
