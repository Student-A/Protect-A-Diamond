//==========Pearl.h==========
#ifndef PEARL_H_INCLUDED
#define PEARL_H_INCLUDED

//=================================
// included dependencies
#include "Texture.h"
#include "AnimationController.h"
//=================================


class Pearl
{
public:
    void init( float x_position, float y_position, float scale_size, float radius );
    void render( Shader & shader );
    void updateFrame();
    void move( float target_x, float target_y );

    TextureGrid _grids[5];
    AnimationController _animations[10];

    int _state = 0;

    bool _charging = false;
    int _current_charge_level = 0;

    int _current_animation = 0;

    float _radius;
    int _health;
    float _size = 1.;
    float _velocity[2] = {0, 0};
    float _resultant_velocity = 7.;
    float _charge_velocities[4] = {10, 15, 25, 65};
    float _charge_damage[4] = {2, 3, 5, 10};
    float _position[2] = {0, 0};
    bool _rage_mode = false;

};

#endif  // PEARL_H_INCLUDED
