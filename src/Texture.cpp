#include "Texture.h"
#include "Api.h"


void TextureGrid::init(const char * dir, float clip_width, float clip_height,
                   int sprites_per_row, int sprites_per_column,
                   int grid_width, int grid_height, float centerx, float centery
)
{
    _id = LoadTexture( dir );
    _clip_width = clip_width;
    _clip_height = clip_height;
    _sprites_per_row = sprites_per_row;
    _sprites_per_column = sprites_per_column;
    _grid_width = grid_width;
    _grid_height = grid_height;

    _center[0] = centerx;
    _center[1] = centery;

    _data.resize(4);

    _data[0].x = 0; _data[0].y = 0; _data[0].z = 0;
    _data[1].x = _clip_width; _data[1].y = 0; _data[1].z = 0;
    _data[2].x = _clip_width; _data[2].y = _clip_height; _data[2].z = 0;
    _data[3].x = 0.; _data[3].y = _clip_height; _data[3].z = 0;
    _vertex_data_id = InitializeVBOs(_data);


    std::vector<TexCoord> coords;
    coords.resize(4);
    _clips_id.resize(sprites_per_column*sprites_per_row);
    for (int col=0; col < sprites_per_column; col++)
	{
		for (int row=0; row < sprites_per_row; row++)
		{
		    coords[0].s = (row*clip_width)/(float)grid_width;
		    coords[0].t = (col*clip_height)/(float)grid_height;
		    coords[1].s = ((row*clip_width)+clip_width)/(float)grid_width;
		    coords[1].t = (col*clip_height)/(float)grid_height;
		    coords[2].s = ((row*clip_width)+clip_width)/(float)grid_width;
		    coords[2].t = ((col*clip_height)+clip_height)/(float)grid_height;
		    coords[3].s = (row*clip_width)/(float)grid_width;
		    coords[3].t = ((col*clip_height)+clip_height)/(float)grid_height;
			_clips_id[(row+(sprites_per_row*col))] = InitializeTCBOs( coords );
		}
	}

    std::vector<GLuint> index_data = {0, 1, 2, 3};
    _index_data_id = InitializeIBOs(index_data);
}

void TextureGrid::initTextTexture( GLuint TextureID, float width, float height, float centerx, float centery )
{
    _id = TextureID;
    _clip_width = width;
    _clip_height = height;
    _sprites_per_row = 1;
    _sprites_per_column = 1;
    _grid_width = width;
    _grid_height = height;

    _center[0] = centerx;
    _center[1] = centery;

    _data.resize(4);

    _data[0].x = 0; _data[0].y = 0; _data[0].z = 0;
    _data[1].x = _clip_width; _data[1].y = 0; _data[1].z = 0;
    _data[2].x = _clip_width; _data[2].y = _clip_height; _data[2].z = 0;
    _data[3].x = 0.; _data[3].y = _clip_height; _data[3].z = 0;
    _vertex_data_id = InitializeVBOs(_data);


    std::vector<TexCoord> coords;
    coords.resize(4);
    _clips_id.resize(_sprites_per_column*_sprites_per_row);
    for (int col=0; col < _sprites_per_column; col++)
	{
		for (int row=0; row < _sprites_per_row; row++)
		{
		    coords[0].s = (row*_clip_width)/(float)_grid_width;
		    coords[0].t = (col*_clip_height)/(float)_grid_height;
		    coords[1].s = ((row*_clip_width)+_clip_width)/(float)_grid_width;
		    coords[1].t = (col*_clip_height)/(float)_grid_height;
		    coords[2].s = ((row*_clip_width)+_clip_width)/(float)_grid_width;
		    coords[2].t = ((col*_clip_height)+_clip_height)/(float)_grid_height;
		    coords[3].s = (row*_clip_width)/(float)_grid_width;
		    coords[3].t = ((col*_clip_height)+_clip_height)/(float)_grid_height;
			_clips_id[(row+(_sprites_per_row*col))] = InitializeTCBOs( coords );
		}
	}

    std::vector<GLuint> index_data = {0, 1, 2, 3};
    _index_data_id = InitializeIBOs(index_data);
}

void TextureGrid::render(GLfloat xpos, GLfloat ypos, GLfloat zpos, Shader & shader, int sprite_no)
{
    GLenum err;
    shader.setModelView( glm::mat4());

    shader.multiplyModelView(glm::translate(glm::vec3(xpos-_center[0], ypos-_center[1], zpos)));

    /**< Rotation */
    shader.multiplyModelView(glm::translate(glm::vec3(-(_rotation_center[0]-_center[0]+xpos), -(_rotation_center[1]-_center[1]+ypos), 0.)));
    shader.multiplyModelView(glm::rotate( _rotation_angle, glm::vec3(0, 0, 1)));
    shader.multiplyModelView(glm::translate(glm::vec3((_rotation_center[0]-_center[0]+xpos), (_rotation_center[1]-_center[1]+ypos), 0.)));


    /**< Scale */
    shader.multiplyModelView(glm::translate(glm::vec3(-(_scale_center[0]-_center[0]+xpos), -(_scale_center[1]-_center[1]+ypos), 0.)));
    shader.multiplyModelView(glm::scale(glm::vec3(_scale_x, _scale_y, 1.)));
    shader.multiplyModelView(glm::translate(glm::vec3((_scale_center[0]-_center[0]+xpos), (_scale_center[1]-_center[1]+ypos), 0.)));


    /**< Reflection */
    shader.multiplyModelView(glm::translate(glm::vec3(-(_reflect_center[0]-_center[0]+xpos), -(_reflect_center[1]-_center[1]+ypos), 0.)));
    shader.multiplyModelView(glm::scale(glm::vec3(_reflect_factor_x, _reflect_factor_y, 1.)));
    shader.multiplyModelView(glm::translate(glm::vec3((_reflect_center[0]-_center[0]+xpos), (_reflect_center[1]-_center[1]+ypos), 0.)));

    shader.updateModelView();

    glBindTexture(GL_TEXTURE_2D, _id);

    glBindBuffer(GL_ARRAY_BUFFER, _clips_id[sprite_no]);
    shader.shaderTexCoordPointer( sizeof(TexCoord), 0);

    glBindBuffer(GL_ARRAY_BUFFER, _vertex_data_id);
    shader.shaderVertexPointer( sizeof(Vertex), 0 );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_data_id);
    glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );
}

void StartDrawingTextures()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void EndDrawingTextures()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureGrid::scale(float x_factor, float y_factor, float centerx, float centery)
{
    _scale_center[0] = centerx;
    _scale_center[1] = centery;
    _scale_x = x_factor;
    _scale_y = y_factor;
}

void TextureGrid::rotate(float angle, float centerx, float centery)
{
    _rotation_angle = angle;
    _rotation_center[0] = centerx;
    _rotation_center[1] = centery;
}

void TextureGrid::reflect( bool x, bool y, float centerx, float centery)
{
    _reflect_center[0] = centerx;
    _reflect_center[1] = centery;
    _reflect_factor_x = x? -1:1;
    _reflect_factor_y = y? -1:1;
}

void TextureGrid::identity()
{
    _rotation_angle = 0;
    _reflect_factor_x = 1;
    _reflect_factor_y = 1;
    _scale_x = 1.;
    _scale_y = 1.;
}
