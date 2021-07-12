// ==================================================================
#version 330 core
// Read in our attributes stored from our vertex buffer object
// We explicitly state which is the vertex information
// (The first 3 floats are positional data, we are putting in our vector)
layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals; // Our second attribute - normals.
layout(location=2)in vec2 texCoord; // Our third attribute - texture coordinates.
layout(location=3)in vec3 tangents; // Our third attribute - texture coordinates.
layout(location=4)in vec3 bitangents; // Our third attribute - texture coordinates.

// If we have texture coordinates we can now use this as well
out vec3 FragPos;
out vec2 v_texCoord;

out vec3 myNormal;
out float D;

uniform mat4 model; // Object space
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal_matrix;
uniform vec3 lightPos; // Our light source position from where light is hitting this object
uniform vec3 viewPos;  // Where our camera is
uniform int d_index;
uniform float camera_z;

float CalculateD();

float z_min;
float z_max;
vec3 newViewPos;

void main()
{
	z_min = 0;
	z_max = 10.0;
	
	mat4 mvp = projection * view * model;
	FragPos = normalize(vec3(view * model * vec4(position, 1.0)));
	newViewPos = normalize(vec3(view * model * vec4(viewPos, 1.0)));
	gl_Position = mvp * vec4(position, 1.0);

  	// Store the texture coordinaets which we will output to
  	// the next stage in the graphics pipeline.
  	v_texCoord = texCoord;
	myNormal = normalize(vec3(normal_matrix * vec4(normals, 1.0)));
	D = CalculateD();
}

float CalculateD() {	
	vec3 lightDir = normalize(lightPos - FragPos);
	float intensity = max(dot(myNormal, lightDir), 0.0);
	vec3 viewDir = normalize(newViewPos - FragPos);

	// Depth of field calculation
	if (d_index == 0) {
		float z = FragPos.x - camera_z;
		return 1 - log(camera_z / z_min)/log(z_max / z_min);
	}
	// Backlighting
	else if (d_index == 1) {
		return pow(dot(myNormal, viewDir), 2);
	}
	// Specular lighting
	else if (d_index == 2) {	
		vec3 refLightVector = normalize(lightDir - (2 *  intensity * myNormal));
		return pow(dot(viewDir, refLightVector), 2);
	}
}