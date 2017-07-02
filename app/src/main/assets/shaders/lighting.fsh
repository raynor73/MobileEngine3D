varying vec2 textureCoordinate0;
varying vec3 worldPosition0;
//varying vec3 normal0;
varying mat3 tbnMatrix;

uniform sampler2D diffuse;
uniform sampler2D normalMap;

#include "lighting.glh"
