precision mediump float;

varying vec2 textureCoordinate0;
varying vec3 worldPosition0;
varying mat3 tbnMatrix;

uniform vec3 C_eyePosition;
uniform sampler2D diffuse;
uniform sampler2D displacementMap;
uniform float displacementMapScale;
uniform float displacementMapBias;

uniform vec3 R_ambient;

#include "sampling.glh"

void main()
{
	vec3 directionToEye = normalize(C_eyePosition - worldPosition0);
	vec2 textureCoordinate = calculateParallaxTextureCoordinate(displacementMap, tbnMatrix, directionToEye,
																textureCoordinate0, displacementMapScale,
																displacementMapBias);
	gl_FragColor = texture2D(diffuse, textureCoordinate) * vec4(R_ambient, 1);
}
