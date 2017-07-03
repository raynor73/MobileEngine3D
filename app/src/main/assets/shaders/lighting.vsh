attribute vec3 position;
attribute vec2 textureCoordinate;
attribute vec3 normal;
attribute vec3 tangent;

varying vec2 textureCoordinate0;
varying vec3 worldPosition0;
varying vec3 normal0;
varying vec3 tangent0;
varying mat3 tbnMatrix;

uniform mat4 T_model;
uniform mat4 T_modelViewProjection;

void main() {
	gl_Position = T_modelViewProjection * vec4(position, 1);
	textureCoordinate0 = textureCoordinate;
	worldPosition0 = (T_model * vec4(position, 1)).xyz;

	normal0 = normalize((T_model * vec4(normal, 0)).xyz);
	tangent0 = normalize((T_model * vec4(tangent, 0)).xyz);

	vec3 n = normalize((T_model * vec4(normal, 0)).xyz);
	vec3 t = normalize((T_model * vec4(tangent, 0)).xyz);

	t = normalize(t - dot(t, n) * n);

	vec3 biTangent = cross(t, n);

	tbnMatrix = mat3(t, biTangent, n);
}
