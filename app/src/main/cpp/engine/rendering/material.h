#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>
#include <string>
#include <engine/core/vector3f.h>
#include <engine/rendering/resourcemanagement/mappedvalues.h>
#include "texture.h"

using namespace std;

class Material : public MappedValues
{
public:
	Material(Texture *diffuse, float specularIntensity, float specularPower, Texture *normalMap,
			 Texture *displacementMap, float displacementMapScale = 0, float displacementMapOffset = 0);

	void addTexture(const string &name, Texture *texture) { m_textures[name] = texture; }
	Texture *findTexture(const string &name) const;

	Material(const Material &) = delete;
	Material &operator =(const Material &) = delete;

private:
	unordered_map<string, Texture *> m_textures;
};

#endif // MATERIAL_H
