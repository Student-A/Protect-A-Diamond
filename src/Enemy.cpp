//==========Enemy.cpp==========
#include "Enemy.h"
#include "Globals.h"

extern int ENEMIESCOUNT;

int Randint(int from , int to);

void Enemy1::init1( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color)
{
    _type = 1;
    _dash_period = 30;
    _size = scale_size;
    _color = color;
    _position.x = x_pos;
    _position.y = y_pos;
    _radius = radius * _size;
    _health = health;
    _grids[0] = grid;
    _grids[1] = chargegrid;

    _animations[0].init( _grids[0], 0, 0, 0, 0);
    _animations[2].init( _grids[1], 2, 0, 14, 0);

    _animations[3].init( _grids[0], 2, 0, 0, 0);
    _animations[3]._current_alpha = 0.6;
    _animations[4].init( _grids[0], 2, 0, 0, 0);
    _animations[4]._current_alpha = 0.4;
    _animations[5].init( _grids[0], 2, 0, 0, 0);
    _animations[5]._current_alpha = 0.2;

}

void Enemy1::init2( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color)
{
    _type = 2;
    _size = .5;
    _color = color;
    _position.x = x_pos;
    _position.y = y_pos;
    _radius = radius * _size;
    _health = health;
    _grids[0] = grid;
    _dash_period = 700;
    _state = 2;
    _resultant_velocity = 1.5;

    _animations[3].init( _grids[0], 2, 0, 0, 0);
    _animations[3]._current_alpha = 0.6;
    _animations[4].init( _grids[0], 2, 0, 0, 0);
    _animations[4]._current_alpha = 0.4;
    _animations[5].init( _grids[0], 2, 0, 0, 0);
    _animations[5]._current_alpha = 0.2;
}

void Enemy1::init3( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color)
{
    _type = 3;
    _size = .7;
    _color = color;
    _position.x = x_pos;
    _position.y = y_pos;
    _radius = radius * _size;
    _health = health;
    _grids[0] = grid;
    _state = 0;
    _allowed_distance_from_tower = Randint(70, 100);
}

void Enemy1::init4( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color)
{
    _allowed_distance_from_tower = Randint(300, 450);
    _type = 4;
    _dash_period = 80;
    _size = scale_size;
    _color = color;
    _position.x = x_pos;
    _position.y = y_pos;
    _radius = radius * _size;
    _health = health;
    _grids[0] = grid;
    _grids[1] = chargegrid;

    _resultant_velocity = 1.7;

    _animations[0].init( _grids[0], 0, 0, 0, 0);
    _animations[2].init( _grids[1], 2, 0, 14, 0);

    _animations[3].init( _grids[0], 2, 0, 0, 0);
    _animations[3]._current_alpha = 0.6;
    _animations[4].init( _grids[0], 2, 0, 0, 0);
    _animations[4]._current_alpha = 0.4;
    _animations[5].init( _grids[0], 2, 0, 0, 0);
    _animations[5]._current_alpha = 0.2;
}

void Enemy1::update()
{
    _cool_down_after_hit --;
    if (_cool_down_after_hit<0) _cool_down_after_hit = 0;
    if (_type == 3) return;
    if (_state != 2)
    {
        while (_after_positions.size() != 0)
        {
            _after_positions.pop_back();
        }
    }
    if (_state == 2)
    {
        if (_dash_period-- <= 0)
        {
            _state = 0;
            if (_type == 1)
            {
                _dash_period = 30;
                _resultant_velocity = 1.5;
            }
            if (_type == 4)
            {
                _dash_period = 80;
                _resultant_velocity = 1.7;
            }
            if (_type == 2)
            {
                _health = 0;
                ENEMIESCOUNT--;
            }
        }
        if (_after_effect_counter++ == 2)
        {
            if (_type == 1)
            {
                _resultant_velocity = 4;
            }
            if (_type == 4)
            {
                _resultant_velocity = 6;
            }
            _after_effect_counter = 0;
            _after_positions.push_back(_position);
            if (_after_positions.size() > 3)
            {
                VectorPopFront(_after_positions);
            }
        }
    }

    if (_type == 1 || _type == 4)
    {
        if (_state == 1 && _animations[2]._current_frame == 9)
            _charge_cycles ++;
        if (_state == 1 && _charge_cycles >= 7*3 && _type == 1)
        {
            _charge_cycles = 0;
            _state = 2;
        }
        if (_state == 1 && _charge_cycles >= 4*3 && _type == 4)
        {
            _charge_cycles = 0;
            _state = 2;
        }
    }
}

void Enemy1::move(float target_x, float target_y)
{
    if (_type == 1)
    {
        if (glm::distance(_position, glm::vec2(target_x, target_y)) < _allowed_distance_from_tower && _state != 2)
        {

            _state = 1;
            return;
        }
    }
    if (_type == 4)
    {
        if (_state != 2 && glm::distance(_position, glm::vec2(target_x, target_y)) >= _allowed_distance_from_tower)
        {
            _state = 1;
            return;
        }
    }

    if (_state == 1) return;

    float angle_difference = (atan((target_y-_position.y)/(target_x-_position.x)));
    if (_type != 2 || _resultant_velocity!= NONE )
    {
        _velocity.x = fabs(cosf(angle_difference)*_resultant_velocity);
        _velocity.y = fabs(sinf(angle_difference)*_resultant_velocity);
        if (_type == 2)
        {
            if (target_x < _position.x) _velocity.x *= -1;
            if (target_y < _position.y) _velocity.y *= -1;
        }
    }

    if (_type == 2)
    {
        _position += _velocity;
        _resultant_velocity = NONE;
        return;
    }

    if (_type == 4 && _state == 0)
    {
        _velocity.x *= -1;
        _velocity.y *= -1;
    }

    if (target_x > _position.x)
    {
        _position.x += _velocity.x;
        if (target_x < _position.x)
            _position.x = target_x;
    }
    if (target_x < _position.x)
    {
        _position.x -= _velocity.x;
        if (target_x > _position.x)
            _position.x = target_x;
    }
    if (target_y > _position.y)
    {
        _position.y += _velocity.y;
        if (target_y < _position.y)
            _position.y = target_y;
    }
    if (target_y < _position.y)
    {
        _position.y -= _velocity.y;
        if (target_y > _position.y)
            _position.y = target_y;
    }
}


void Enemy1::render( Shader & shader)
{
    shader.setColor( _color.x, _color.y, _color.z, _color.w);
    _grids[0]->scale(_size, _size, _grids[0]->_center[0], _grids[0]->_center[1]);
    _grids[0]->render( _position.x, _position.y, 0, shader, 0 );

    //shader.setColor( .1, .1, .1, .6);
    if (_state == 1)
    {
        _animations[2].render(_position.x, _position.y, shader);
        _animations[2].update();
    }

    if (_state == 2 && _after_positions.size() > 0)
    {
        for (int i = 0; i < _after_positions.size(); i++)
        {
            _animations[5-i].render(_after_positions[i].x, _after_positions[i].y, shader);
        }
    }

    _grids[0]->identity();

    shader.setColor( 1., 1., 1., 1.);
}
