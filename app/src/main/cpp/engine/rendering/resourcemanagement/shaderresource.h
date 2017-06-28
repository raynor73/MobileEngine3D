#ifndef SHADERRESOURCE_H
#define SHADERRESOURCE_H

#include <glwrapper.h>
#include <string>

using namespace std;

class ShaderResource
{
public:
	ShaderResource();
	~ShaderResource();

	GLuint programReference() const { return m_programReference; }

	ShaderResource(const ShaderResource &) = delete;
	ShaderResource &operator =(const ShaderResource &) = delete;

private:
	static const string TAG;

	GLuint m_programReference;
};

#endif // SHADERRESOURCE_H
