#include "textureresource.h"

TextureResource::TextureResource()
{
	glGenTextures(1, &m_id);
}

TextureResource::~TextureResource()
{
	if (glIsTexture(m_id) == GL_TRUE)
		glDeleteTextures(1, &m_id);
}
