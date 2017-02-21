//==========Shader.h===========
#ifndef Shader_H_INCLUDED
#define Shader_H_INCLUDED

//=================================
// included dependencies
#define GLEW_STATIC

#include <fstream>
#include <string>
#include <SDL.h>
#include <glew.h>
#include "SDL_opengl.h"
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
//=================================

class Shader
{
private:
    GLuint _vertex_shader_id;
    GLuint _fragment_shader_id;

    /**< Uniforms */
    GLuint _AColor;
    GLuint _ATextureUnit;

    /**< Attributes (in's) */
    GLuint _AVertexPos3D;
    GLuint _ATexCoord;

    GLuint _AProjectionMatrix;
    GLuint _AModelViewMatrix;

    glm::mat4 ProjectionMatrix;
    glm::mat4 ModelViewMatrix;

    void printShaderLog( GLuint shader );
    void printProgramLog( GLuint program );
public:
    float _red = 1.; float _green = 1.; float _blue = 1.; float _alpha = 1.;
    GLuint _shader_program_id;
    void loadShaders( const char * directory1, const char * directory2 );

    void shaderVertexPointer( GLsizei stride, const GLvoid * data);
    void shaderTexCoordPointer( GLsizei stride, const GLvoid * data);

    void enableVertexPointer();
    void enableTexCoordPointer();
    void disableVertexPointer();
    void disableTexCoordPointer();

    void setProjection( glm::mat4 projection_matrix );
    void setModelView( glm::mat4 model_view_matrix );

    void multiplyProjection (glm::mat4 matrix );
    void multiplyModelView ( glm::mat4 matrix );

    void rotateModelView( GLfloat angle, glm::vec3 center, glm::vec3 components );
    void scaleModelView( glm::vec3 center, glm::vec3 scale_factor );
    void reflectModelView( glm::vec3 center, bool x, bool y, bool z);

    void scaleProjection(glm::vec3 center, glm::vec3 scale_factor );

    void updateProjection();
    void updateModelView();

    void setTextureUnit( GLuint unit );
    void setColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
};

#endif
