// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Take in our previous texture coordinates.
in vec3 FragPos;
in vec2 v_texCoord;
in vec3 myNormal;
in float D;

uniform vec3 color;
uniform vec3 lightPos; // Our light source position from where light is hitting this object
uniform vec3 viewPos;
uniform int xtoon;

// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 


vec3 CalcIntensityColor(float intensity);
void RenderWithoutXtoon();
void RenderWithXtoon();

vec3 normal;
vec3 lightDir;
float intensity;

void main()
{
	normal = normalize(myNormal);
	lightDir = normalize(lightPos - FragPos);
	intensity = max(dot(normal, lightDir), 0.0);

	// This is the border, which we do not want to be affected by the lighting
	if (color.x == 0.0 && color.y == 0.0 && color.z == 0.0) {
		FragColor = vec4(color, 1.0);
	}
	else {
		if (xtoon == 0) {
			RenderWithoutXtoon();
		}
		else if (xtoon == 1) {
			// These calculations are effected by the lighting
			RenderWithXtoon();
		}
	}
}

void RenderWithoutXtoon() {
	vec3 intensity_color = CalcIntensityColor(intensity);
	FragColor = vec4(intensity_color * color, 1.0);
}

void RenderWithXtoon() {
	vec2 texCoord = vec2(intensity, D);
	// Since we are rendering using a texture, we no longer use CalcIntensityColor
	vec3 tex_color = texture(u_DiffuseMap, texCoord).rgb;
	FragColor = vec4(tex_color, 1.0);
}

vec3 CalcIntensityColor(float intensity) {
	vec3 intensity_color;
	
	if(intensity > 0.75) {
		intensity_color=vec3(1.0,1.0,1.0);
	}
	if(intensity > 0.50) {
		intensity_color=vec3(0.66,0.66,0.66);
	}
	else if(intensity > 0.25) {
		intensity_color=vec3(0.33,0.33,0.33);
	}
	else {
		intensity_color=vec3(0.0,0.0,0.0);
	}
	return intensity_color;
}