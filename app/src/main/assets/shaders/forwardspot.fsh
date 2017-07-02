precision mediump float;

#include "lighting.fsh"

uniform SpotLight R_spotLight;

vec4 calculateLightingEffect(vec3 normal, vec3 worldPosition)
{
	return calculateSpotLight(R_spotLight, normal, worldPosition);
}

#include "lightingMain.fsh"
