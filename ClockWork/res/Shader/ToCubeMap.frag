#version 460 core

in vec3 FragmentPosition;

out vec4 FragColor;

uniform sampler2D EquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(FragmentPosition));
	vec3 color = texture(EquirectangularMap, uv).rgb;

	FragColor = vec4(color,1);

}