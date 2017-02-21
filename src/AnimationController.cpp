//==========AnimationController.cpp==========
#include "AnimationController.h"

void AnimationController::init( TextureGrid * grid, float delay, int first_frame, int end_frame, int when_done)
{
    _grid = grid;
    _delay = delay;
    _first_frame = first_frame;
    _end_frame = end_frame;
    _current_frame = first_frame;
    _when_done = when_done;
}

void AnimationController::update()
{
    if ( _counter++ == _delay)
    {
        _counter = 0;
        if (_current_frame == _end_frame)
        {
            _current_frame = _when_done;
            _current_alpha = 1.;
        }
        else
        {
            _current_frame += 1;
        }
    }
}

void AnimationController::render( float x_pos, float y_pos, Shader & shader)
{
    if (_alpha_per_delay != 0.)
    {
        _current_alpha -= _alpha_per_delay;
        shader.setColor(shader._red, shader._green, shader._blue, _current_alpha);

    }
    if (_current_alpha != 1.)
    {
        shader.setColor( shader._red, shader._green, shader._blue, _current_alpha );
    }
    _grid->render(x_pos, y_pos, 0, shader, _current_frame);
    if (_current_alpha != 1.)
    {
        shader.setColor( shader._red, shader._green, shader._blue, 1. );
    }
}

void AnimationController::reset()
{
    _current_frame = 0;
    _counter = 0;
}
