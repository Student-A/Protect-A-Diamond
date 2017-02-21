#include "Api.h"
#include "Text.h"


void Text::init(std::string txt, TTF_Font  * afont)
{
	_font = afont;
	_string = txt;
	GLuint text_texture = GenerateTextTexture( _font, _string, 1., 1., 1., 1., &_width, &_height );
	_text_texture.initTextTexture( text_texture, _width, _height, 0, 0 );
}

void Text::resetText( std::string new_text )
{
	_string = new_text;
	GLuint text_texture = GenerateTextTexture( _font, _string, 1., 1., 1., 1., &_width, &_height );
	_text_texture.initTextTexture( text_texture, _width, _height, 0, 0 );
}


void Text::setColor(float red, float green, float blue, float alpha)
{
	_red=red;
	_green=green;
	_blue=blue;
	_alpha=alpha;
}

void Text::renderText( float x, float y, float z, Shader & shader )
{
    shader.setColor(_red, _green, _blue, _alpha);
    _text_texture.render(x, y, z, shader, 0);
}
