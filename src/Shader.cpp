#include "Shader.h"
#include <cstdio>
#include <glm/gtc/type_ptr.hpp>


void Shader::printProgramLog( GLuint program )
{
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		char* infoLog = new char[ maxLength ];

		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a program\n", program );
	}
}

void Shader::printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		char* infoLog = new char[ maxLength ];

		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a shader\n", shader );
	}
}


void Shader::loadShaders( const char * v_shader_dir, const char * f_shader_dir)
{
    _vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    _fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    GLint shaderCompiled = GL_FALSE;
    std::string content = "";
    std::ifstream v_shader( v_shader_dir );
    content.assign( ( std::istreambuf_iterator< char >( v_shader ) ), std::istreambuf_iterator< char >() );
    v_shader.close();
    glShaderSource(_vertex_shader_id, 1, (const GLchar**)&content, NULL);
    glCompileShader( _vertex_shader_id );

    //Check shader for errors
    shaderCompiled = GL_FALSE;
    glGetShaderiv( _vertex_shader_id, GL_COMPILE_STATUS, &shaderCompiled );
    if( shaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile shader %d!\n\nSource:\n%s\n", _vertex_shader_id, content.c_str() );
        printShaderLog( _vertex_shader_id );
        glDeleteShader( _vertex_shader_id );
        _vertex_shader_id = 0;
        return;
    }


    content = "";
    std::ifstream f_shader( f_shader_dir );
    content.assign( ( std::istreambuf_iterator< char >( f_shader ) ), std::istreambuf_iterator< char >() );
    f_shader.close();
    glShaderSource(_fragment_shader_id, 1, (const GLchar**)&content, NULL);
    glCompileShader( _fragment_shader_id );


    shaderCompiled = GL_FALSE;
    glGetShaderiv( _fragment_shader_id, GL_COMPILE_STATUS, &shaderCompiled );
    if( shaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile shader %d!\n\nSource:\n%s\n", _fragment_shader_id, content.c_str() );
        printShaderLog( _fragment_shader_id );
        glDeleteShader( _fragment_shader_id );
        _fragment_shader_id = 0;
        return;
    }


    _shader_program_id = glCreateProgram();
    glAttachShader( _shader_program_id, _fragment_shader_id);
    glAttachShader( _shader_program_id, _vertex_shader_id);
    glLinkProgram( _shader_program_id );

    glDeleteShader( _vertex_shader_id );
    glDeleteShader( _fragment_shader_id );

    //Get attributes locations
    _AVertexPos3D = glGetAttribLocation( _shader_program_id, "VertexPos3D" );
    if( _AVertexPos3D == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "VertexPos3D" );
    }

    _ATexCoord = glGetAttribLocation( _shader_program_id, "TexCoord" );
    if( _ATexCoord == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "TexCoord" );
    }

    //Get uniforms locations
    _AColor = glGetUniformLocation( _shader_program_id, "Color" );
    if( _AColor == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "Color" );
    }

    _ATextureUnit = glGetUniformLocation( _shader_program_id, "TextureUnit");
    if ( _ATextureUnit == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "TextureUnit" );
    }

    _AProjectionMatrix = glGetUniformLocation( _shader_program_id, "ProjectionMatrix");
    if ( _AProjectionMatrix == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "ProjectionMatrix" );
    }

    _AModelViewMatrix = glGetUniformLocation( _shader_program_id, "ModelViewMatrix");
    if ( _AModelViewMatrix == -1 )
    {
        printf( "%s is not a valid glsl program variable!\n", "ModelViewMatrix" );
    }

}


void Shader::setProjection( glm::mat4 projection_matrix )
{
    ProjectionMatrix = projection_matrix;
}

void Shader::setModelView ( glm::mat4 model_view_matrix )
{
    ModelViewMatrix = model_view_matrix;
}

void Shader::multiplyProjection( glm::mat4 matrix )
{
    ProjectionMatrix = matrix * ProjectionMatrix;
}

void Shader::multiplyModelView( glm::mat4 matrix )
{
    ModelViewMatrix = matrix * ModelViewMatrix;
}

void Shader::rotateModelView( GLfloat angle, glm::vec3 center, glm::vec3 components )
{
    multiplyModelView( glm::translate(-center) );
    multiplyModelView( glm::rotate( angle, components));
    multiplyModelView( glm::translate(center) );
}

void Shader::scaleModelView( glm::vec3 center, glm::vec3 scale_factor )
{
    multiplyModelView( glm::translate(-center) );
    multiplyModelView( glm::scale( scale_factor ));
    multiplyModelView( glm::translate(center) );
}

void Shader::reflectModelView( glm::vec3 center, bool x, bool y, bool z)
{
    int x_, y_, z_;
    x_ = x? -1: 1;
    y_ = y? -1: 1;
    z_ = z? -1: 1;
    multiplyModelView( glm::translate(-center) );
    multiplyModelView( glm::scale( glm::vec3( x_, y_, z_) ));
    multiplyModelView( glm::translate(center) );
}

void Shader::scaleProjection(glm::vec3 center, glm::vec3 scale_factor )
{
    multiplyProjection( glm::translate(-center) );
    multiplyProjection( glm::scale( scale_factor ));
    multiplyProjection( glm::translate(center) );
}

void Shader::updateProjection()
{
    glUniformMatrix4fv( _AProjectionMatrix, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix) );
}

void Shader::updateModelView()
{
    glUniformMatrix4fv( _AModelViewMatrix, 1, GL_FALSE, glm::value_ptr(ModelViewMatrix) );
}

void Shader::setColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    _red = r;
    _blue = b;
    _green = g;
    _alpha = a;
    glUniform4f(_AColor, r, g, b, a);
}

void Shader::setTextureUnit( GLuint unit )
{
    glUniform1i( _ATextureUnit, unit );
}

void Shader::enableVertexPointer()
{
    glEnableVertexAttribArray( _AVertexPos3D );
}

void Shader::disableVertexPointer()
{
    glDisableVertexAttribArray ( _AVertexPos3D );
}

void Shader::enableTexCoordPointer()
{
    glEnableVertexAttribArray( _ATexCoord );
}

void Shader::disableTexCoordPointer()
{
    glDisableVertexAttribArray( _ATexCoord );
}

void Shader::shaderTexCoordPointer( GLsizei stride, const GLvoid * data)
{
    glVertexAttribPointer( _ATexCoord, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void Shader::shaderVertexPointer( GLsizei stride, const GLvoid * data)
{
    glVertexAttribPointer( _AVertexPos3D, 3, GL_FLOAT, GL_FALSE, stride, data );
}
