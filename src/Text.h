//==========Text.h==========
#ifndef Text_H_INCLUDED
#define Text_H_INCLUDED

//=================================
// included dependencies
#include <string>
#include "Texture.h"
#include <SDL_ttf.h>
//=================================

class Text
{
public:
    int _width, _height;
    TextureGrid _text_texture;
    TTF_Font * _font;
    std::string _string;
    float _red=1., _green=1., _blue=1., _alpha=1.;
    void init(std::string txt, TTF_Font * afont);
    void setColor(float red, float green, float blue, float alpha);
    void renderText( float x, float y, float z, Shader & shader );
    void resetText( std::string new_text );
};


#endif  // Text_H_INCLUDED
