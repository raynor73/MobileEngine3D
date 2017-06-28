#include "texture.h"
#include <logwrapper.h>
#include <JpegDec.h>
#include <sstream>

using namespace std;
using namespace jpegutil;

const string Texture::TAG = "Texture";

unordered_map<string, weak_ptr<TextureResource>> Texture::s_loadedTextures;

Texture::Texture(const string &path)
{
	if (s_loadedTextures.count(path) > 0) {
		weak_ptr<TextureResource> textureResource = s_loadedTextures[path];
		if (textureResource.expired()) {
			s_loadedTextures.erase(path);
			loadTextureAndPutToCache(path);
		} else {
			Log::i("Texture", "Texture with filename:" + path + "already loaded, reusing texture");
			m_textureResource = textureResource.lock();
		}
	} else {
		loadTextureAndPutToCache(path);
	}
}

void Texture::bind()
{
	bind(0);
}

void Texture::bind(GLenum samplerSlot)
{
	glActiveTexture(GL_TEXTURE0 + samplerSlot);
	glBindTexture(GL_TEXTURE_2D, m_textureResource->id());
}

void Texture::loadTextureAndPutToCache(const string &path)
{
	JpegDec jpegDec;
	uint8_t *image;
	int imageWidth, imageHeight;

	if (jpegDec.decode(path.c_str(), image, imageWidth, imageHeight)) {
		m_textureResource = make_shared<TextureResource>();
		glBindTexture(GL_TEXTURE_2D, m_textureResource->id());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		/*stringstream sstream;
		sstream << "width: " << imageWidth << "; height: " << imageHeight;
		Log::d(TAG, sstream.str());*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		s_loadedTextures[path] = m_textureResource;

		free(image);
	}
}
