#ifndef TEXTURERESOURCE_H
#define TEXTURERESOURCE_H

#include <glwrapper.h>

class TextureResource
{
public:
	TextureResource();
	~TextureResource();

	GLuint id() const { return m_id; }

	TextureResource(const TextureResource &) = delete;
	TextureResource &operator =(const TextureResource &) = delete;

private:
	GLuint m_id;
};

#endif // TEXTURERESOURCE_H
