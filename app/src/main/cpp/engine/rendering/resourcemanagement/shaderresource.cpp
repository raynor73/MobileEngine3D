#include "shaderresource.h"
#include <logwrapper.h>
#include <stdexcept>
#include <sstream>

using namespace std;

const string ShaderResource::TAG = "ShaderResource";

ShaderResource::ShaderResource()
{
	m_programReference = glCreateProgram();

	stringstream sstream;
	sstream << "programReference #3: " << m_programReference << "; ";
	if (glIsProgram(m_programReference) == GL_TRUE)
		sstream << "glIsProgram(): true";
	else
		sstream << "glIsProgram(): false";
	Log::d(TAG, sstream.str());

	if (m_programReference == 0 || !glIsProgram(m_programReference)) {
		Log::e("ShaderResource", "Error creating shader program");
		throw new runtime_error("Error creating shader program");
	}
}

ShaderResource::~ShaderResource()
{
	// TODO Here is the issue
	stringstream sstream;
	sstream << "glDeleteProgram: " << m_programReference;
	Log::d(TAG, sstream.str());

	glDeleteProgram(m_programReference);
}
