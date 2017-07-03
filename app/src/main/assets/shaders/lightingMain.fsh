void main()
{
	vec3 normal = normalize(tbnMatrix * (255.0 / 128.0 * texture2D(normalMap, textureCoordinate0.xy).xyz - 1.0));
	gl_FragColor = texture2D(diffuse, textureCoordinate0.xy) * calculateLightingEffect(tangent0, worldPosition0);
}
