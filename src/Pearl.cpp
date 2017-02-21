//==========Pearl.cpp==========
#include "Pearl.h"

void Pearl::updateFrame()
{
    if (_state == 1)
    {
        if (_current_animation != 1)
        {
            _animations[1].reset();
            _current_animation = 1;
            _current_charge_level = 0;
        }
        if (_animations[1]._current_frame == 2)
        {
            _current_charge_level = 1;
        }
        if (_animations[1]._current_frame == 5)
        {
            _current_charge_level = 2;
        }
        if (_animations[1]._current_frame == 9)
        {
            _current_charge_level = 3;
        }
        if (_animations[1]._current_frame == 17)
        {
            _current_charge_level = 4;
        }
    }
    if (_state == 0 || _state == 2)
    {
        _current_animation = 0;
    }
    if (_rage_mode && _state == 1)
    {
        _current_charge_level = 4;
        if (_animations[1]._current_frame < 17)
        {
            _animations[1]._current_frame = 17;
        }
    }

}

void Pearl::init( float x_position, float y_position, float scale_size, float radius )
{
    _position[0] = x_position;
    _position[1] = y_position;
    _size = scale_size;
    _radius = radius * scale_size;

    _grids[0].init( "assets/pearl.png", 128, 128, 1, 1, 128, 128, 64, 64 );
    _grids[1].init( "assets/pearlcharge.png", 134, 134, 3, 6, 512, 1024, 64, 64 );
    _grids[2].init( "assets/pearldasheffect.png", 198, 188, 5, 3, 1024, 1024, 96, 94 );

    _grids[0].scale( _size, _size, 64, 64 );
    _grids[1].scale( _size, _size, 64, 64 );
    _grids[2].scale( _size, _size, 96, 94 );

    _animations[0].init( &_grids[0], 0, 0, 0, 0);
    _animations[1].init( &_grids[1], 9, 0, 17, 17);
    _animations[2].init( &_grids[2], 2, 0, 14, 0);

    _animations[3].init( &_grids[0], 2, 0, 0, 0);
    _animations[3]._current_alpha = 0.7;
    _animations[4].init( &_grids[0], 2, 0, 0, 0);
    _animations[4]._current_alpha = 0.6;
    _animations[5].init( &_grids[0], 2, 0, 0, 0);
    _animations[5]._current_alpha = 0.5;
    _animations[6].init( &_grids[0], 2, 0, 0, 0);
    _animations[6]._current_alpha = 0.4;
    _animations[7].init( &_grids[0], 2, 0, 0, 0);
    _animations[7]._current_alpha = 0.3;
    _animations[8].init( &_grids[0], 2, 0, 0, 0);
    _animations[8]._current_alpha = 0.2;
    _animations[9].init( &_grids[0], 2, 0, 0, 0);
    _animations[9]._current_alpha = 0.1;

}

void Pearl::render( Shader & shader)
{
    _animations[_current_animation].render(_position[0], _position[1], shader);
    _animations[_current_animation].update();

    if (_state == 1 && _current_charge_level == 4)
    {
        _animations[2].render(_position[0], _position[1], shader);
        _animations[2].update();
    }
}



void Pearl::move( float target_x, float target_y )
{
    float angle_difference = (atan((target_y-_position[1])/(target_x-_position[0])));

    if (_state == 2)
    {
        _velocity[0] = fabs(cosf(angle_difference)*_charge_velocities[_current_charge_level-1]);
        _velocity[1] = fabs(sinf(angle_difference)*_charge_velocities[_current_charge_level-1]);
    }
    else
    {
        _velocity[0] = fabs(cosf(angle_difference)*_resultant_velocity);
        _velocity[1] = fabs(sinf(angle_difference)*_resultant_velocity);
    }

    if (_state == 1)
    {
        _velocity[0] =0;
        _velocity[1] =0;
    }

    if (target_x == _position[0] && target_y == _position[1])
    {
        _velocity[0] = 0;
        _velocity[1] = 0;
    }

    if (target_x > _position[0])
    {
        _position[0] += _velocity[0];
        if (target_x < _position[0])
            _position[0] = target_x;
    }
    if (target_x < _position[0])
    {
        _position[0] -= _velocity[0];
        if (target_x > _position[0])
            _position[0] = target_x;
    }
    if (target_y > _position[1])
    {
        _position[1] += _velocity[1];
        if (target_y < _position[1])
            _position[1] = target_y;
    }
    if (target_y < _position[1])
    {
        _position[1] -= _velocity[1];
        if (target_y > _position[1])
            _position[1] = target_y;
    }
}
