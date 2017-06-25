#include "shaderresource.h"
#include <logwrapper.h>
#include <stdexcept>

using namespace std;

ShaderResource::ShaderResource()
{
	m_programReference = glCreateProgram();

	if (m_programReference == 0) {
		Log::e("ShaderResource", "Error creating shader program");
		throw new runtime_error("Error creating shader program");
	}
}

ShaderResource::~ShaderResource()
{
	glDeleteProgram(m_programReference);
}
