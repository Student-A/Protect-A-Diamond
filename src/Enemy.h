//==========Enemy.h==========
#ifndef Enemy_H_INCLUDED
#define Enemy_H_INCLUDED

//=================================
// included dependencies
#include "Texture.h"
#include "AnimationController.h"
//=================================

void VectorPopFront(std::vector<glm::vec2> & container);

class Enemy1
{
public:
    void init1( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color);
    void init2( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color);
    void init3( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color);
    void init4( float x_pos, float y_pos, float radius, float scale_size, int health, TextureGrid * grid, TextureGrid * chargegrid, glm::vec4 color);
    void render( Shader & shader );
    void move(float target_x, float target_y);
    void update();

    TextureGrid * _grids[2];
    AnimationController _animations[10];
    float _resultant_velocity = 1.5;
    float _size = 1.;
    int _state = 0;
    int _charge_cycles = 0;
    std::vector<glm::vec2> _after_positions;
    glm::vec2 _velocity;
    glm::vec2 _position;
    glm::vec4 _color;

    int _type = 0;
    int _after_effect_counter= 0;

    int _dash_period = 0;

    float _min_distance_from_tower = 160;
    float _max_distance_from_tower = 230;
    float _allowed_distance_from_tower;

    int _cool_down_after_hit = 0;

    int _health;
    float _radius;
};



#endif  // Enemy_H_INCLUDED

