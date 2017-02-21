//==========AnimationController.h==========
#ifndef ANIMATIONCONTROLLER_H_INCLUDED
#define ANIMATIONCONTROLLER_H_INCLUDED

//=================================
// included dependencies
#include "Texture.h"
//=================================

class AnimationController
{
private:
    int _counter = 0;
public:
    void init( TextureGrid * grid, float delay, int first_frame, int end_frame, int when_done);
    void update();
    void reset();
    void render( float x_pos, float y_pos, Shader & shader);

    TextureGrid * _grid;
    int _delay;
    int _first_frame;
    int _end_frame;
    int _current_frame;
    int _when_done;
    float _alpha_per_delay = 0.;
    float _current_alpha = 1.;
};

#endif  // ANIMATIONCONTROLLER_H_INCLUDED
