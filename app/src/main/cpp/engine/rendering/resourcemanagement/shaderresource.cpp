#include "shaderresource.h"
#include <logwrapper.h>
#include <stdexcept>

using namespace std;

const string ShaderResource::TAG = "ShaderResource";

ShaderResource::ShaderResource()
{
	m_programReference = glCreateProgram();

	if (m_programReference == 0 || !glIsProgram(m_programReference)) {
		Log::e("ShaderResource", "Error creating shader program");
		throw new runtime_error("Error creating shader program");
	}
}

ShaderResource::~ShaderResource()
{
	if (glIsProgram(m_programReference) == GL_TRUE)
		glDeleteProgram(m_programReference);
}
