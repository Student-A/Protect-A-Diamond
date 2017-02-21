//==========api.h==========
#ifndef API_H_INCLUDED
#define API_H_INCLUDED

//=================================
// included dependencies
#define GLEW_STATIC
#include <SDL.h>
#include <glew.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include "Eval.h"
#include <iostream>
#include <math.h>
#include "glm/glm.hpp"
#include "Shader.h"
#include "Sound.h"

//=================================


struct Vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};
struct TexCoord
{
    GLfloat s;
    GLfloat t;
};
struct VertexData
{
    Vertex v;
    TexCoord t;
};


class API
{
private:
    /**< Window Dimensions */
    int _window_width;
    int _window_height;

    /**< FPS control */
    int _count_time = 0;

    /**< Input related */
    const Uint8 * _key_states;
    SDL_Event _event;




public:
    int _fps = 60;

    SDL_Window * _window;

    Shader SHADERS[2];

    bool _is_trying_to_exit = false;
    bool _left_mouse_button_down = false;
    bool _right_mouse_button_down = false;
    bool _is_left_mouse_button_clicked = false;

    API();
    void handleEvents();
    void start();
    void delayFPS();
    void flipWindow();
    void clearWindow();
    void exitApplication();
    void init(int window_width, int window_height, const char * title);
};


GLuint InitializeVBOs(std::vector<Vertex> & vertices);

GLuint InitializeIBOs(std::vector<GLuint> & indices);

GLuint InitializeTCBOs( std::vector<TexCoord> & coords);

GLuint InitializeVBOs(VertexData * verticesdata , int number);

GLuint LoadTexture(std::string dir);

GLuint GenerateTextTexture(TTF_Font * afont, std::string text, float red, float green, float blue, float alpha, int *getwidth, int *getheight);



#endif  // api_H_INCLUDED
