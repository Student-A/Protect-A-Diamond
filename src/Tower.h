//==========Tower.h==========
#ifndef Tower_H_INCLUDED
#define Tower_H_INCLUDED

//=================================
// included dependencies
#include "Texture.h"
#include "AnimationController.h"
//=================================

class Tower
{
public:
    TextureGrid _grid;
    void init( float x_pos, float y_pos, float radius, int health, float scale_size);
    void render( Shader & shader);
    void move( float target_x, float target_y);


    float _min_distance_from_target = 120.;
    float _position[2];
    glm::vec2 _velocity;
    float _resultant_velocity = 3.;
    float _size;
    int _health;
    float _radius;
    float _facing_angle = 0;
};

#endif  // Tower_H_INCLUDED
