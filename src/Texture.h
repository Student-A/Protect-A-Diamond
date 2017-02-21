//==========Texture.h==========
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

//=================================
// included dependencies
#include "Api.h"
//=================================

class TextureGrid
{
public:
    void init( const char * dir, float clip_width, float clip_height, int sprites_per_row, int sprites_per_column, int grid_width, int grid_height, float centerx, float centery);
    void initTextTexture( GLuint TextureID, float width, float height, float centerx, float centery );
    void render(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, Shader & shader, int sprite_no);
    void scale(float x_factor, float y_factor, float centerx, float centery);
    void rotate(float angle, float centerx, float centery);
    void reflect( bool x, bool y, float centerx, float centery);
    void identity();
    GLuint _id;
    GLuint _vertex_data_id;
    GLuint _index_data_id;
    std::vector<GLuint> _clips_id;
    std::vector<Vertex> _data;

    float _clip_width;
    float _clip_height;
    float _center[2] = {0, 0};
    int _sprites_per_row;
    int _sprites_per_column;
    int _grid_width;
    int _grid_height;

    float _rotation_angle = 0.;
    float _rotation_center[2] = {0, 0};
    float _translate_x = 0.;
    float _translate_y = 0.;
    float _translate_z = 0.;
    float _scale_x = 1;
    float _scale_y = 1;
    float _scale_center[2] = {0, 0};
    float _shear_x = 0.;
    float _shear_y = 0.;
    float _shear_z = 0.;
    float _reflect_factor_x = 1;
    float _reflect_factor_y = 1;
    float _reflect_center[2] = {0, 0};
};

void StartDrawingTextures();
void EndDrawingTextures();

#endif  // TEXTURE_H_INCLUDED
