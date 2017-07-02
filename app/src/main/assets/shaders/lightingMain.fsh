void main()
{
	gl_FragColor =
			texture2D(diffuse, textureCoordinate0.xy) *
			calculateLightingEffect(normalize(normal0), worldPosition0);
}
