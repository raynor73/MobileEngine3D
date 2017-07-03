#include "material.h"

Material::Material(Texture *diffuse, float specularIntensity, float specularPower, Texture *normalMap,
				   Texture *displacementMap, float displacementMapScale, float displacementMapOffset)
{
	addTexture("diffuse", diffuse);
	addTexture("normalMap", normalMap);
	addFloat("specularIntensity", specularIntensity);
	addFloat("specularPower", specularPower);
	addTexture("displacementMap", displacementMap);
	float baseBias = displacementMapScale / 2.0f;
	addFloat("displacementMapScale", displacementMapScale);
	addFloat("displacementMapBias", -baseBias + baseBias * displacementMapOffset);
}

Texture *Material::findTexture(const string &name) const
{
	if (m_textures.count(name) > 0)
		return m_textures.at(name);
	else
		return nullptr;
}
