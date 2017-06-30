precision mediump float;

#include "lighting.glh"

varying vec2 textureCoordinate0;
varying vec3 normal0;
varying vec3 worldPosition0;

uniform sampler2D diffuse;
uniform SpotLight R_spotLight;

void main()
{
	gl_FragColor =
			texture2D(diffuse, textureCoordinate0.xy) *
			calculateSpotLight(R_spotLight, normalize(normal0), worldPosition0);
}
