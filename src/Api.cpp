#include "Api.h"

int CURSOR_POSITION[2];


API::API()
{
    // Initialize SDL
	SDL_Init( SDL_INIT_EVERYTHING );

	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	// Input States Array
	_key_states = SDL_GetKeyboardState(NULL);

	// Open Audio device
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
        std::cout << "Mix_OpenAudio ERROR: " << Mix_GetError() << std::endl;
    else
        std::cout << "Mix_OpenAudio SUCCESS" << std::endl;

    // Set Volume
    Mix_VolumeMusic(100);

}


void API::init( int window_width, int window_height, const char * title)
{
    _window_width = window_width;
	_window_height= window_height;

	_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

	// Initialize OpenGL
	SDL_GL_CreateContext(_window);

    glewExperimental=GL_TRUE;
    glewInit();

    glClearColor( 1.f, 0.f, 0.f, 1.f);

    /**< Shaders related */
    SHADERS[0].loadShaders("shaders/0.vert", "shaders/0.frag");

    glUseProgram(SHADERS[0]._shader_program_id);
    SHADERS[0].setProjection( glm::ortho<GLfloat>( 0.0, float(window_width), float(window_height), 0., -1.0, 1.0 ) );
    SHADERS[0].updateProjection();

    SHADERS[0].setModelView( glm::mat4() );
    SHADERS[0].updateModelView();

    SHADERS[0].setTextureUnit( 0 );
    SHADERS[0].enableTexCoordPointer();
    SHADERS[0].enableVertexPointer();

    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}


void API::flipWindow()
{
	SDL_GL_SwapWindow( _window );
}


void API::clearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


void API::delayFPS()
{
    if ( (SDL_GetTicks()-_count_time) < (1000/ _fps) )
    {
        SDL_Delay((1000./(float) _fps) - (SDL_GetTicks()-_count_time));
    }
    //_running_fps=Stringify(float(1000/(SDL_GetTicks()-_count_time)));
    _count_time = SDL_GetTicks();
}


void API::handleEvents()
{
    if( SDL_PollEvent(&_event) )
    {
        if (_event.type == SDL_QUIT)
        {
            _is_trying_to_exit = true;
        }

        if (_event.type == SDL_MOUSEMOTION)
        {
            CURSOR_POSITION[0] = _event.motion.x;
            CURSOR_POSITION[1] = _event.motion.y;
        }
        _is_left_mouse_button_clicked = false;
        if (_event.type == SDL_MOUSEBUTTONDOWN)
        {
                if (_event.button.button == 1)
                {
                    _left_mouse_button_down = true;
                }
                if (_event.button.button == 0)
                {
                    _right_mouse_button_down = true;
                }
        }

        if (_event.type == SDL_MOUSEBUTTONUP)
        {
            if (_event.button.button == 1)
            {
                _left_mouse_button_down = false;
                _is_left_mouse_button_clicked = true;
            }
            if (_event.button.button == 0)
            {
                _right_mouse_button_down = false;
            }
        }
    }
}


GLuint InitializeVBOs(std::vector<Vertex> & vertices)
{
    GLuint VBOid;
    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    return VBOid;
}

GLuint InitializeTCBOs( std::vector<TexCoord> & coords)
{
    GLuint TCBOid;
    glGenBuffers(1, &TCBOid);
    glBindBuffer(GL_ARRAY_BUFFER, TCBOid);
    glBufferData(GL_ARRAY_BUFFER, coords.size()*sizeof(TexCoord), coords.data(), GL_STATIC_DRAW);
    return TCBOid;
}

GLuint InitializeIBOs(std::vector<GLuint> & indices)
{
    GLuint IBOid;
    glGenBuffers(1, &IBOid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    return IBOid;
}

GLuint InitializeVBOs(VertexData * verticesdata , int number)
{
    GLuint VBOid;
    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, number*sizeof(VertexData), verticesdata, GL_DYNAMIC_DRAW);
    return VBOid;
}

GLuint GenerateTextTexture(TTF_Font  * afont, std::string text, float red, float green, float blue, float alpha, int *getwidth, int *getheight)
{
	SDL_Color text_color = { 255, 255, 255 };
	SDL_Surface * image=TTF_RenderText_Blended( afont, text.c_str(), text_color );
	*getwidth=image->w;
	*getheight=image->h;
	GLuint texture_id;
	GLenum tex_format;
	if (image->format->BytesPerPixel==4)
    {
        if (image->format->Rmask == 0x000000ff)
            tex_format=GL_RGBA;
        else
            tex_format=GL_BGRA;
    }
    else if (image->format->BytesPerPixel==3)
    {
        if (image->format->Rmask == 0x000000ff)
            tex_format=GL_RGB;
        else
            tex_format=GL_BGR;
    }
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel, image->w, image->h,
                 0, tex_format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, NULL);
    return texture_id;
}

GLuint LoadTexture(std::string dir)
{
    GLuint texture_id;
    SDL_Surface * image = IMG_Load( dir.c_str() );
    GLenum tex_format;
	if (image->format->BytesPerPixel==4)
    {
        if (image->format->Rmask == 0x000000ff)
            tex_format=GL_RGBA;
        else
            tex_format=GL_BGRA;
    }
    else if (image->format->BytesPerPixel==3)
    {
        if (image->format->Rmask == 0x000000ff)
            tex_format=GL_RGB;
        else
            tex_format=GL_BGR;
    }
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel, image->w, image->h,
                 0, tex_format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, NULL);
    return texture_id;
}
