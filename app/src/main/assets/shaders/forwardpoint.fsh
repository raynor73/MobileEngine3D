precision mediump float;

#include "lighting.fsh"

uniform PointLight R_pointLight;

vec4 calculateLightingEffect(vec3 normal, vec3 worldPosition)
{
	return calculatePointLight(R_pointLight, normal, worldPosition);
}

#include "lightingMain.fsh"
