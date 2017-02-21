//==========ShardsEmitter.cpp==========
#import "ShardsEmitter.h"

void Shard::init( TextureGrid * texture, int life, glm::vec4 color_mask, float alpha_decomposition_rate,
                  float angular_velocity, float velocity, float direction_angle, float scale_size, glm::vec2 position )
{
    _position = position;
    _size = scale_size;
    _texture = texture;
    _life = life;
    _current_color_mask = color_mask;
    _alpha_decomposition_rate = alpha_decomposition_rate;
    _angular_velocity = angular_velocity;
    _resultant_velocity = velocity;
    _direction_angle = direction_angle;
    _velocity.x = cosf(_direction_angle)*_resultant_velocity;
    _velocity.y = sinf(_direction_angle)*_resultant_velocity;
}

void Shard::update()
{
    if (_life <= 0) return;
    _current_color_mask.w -= _alpha_decomposition_rate;
    if (_current_color_mask.w < 0) _current_color_mask.w = 0;

    _current_angle += _angular_velocity;
    _position += _velocity;

    _life --;
}

void Shard::render( Shader & shader)
{
    _texture->scale( _size, _size, _texture->_center[0], _texture->_center[1]);
    _texture->rotate( _current_angle, _texture->_center[0], _texture->_center[1] );
    shader.setColor( _current_color_mask.x, _current_color_mask.y, _current_color_mask.z, _current_color_mask.w );
    _texture->render(_position.x, _position.y, 0., shader, 0);
}


void ShardsEmitter::generate( int number_of_shards, int life, glm::vec2 dispersion_angles, float average_size, float size_uncertainity,
                               glm::vec4 average_color, glm::vec4 color_uncertainity, TextureGrid * texture, glm::vec2 position)
{
    _position = position;
    _shards.resize(number_of_shards);
    _life = life;
    for (int i = 0; i < number_of_shards; i++)
    {
        glm::vec4 color_mask;

        color_mask.x = average_color.x + Randfloat(-color_uncertainity.x, color_uncertainity.x);
        color_mask.y = average_color.y + Randfloat(-color_uncertainity.y, color_uncertainity.y);
        color_mask.z = average_color.z + Randfloat(-color_uncertainity.z, color_uncertainity.z);
        color_mask.w = average_color.w + Randfloat(-color_uncertainity.w, color_uncertainity.w);


        _shards[i].init( texture, life,
                color_mask, 0.02, Randfloat(-1., 1), 5., Randfloat(dispersion_angles.x, dispersion_angles.y),
                average_size+Randfloat(-size_uncertainity, size_uncertainity), _position );
    }

}

void ShardsEmitter::renderShards( Shader & shader)
{
    if (_life > 0)
    {
        for (int i = 0; i < _shards.size(); i++)
        {
            _shards[i].render(shader);
        }
    }
}

void ShardsEmitter::updateShards()
{
    if (_life > 0)
    {
        for (int i = 0; i < _shards.size(); i++)
        {
            _shards[i].update();
        }
    }
    _life --;
}
