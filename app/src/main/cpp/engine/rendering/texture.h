#ifndef TEXTURE_H
#define TEXTURE_H

#include <istream>
#include <string>
#include <memory>
#include <unordered_map>
#include <glwrapper.h>
#include <engine/rendering/resourcemanagement/textureresource.h>

using namespace std;

class Texture
{
public:
	Texture(const string &path);

	Texture(const Texture &) = delete;
	Texture &operator=(const Texture &) = delete;

	void bind();
	void bind(GLenum samplerSlot);

private:
	static unordered_map<string, weak_ptr<TextureResource>> s_loadedTextures;

	shared_ptr<TextureResource> m_textureResource;

	void loadTextureAndPutToCache(const string &path);
};

#endif // TEXTURE_H
