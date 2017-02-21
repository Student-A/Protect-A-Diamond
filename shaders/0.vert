#version 130

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

#if __VERSION__ >= 130

in vec3 VertexPos3D;

in vec2 TexCoord;
out vec2 OutTexCoord;

#else

attribute vec3 VertexPos3D;

attribute vec2 TexCoord;
varying vec2 OutTexCoord;

#endif

void main()
{
	OutTexCoord = TexCoord;
	
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPos3D.x, VertexPos3D.y, VertexPos3D.z, 1.0);
}

