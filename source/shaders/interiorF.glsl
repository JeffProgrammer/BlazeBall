#version 120

varying vec2 UV;
varying vec3 normal;

varying vec3 position_world;
varying vec3 direction_camera;
varying vec3 light_camera;
varying vec3 normal_camera;
varying vec3 direction_tangent;
varying vec3 light_tangent;

varying vec3 tangent_camera;
varying vec3 bitangent_camera;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;
uniform sampler2D noiseSampler;

uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 sunPosition;
uniform float specularExponent;

void main() {
	//Texture
	vec3 materialColor = texture2D(textureSampler, UV).rgb;
	vec3 specularColor = texture2D(specularSampler, UV).rgb;
	vec3 normalColor = normalize(texture2D(normalSampler, UV).rgb * 2.0 - 1.0);

	//Cool MBU-style noise effect
	vec3 shade = texture2D(noiseSampler, floor(UV) / 32.0).rgb;

	//Lighting
	vec3 n = normalize(normalColor);
	vec3 l = normalize(light_tangent);

	//Cosine of the angle from the light to the normal
	float cosTheta = clamp(dot(n, l), 0.0, 1.0);

	//Eye vector (towards the camera)
	vec3 eye = normalize(direction_tangent);

	//Direction that light reflects
	vec3 reflect = reflect(-l, n);

	//Angle from the eye vector and reflect vector
	float cosAlpha = clamp(dot(eye, reflect), 0.0, 1.0);
	

	//Diffuse
	vec3 color = materialColor * (lightColor.rgb * lightColor.a) * cosTheta;
	//Ambient
	color += materialColor * ambientColor.rgb;
	//Specular
	color += specularColor * (lightColor.rgb * lightColor.a) * pow(cosAlpha, specularExponent);

	//Shade
	color *= shade;

	gl_FragColor = vec4(color, 1);
}