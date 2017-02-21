//==========ShardsEmitter.h==========
#ifndef ShardsEmitter_H_INCLUDED
#define ShardsEmitter_H_INCLUDED

//=================================
// included dependencies
#include "Texture.h"
//=================================

int Randint(int from, int to);
float Randfloat(float from, float to);

class Shard
{
public:
    void init(TextureGrid * texture, int life, glm::vec4 color_mask, float alpha_decomposition_rate, float angular_velocity, float velocity, float direction_angle, float scale_size, glm::vec2 position);
    void render(Shader & shader);
    void update();

    TextureGrid * _texture;
    float _current_angle = 0.;
    float _angular_velocity;
    int _life;
    float _alpha_decomposition_rate;
    glm::vec4 _current_color_mask;
    float _resultant_velocity;
    glm::vec2 _velocity;
    glm::vec2 _position;
    float _direction_angle;
    float _size;
};

class ShardsEmitter
{
public:
    void updateShards();
    void generate( int number_of_shards, int life, glm::vec2 dispersion_angles, float average_size, float size_uncertainity, glm::vec4 average_color, glm::vec4 color_uncertainity, TextureGrid * texture, glm::vec2 position);
    void renderShards( Shader & shader);



    std::vector<Shard> _shards;
    glm::vec2 _position;
    int _number_of_shards;
    int _life;
    float _average_size;
    glm::vec2 _dispersion_angles;
    float _size_uncertainity;
    glm::vec4 _average_color;
    glm::vec4 _color_uncertainity;
};

#endif  // ShardsEmitter_H_INCLUDED

