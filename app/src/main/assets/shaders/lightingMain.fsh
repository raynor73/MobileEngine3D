#include "sampling.glh"

void main()
{
	vec3 directionToEye = normalize(C_eyePosition - worldPosition0);
	vec2 textureCoordinate = calculateParallaxTextureCoordinate(displacementMap, tbnMatrix, directionToEye,
																textureCoordinate0, displacementMapScale,
																displacementMapBias);

	vec3 normal = normalize(tbnMatrix * (255.0 / 128.0 * texture2D(normalMap, textureCoordinate).xyz - 1.0));
	gl_FragColor = texture2D(diffuse, textureCoordinate) * calculateLightingEffect(normal, worldPosition0);
}
