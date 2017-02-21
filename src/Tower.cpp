//==========Tower.cpp==========
#include "Tower.h"

void Tower::init( float x_pos, float y_pos, float radius, int health, float scale_size)
{
    _position[0] = x_pos;
    _position[1] = y_pos;
    _size = scale_size;
    _radius = radius* _size;
    _health = health;
}

void Tower::render( Shader & shader)
{
    _grid.rotate(_facing_angle, _grid._center[0], _grid._center[1]);
    _grid.render( _position[0], _position[1], 0, shader, 0);
}

void Tower::move( float target_x, float target_y )
{
    float angle_difference = (atan((target_y-_position[1])/(target_x-_position[0])));
    _facing_angle = angle_difference;
    if (_position[0]-target_x < 0 )
    {
        _facing_angle+=3.1415;
    }
    if (glm::distance(glm::vec2(target_x, target_y), glm::vec2(_position[0], _position[1])) < _min_distance_from_target)
        return;

    _velocity[0] = fabs(cosf(angle_difference)*_resultant_velocity);
    _velocity[1] = fabs(sinf(angle_difference)*_resultant_velocity);



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
