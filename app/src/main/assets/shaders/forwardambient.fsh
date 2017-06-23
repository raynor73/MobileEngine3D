precision mediump float;

//varying vec2 textureCoordinate0;

uniform vec3 R_ambient;
//uniform sampler2D diffuse;

void main()
{
	//gl_FragColor = texture2D(diffuse, textureCoordinate0.xy) * vec4(R_ambient, 1);
	gl_FragColor = vec4(R_ambient, 1);
}
