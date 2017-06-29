precision mediump float;

#include "lighting.glh"

varying vec2 textureCoordinate0;
varying vec3 normal0;
varying vec3 worldPosition0;

uniform sampler2D diffuse;
uniform DirectionalLight R_directionalLight;

void main() {
	gl_FragColor =
			texture2D(diffuse, textureCoordinate0.xy) *
			calculateDirectionalLight(R_directionalLight, normalize(normal0), worldPosition0);
}
