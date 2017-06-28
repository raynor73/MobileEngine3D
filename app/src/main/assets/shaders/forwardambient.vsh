precision mediump float;

attribute vec3 position;
attribute vec2 textureCoordinate;

varying vec2 textureCoordinate0;

uniform mat4 T_modelViewProjection;

void main() {
	gl_Position = T_modelViewProjection * vec4(position, 1);
	textureCoordinate0 = textureCoordinate;
}
