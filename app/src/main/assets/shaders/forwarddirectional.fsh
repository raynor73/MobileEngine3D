precision mediump float;

#include "lighting.fsh"

uniform DirectionalLight R_directionalLight;

vec4 calculateLightingEffect(vec3 normal, vec3 worldPosition)
{
	return calculateDirectionalLight(R_directionalLight, normal, worldPosition);
}

#include "lightingMain.fsh"
