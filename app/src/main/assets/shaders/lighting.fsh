varying vec2 textureCoordinate0;
varying vec3 worldPosition0;
varying mat3 tbnMatrix;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform float displacementMapScale;
uniform float displacementMapBias;

#include "lighting.glh"
