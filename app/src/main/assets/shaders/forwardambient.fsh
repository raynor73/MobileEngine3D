precision mediump float;

varying vec2 textureCoordinate0;
varying vec3 worldPosition0;
varying vec3 normal0;
varying vec3 tangent0;
varying mat3 tbnMatrix;

uniform sampler2D diffuse;
uniform sampler2D normalMap;

uniform vec3 R_ambient;

vec4 calculateLightingEffect(vec3 normal, vec3 worldPosition)
{
	return vec4(R_ambient, 1);
}

#include "lightingMain.fsh"
