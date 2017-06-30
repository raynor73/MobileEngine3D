precision mediump float;

attribute vec3 position;
attribute vec2 textureCoordinate;
attribute vec3 normal;

varying vec2 textureCoordinate0;
varying vec3 normal0;
varying vec3 worldPosition0;

uniform mat4 T_model;
uniform mat4 T_modelViewProjection;

void main() {
	gl_Position = T_modelViewProjection * vec4(position, 1);
	textureCoordinate0 = textureCoordinate;
	normal0 = (T_model * vec4(normal, 0)).xyz;
	worldPosition0 = (T_model * vec4(position, 1)).xyz;
}
