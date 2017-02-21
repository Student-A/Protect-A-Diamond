//Plain polygon color 
#version 130

uniform vec4 Color;
uniform sampler2D TextureUnit;

#if __VERSION__ >= 130

in vec2 OutTexCoord;

out vec4 gl_FragColor;

#else

varying vec2 OutTexCoord;

#endif

void main()
{
	gl_FragColor = texture(TextureUnit, OutTexCoord) - (vec4(1., 1., 1., 1.) - Color);
}