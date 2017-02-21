#include "Universal.h"
#include "Texture.h"
#include "Sound.h"
#include "Text.h"
#include "Pearl.h"
#include "Tower.h"
#include "Enemy.h"
#include "Wave.h"
#include "ShardsEmitter.h"

extern int CURSOR_POSITION[2];

int GAMEOVER = 1;
Text GREAT, FANTASTIC, INCREDIBLE, BINGO;
float TEXT_TIMER;
float COMBO_TEXT_TIMER;
Text COMBOTEXT;
glm::vec2 COMBOTEXTPOS;
unsigned int SHATTERCOUNT = 0;
unsigned int SCORE = 0;
unsigned int WAVE = 0;
Text SCORETEXT;
Text HEALTHTEXT;
Text WAVETEXT;
int ENEMIESCOUNT;
int RAEGMODECOUNTER = 1;
float RAGETIMER;

std::vector<glm::vec2> trackpoints;
std::vector<ShardsEmitter> BROKENSHARDS;
TextureGrid SHARD;
TextureGrid ChargeEnemy;

Sound shatter_sound;

int CURRENT_WAVE = 1;
std::vector<Wave> WAVESINFO;

std::vector<Enemy1> AQEEQENEMIES;
std::vector<Enemy1> FAKEENEMIES;
std::vector<Enemy1> TOPAZENEMIES;
std::vector<Enemy1> PLATINUMENEMIES;

TextureGrid enemy1_grid, enemy2_grid, enemy3_grid, enemy4_grid;

struct ScreenShakeDetails
{
    void setDetails( glm::vec2 frequency, glm::vec2 amplitude, int cycles)
    {
        if (!_interruptable && _cycles != 0) return;
        _frequency = frequency;
        _amplitude = amplitude;
        _cycle = glm::vec2(0, 0);
        _direction = glm::vec2(1, 1);
        _cycles = cycles;
    }
    glm::vec2 _cycle = glm::vec2(0, 0);
    glm::vec2 _direction;
    glm::vec2 _frequency;
    glm::vec2 _amplitude;
    int _cycles = 0;
    bool _interruptable = true;
}SCREENSHAKEDETAILS;

void ShakeScreen( Shader & shader )
{
    if ( SCREENSHAKEDETAILS._cycles <= 0) return;
    SCREENSHAKEDETAILS._cycle.x += (6.282/SCREENSHAKEDETAILS._frequency.x);
    SCREENSHAKEDETAILS._cycle.y += (6.282/SCREENSHAKEDETAILS._frequency.y);

    if (SCREENSHAKEDETAILS._cycle.x >= 6.282 || SCREENSHAKEDETAILS._cycle.y >= 6.282)
    {
        if (SCREENSHAKEDETAILS._cycle.x >= 6.282) SCREENSHAKEDETAILS._cycle.x = 0;
        if (SCREENSHAKEDETAILS._cycle.y >= 6.282) SCREENSHAKEDETAILS._cycle.y = 0;
        SCREENSHAKEDETAILS._cycles --;
    }

    glm::vec3 translate_vec;
    translate_vec.x = sinf(SCREENSHAKEDETAILS._cycle.x)*SCREENSHAKEDETAILS._amplitude.x;
    translate_vec.y = sinf(SCREENSHAKEDETAILS._cycle.y)*SCREENSHAKEDETAILS._amplitude.y;
    translate_vec.z = 0.;
    if (SCREENSHAKEDETAILS._cycles<= 0)
    {
        if (!SCREENSHAKEDETAILS._interruptable) SCREENSHAKEDETAILS._interruptable = true;
        shader.setProjection(glm::ortho<GLfloat>( 0, float(WINDOW_WIDTH), float(WINDOW_HEIGHT), 0, -1.0, 1.0 ));
    }
    else
        shader.setProjection(glm::ortho<GLfloat>( translate_vec.x, float(WINDOW_WIDTH)+translate_vec.x, float(WINDOW_HEIGHT)+translate_vec.y, translate_vec.y, -1.0, 1.0 ));
    shader.updateProjection();
}

void GenerateWave( int number )
{
    if (number >= 30) return;
    AQEEQENEMIES.resize(0);
    FAKEENEMIES.resize(0);
    TOPAZENEMIES.resize(0);
    PLATINUMENEMIES.resize(0);
    int points = WAVESINFO[number]._points;

    float r;
    float g;
    float b;
    float radius;
    float x_point;
    float y_point;

    for (int i = 0; i < WAVESINFO[number]._must_types.size(); i++)
    {
        for (int j = 0; j < WAVESINFO[number]._must_counts[i]; j++)
        {
            Enemy1 enemy;
            switch (WAVESINFO[number]._must_types[i])
            {
            case AQEEQ:
                radius = Randint(500, 800);
                x_point = Randint( 0,  radius*2) -(radius-400);
                y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
                if (Randint(0, 1) == 0)
                    y_point *= -1;
                y_point += 300;
                enemy.init1(x_point, y_point, 32, .7 ,2, &enemy1_grid, &ChargeEnemy,  glm::vec4(.7, .05, .05, 1.));
                enemy._allowed_distance_from_tower = Randint(enemy._min_distance_from_tower, enemy._max_distance_from_tower);
                AQEEQENEMIES.push_back(enemy);
                points -= AQEEQPTS;
                break;
            case FAKE:
                r = float(Randint(0, 100))/100.;
                g = float(Randint(0, 100))/100.;
                b = float(Randint(0, 100))/100.;
                radius = Randint(500, 800);
                x_point = Randint( 0,  radius*2) -(radius-400);
                y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
                if (Randint(0, 1) == 0)
                    y_point *= -1;
                y_point += 300;
                enemy.init2(x_point, y_point, 24, 1 ,2, &enemy2_grid, &ChargeEnemy,  glm::vec4(r, g, b, 1.));
                FAKEENEMIES.push_back(enemy);
                points -= FAKEPTS;
                break;
            case TOPAZ:
                radius = Randint(500, 800);
                x_point = Randint( 0,  radius*2) -(radius-400);
                y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
                if (Randint(0, 1) == 0)
                    y_point *= -1;
                y_point += 300;
                enemy.init3(x_point, y_point, 48, 1 ,5, &enemy3_grid, &ChargeEnemy,  glm::vec4(.8, .9, .05, 1.));
                TOPAZENEMIES.push_back(enemy);
                points -= TOPAZPTS;
                break;
            case PLATINUM:
                radius = Randint(500, 800);
                x_point = Randint( 0,  radius*2) -(radius-400);
                y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
                if (Randint(0, 1) == 0)
                    y_point *= -1;
                y_point += 300;
                enemy.init4(x_point, y_point, 26, 1 , 9, &enemy4_grid, &ChargeEnemy,  glm::vec4(1., 1., 1., 1.));
                enemy._allowed_distance_from_tower = 400;
                PLATINUMENEMIES.push_back(enemy);
                points -= PLATINUMPTS;
                break;
            }
        }
    }
    std::vector<ENEMYTYPES> a;

    while (points > 0)
    {
        while (a.size()>0) a.pop_back();
        if ( points >= AQEEQPTS)
        for (int i = 0; i < WAVESINFO[number]._sticks[AQEEQ]; i++)
        {
            a.push_back( AQEEQ );
        }
        if ( points >= FAKEPTS)
        for (int i = 0; i < WAVESINFO[number]._sticks[FAKE]; i++)
        {
            a.push_back( FAKE );
        }
        if ( points >= TOPAZPTS)
        for (int i = 0; i < WAVESINFO[number]._sticks[TOPAZ]; i++)
        {
            a.push_back( TOPAZ );
        }
        if ( points >= PLATINUM)
        for (int i = 0; i < WAVESINFO[number]._sticks[PLATINUM]; i++)
        {
            a.push_back( PLATINUM );
        }
        Enemy1 enemy;

        switch(Randint(0, a.size()-1))
        {
        case AQEEQ:
            radius = Randint(500, 800);
            x_point = Randint( 0,  radius*2) -(radius-400);
            y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
            if (Randint(0, 1) == 0)
                y_point *= -1;
            y_point += 300;
            enemy.init1(x_point, y_point, 32, .7 ,2, &enemy1_grid, &ChargeEnemy,  glm::vec4(.7, .05, .05, 1.));
            enemy._allowed_distance_from_tower = Randint(enemy._min_distance_from_tower, enemy._max_distance_from_tower);
            AQEEQENEMIES.push_back(enemy);
            points -= AQEEQPTS;
            break;
        case FAKE:
            r = float(Randint(0, 100))/100.;
            g = float(Randint(0, 100))/100.;
            b = float(Randint(0, 100))/100.;
            radius = Randint(500, 800);
            x_point = Randint( 0,  radius*2) -(radius-400);
            y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
            if (Randint(0, 1) == 0)
                y_point *= -1;
            y_point += 300;
            enemy.init2(x_point, y_point, 24, 1 ,2, &enemy2_grid, &ChargeEnemy,  glm::vec4(r, g, b, 1.));
            FAKEENEMIES.push_back(enemy);
            points -= FAKEPTS;
            break;
        case TOPAZ:
            radius = Randint(500, 800);
            x_point = Randint( 0,  radius*2) -(radius-400);
            y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
            if (Randint(0, 1) == 0)
                y_point *= -1;
            y_point += 300;
            enemy.init3(x_point, y_point, 48, 1 ,5, &enemy3_grid, &ChargeEnemy,  glm::vec4(.8, .9, .05, 1.));
            TOPAZENEMIES.push_back(enemy);
            points -= TOPAZPTS;
            break;
        case PLATINUM:
            radius = Randint(500, 800);
            x_point = Randint( 0,  radius*2) -(radius-400);
            y_point = sqrtf( (radius*radius-((x_point-400)*(x_point-400)) ));
            if (Randint(0, 1) == 0)
                y_point *= -1;
            y_point += 300;
            enemy.init4(x_point, y_point, 26, 1 , 9, &enemy4_grid, &ChargeEnemy,  glm::vec4(1., 1., 1., 1.));
            enemy._allowed_distance_from_tower = 400;
            PLATINUMENEMIES.push_back(enemy);
            points -= PLATINUMPTS;
            break;
        default:;
        }
    }
    ENEMIESCOUNT = AQEEQENEMIES.size() + TOPAZENEMIES.size() + FAKEENEMIES.size() + PLATINUMENEMIES.size();
}

void LoadWaves()
{
    WAVESINFO.resize(50);

    //1
    int i = 0;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 4;
    WAVESINFO[i]._must_types[0] = AQEEQ;
    WAVESINFO[i]._must_counts[0] = 2;

    //2
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 6;
    WAVESINFO[i]._must_types[0] = AQEEQ;
    WAVESINFO[i]._must_counts[0] = 3;

    //3
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 8;
    WAVESINFO[i]._must_types[0] = AQEEQ;
    WAVESINFO[i]._must_counts[0] = 4;

    //4
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 10;
    WAVESINFO[i]._must_types[0] = FAKE;
    WAVESINFO[i]._must_counts[0] = 4;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //5
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 12;
    WAVESINFO[i]._must_types[0] = FAKE;
    WAVESINFO[i]._must_counts[0] = 6;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //6
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 13;
    WAVESINFO[i]._must_types[0] = AQEEQ;
    WAVESINFO[i]._must_counts[0] = 6;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //7
    i++;
    WAVESINFO[i].init(0);
    WAVESINFO[i]._points = 16;
    WAVESINFO[i]._sticks[FAKE] = 1;
    WAVESINFO[i]._sticks[AQEEQ] = 2;

    //8
    i++;
    WAVESINFO[i].init(0);
    WAVESINFO[i]._points = 18;
    WAVESINFO[i]._sticks[FAKE] = 1;
    WAVESINFO[i]._sticks[AQEEQ] = 3;

    //9
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 20;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //10
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 22;
    WAVESINFO[i]._must_types[0] = FAKE;
    WAVESINFO[i]._must_counts[0] = 10;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //11
    i++;
    WAVESINFO[i].init(0);
    WAVESINFO[i]._points = 24;
    WAVESINFO[i]._sticks[FAKE] = 2;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //12
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 26;
    WAVESINFO[i]._must_types[0] = FAKE;
    WAVESINFO[i]._must_counts[0] = 15;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //13
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 28;
    WAVESINFO[i]._must_types[0] = AQEEQ;
    WAVESINFO[i]._must_counts[0] = 10;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //14
    i++;
    WAVESINFO[i].init(0);
    WAVESINFO[i]._points = 30;
    WAVESINFO[i]._sticks[AQEEQ] = 3;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //15
    i++;
    WAVESINFO[i].init(2);
    WAVESINFO[i]._points = 32;
    WAVESINFO[i]._must_types[0] = TOPAZ;
    WAVESINFO[i]._must_counts[0] = 2;
    WAVESINFO[i]._must_types[1] = FAKE;
    WAVESINFO[i]._must_counts[1] = 5;
    WAVESINFO[i]._sticks[AQEEQ] = 2;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //16
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 34;
    WAVESINFO[i]._must_types[0] = TOPAZ;
    WAVESINFO[i]._must_counts[0] = 3;
    WAVESINFO[i]._sticks[AQEEQ] = 2;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //17
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 36;
    WAVESINFO[i]._must_types[0] = TOPAZ;
    WAVESINFO[i]._must_counts[0] = 3;
    WAVESINFO[i]._sticks[AQEEQ] = 3;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //18
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 38;
    WAVESINFO[i]._must_types[0] = AQEEQ;
    WAVESINFO[i]._must_counts[0] = 13;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[TOPAZ] = 3;

    //19
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 40;
    WAVESINFO[i]._must_types[0] = TOPAZ;
    WAVESINFO[i]._must_counts[0] = 6;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //20
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 42;
    WAVESINFO[i]._must_types[0] = PLATINUM;
    WAVESINFO[i]._must_counts[0] = 3;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[TOPAZ] = 1;

    //21
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 44;
    WAVESINFO[i]._must_types[0] = PLATINUM;
    WAVESINFO[i]._must_counts[0] = 5;
    WAVESINFO[i]._sticks[AQEEQ] = 3;
    WAVESINFO[i]._sticks[FAKE] = 1;
    WAVESINFO[i]._sticks[TOPAZ] = 3;

    //22
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 46;
    WAVESINFO[i]._must_types[0] = PLATINUM;
    WAVESINFO[i]._must_counts[0] = 5;
    WAVESINFO[i]._sticks[AQEEQ] = 4;
    WAVESINFO[i]._sticks[FAKE] = 1;
    WAVESINFO[i]._sticks[TOPAZ] = 4;
    WAVESINFO[i]._sticks[PLATINUM] = 4;

    //23
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 48;
    WAVESINFO[i]._must_types[0] = PLATINUM;
    WAVESINFO[i]._must_counts[0] = 5;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[TOPAZ] = 3;

    //24
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 50;
    WAVESINFO[i]._must_types[0] = TOPAZ;
    WAVESINFO[i]._must_counts[0] = 7;
    WAVESINFO[i]._sticks[PLATINUM] = 1;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //25
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 52;
    WAVESINFO[i]._must_types[0] = PLATINUM;
    WAVESINFO[i]._must_counts[0] = 7;
    WAVESINFO[i]._sticks[AQEEQ] = 1;

    //26
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 54;
    WAVESINFO[i]._must_types[0] = FAKE;
    WAVESINFO[i]._must_counts[0] = 30;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //27
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 56;
    WAVESINFO[i]._must_types[0] = AQEEQ;
    WAVESINFO[i]._must_counts[0] = 20;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[TOPAZ] = 1;

    //28
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 58;
    WAVESINFO[i]._must_types[0] = PLATINUM;
    WAVESINFO[i]._must_counts[0] = 7;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[FAKE] = 1;

    //29
    i++;
    WAVESINFO[i].init(1);
    WAVESINFO[i]._points = 60;
    WAVESINFO[i]._must_types[0] = TOPAZ;
    WAVESINFO[i]._must_counts[0] = 6;
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[PLATINUM] = 4;

    //30
    i++;
    WAVESINFO[i].init(0);
    WAVESINFO[i]._points = 70; /**/
    WAVESINFO[i]._sticks[AQEEQ] = 1;
    WAVESINFO[i]._sticks[FAKE] = 1;
    WAVESINFO[i]._sticks[PLATINUM] = 1;
    WAVESINFO[i]._sticks[TOPAZ] = 1;
}

Universal::Universal()
{
    _api.init(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
}

template <typename T>
std::string NumberToString ( T Number )
{
 std::ostringstream ss;
 ss << Number;
 return ss.str();
}

float CircleCollidesCircle( glm::vec2 center1, float radius1, glm::vec2 center2, float radius2)
{
    float distance = glm::distance( center1, center2 );
    if (distance < radius1 + radius2)
    {
        return fabs(distance-(radius1 + radius2));
    }
    return 0.;
}

int Randint( int from, int to)
{
    if (from == to)
    {
        return from;
    }
    if (from > to)
    {
        return (from + to)/Randint(2, 4);
    }
    to +=1;
    int number = rand()*12487;
    return number%(to-from)+from;
}

float Randfloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

bool PointInRect( glm::vec2 point, float max_x, float min_x, float max_y, float min_y)
{
    return point.x > min_x && point.x < max_x && point.y > min_y && point.y < max_y;
}

template <typename Enemy>
void PearlEnemyCollision( Pearl & pearl, Enemy & enemy)
{
    float correction = CircleCollidesCircle( glm::vec2(pearl._position[0], pearl._position[1]), pearl._radius,
                                             enemy._position, enemy._radius);
    if (correction == 0.)
    {
        return;
    }
    if (pearl._state != 2 || (enemy._cool_down_after_hit  > 0 && enemy._type == 3 ))
    {
        if (enemy._type == 4)
        {
            for (int i = 0; i < BROKENSHARDS.size(); i++)
            {
                if (BROKENSHARDS[i]._life <= 0)
                {
                    BROKENSHARDS[i].generate(2, 50, glm::vec2(0, 6.2830), .4, 0.3, enemy._color, glm::vec4(.2, .2, 0., .1), &SHARD, enemy._position);
                    break;
                }
            }
        }
        if (enemy._type == 3)
        {
            for (int i = 0; i < BROKENSHARDS.size(); i++)
            {
                if (BROKENSHARDS[i]._life <= 0)
                {
                    BROKENSHARDS[i].generate(2, 50, glm::vec2(0, 6.2830), .4, 0.3, enemy._color, glm::vec4(.2, .2, 0., .1), &SHARD, enemy._position);
                    break;
                }
            }
            pearl._state = 0;
            while (trackpoints.size() > 0)
                VectorPopFront(trackpoints);
        }

        float angle = atanf((pearl._position[1]-enemy._position.y)/(pearl._position[0]-enemy._position.x));

        if (pearl._position[0] > enemy._position.x)
        {
            pearl._position[1] += sinf(angle)*correction;
            pearl._position[0] += cosf(angle)*correction;
        }
        else
        {
            pearl._position[1] -= sinf(angle)*correction;
            pearl._position[0] -= cosf(angle)*correction;
        }
    }
    else if (enemy._cool_down_after_hit  <= 0)
    {
        enemy._cool_down_after_hit = 15;
        enemy._health -= pearl._charge_damage[pearl._current_charge_level-1];
        if (enemy._health <= 0)
        {
            shatter_sound.Play(0);
            for (int i = 0; i < BROKENSHARDS.size(); i++)
            {
                if (BROKENSHARDS[i]._life <= 0)
                {
                    BROKENSHARDS[i].generate(10, 50, glm::vec2(0, 6.2830), .4, 0.3, enemy._color, glm::vec4(.2, .2, 0., .1), &SHARD, enemy._position);
                    break;
                }
            }
            if (enemy._type == 1)
            {
                SCORE += 10 + 5*SHATTERCOUNT;
                SCORETEXT.resetText( "Score: " + NumberToString( SCORE ) );
                COMBOTEXT.resetText("+" + NumberToString( 10 + 5*SHATTERCOUNT ) + "" );
            }
            if (enemy._type == 2)
            {
                SCORETEXT.resetText( "Score: " + NumberToString( SCORE ) );
                COMBOTEXT.resetText("+" + NumberToString( 10 + 10*SHATTERCOUNT ) + "" );
                SCORE += 10 + 10 * SHATTERCOUNT;
            }

            if (enemy._type == 3)
            {
                SCORETEXT.resetText( "Score: " + NumberToString( SCORE ) );
                COMBOTEXT.resetText("+" + NumberToString( 30 + 5*SHATTERCOUNT ) + "" );
                SCORE += 30 + 5 * SHATTERCOUNT;
                pearl._state = 0;
                while (trackpoints.size() > 0)
                    VectorPopFront(trackpoints);
            }
            if (enemy._type == 4)
            {
                SCORETEXT.resetText( "Score: " + NumberToString( SCORE ) );
                COMBOTEXT.resetText("+" + NumberToString( 40 + 10*SHATTERCOUNT ) + "" );
                SCORE += 40 + 10*SHATTERCOUNT;
            }

            SHATTERCOUNT ++;
            COMBOTEXT._alpha = 1.;
            COMBOTEXTPOS = enemy._position;
            ENEMIESCOUNT--;


            switch (SHATTERCOUNT)
            {
            case 1:
                SCREENSHAKEDETAILS.setDetails( glm::vec2(14., 10.), glm::vec2(5., -5.), 1);
                break;
            case 2:
                SCREENSHAKEDETAILS.setDetails( glm::vec2(8., 10.), glm::vec2(10., -10.), 1.5);
                break;
            case 3:
                SCREENSHAKEDETAILS.setDetails( glm::vec2(7., 10.), glm::vec2(15., -15.), 2);
                break;
            case 4:
                SCREENSHAKEDETAILS.setDetails( glm::vec2(6., 10.), glm::vec2(20., -20.), 3);
                break;
            case 5:
                SCREENSHAKEDETAILS.setDetails( glm::vec2(6., 7.), glm::vec2(25., -25.), 4);
                break;
            case 6:
                SCREENSHAKEDETAILS.setDetails( glm::vec2(6., 7.), glm::vec2(25., -25.), 5);
                break;
            case 7:
                SCREENSHAKEDETAILS.setDetails( glm::vec2(5., 6.), glm::vec2(25., -25.), 6);
                break;
            default:
                if (SHATTERCOUNT > 7)
                    SCREENSHAKEDETAILS.setDetails( glm::vec2(6., 5.), glm::vec2(30., -30.), 12);
                break;

            }

            if (SCORE >= 1000.*RAEGMODECOUNTER)
            {
                RAEGMODECOUNTER++;
                RAGETIMER = SDL_GetTicks();
                pearl._rage_mode = true;
                BINGO._alpha = 1.;
                SCREENSHAKEDETAILS.setDetails( glm::vec2(10., 8.5), glm::vec2(50., -50.), 12);
                SCREENSHAKEDETAILS._interruptable = false;
            }

        }
    }
}

template <typename Enemy>
void EnemyTowerCollision( Enemy & enemy, Tower & tower )
{
    float correction = CircleCollidesCircle( glm::vec2(enemy._position.x, enemy._position.y), enemy._radius,
                                             glm::vec2(tower._position[0], tower._position[1]), tower._radius);
    if (correction == 0.)
    {
        return;
    }
    float angle = atanf((enemy._position.y-tower._position[1])/(enemy._position.x-tower._position[0]));


/*
    if (enemy._position.x > tower._position[0])
    {
        enemy._position.y += sinf(angle)*correction;
        enemy._position.x += cosf(angle)*correction;
    }
    else
    {
        enemy._position.y -= sinf(angle)*correction;
        enemy._position.x -= cosf(angle)*correction;
    }*/
    if (enemy._state != 2)
    {
        if (tower._position[0] > enemy._position.x)
        {
            tower._position[1] += sinf(angle)*correction;
            tower._position[0] += cosf(angle)*correction;
        }
        else
        {
            tower._position[1] -= sinf(angle)*correction;
            tower._position[0] -= cosf(angle)*correction;
        }
    }
    else
    {
        if (tower._position[0] > enemy._position.x)
        {
            tower._position[1] += sinf(angle)*correction*20;
            tower._position[0] += cosf(angle)*correction*20;
        }
        else
        {
            tower._position[1] -= sinf(angle)*correction*20;
            tower._position[0] -= cosf(angle)*correction*20;
        }
        switch (enemy._type)
        {
        case 1:
            shatter_sound.Play(0);
            for (int i = 0; i < BROKENSHARDS.size(); i++)
            {
                if (BROKENSHARDS[i]._life <= 0)
                {
                    BROKENSHARDS[i].generate(2, 20, glm::vec2(0, 6.2830), .5, 0.1, glm::vec4(0.1, 0.1, 0.8, .8), glm::vec4(.2, .2, 0., .1), &SHARD, glm::vec2(tower._position[0], tower._position[1]));
                    break;
                }
            }
            enemy._resultant_velocity = 1.5;
            enemy._state = 0;
            tower._health -= 2;
            HEALTHTEXT.resetText( "Health Points: " + NumberToString(tower._health));
            SCREENSHAKEDETAILS.setDetails( glm::vec2(10., 8.5), glm::vec2(10., -10.), 4);
            ENEMIESCOUNT--;
            break;
        case 2:
            shatter_sound.Play(0);
            for (int i = 0; i < BROKENSHARDS.size(); i++)
            {
                if (BROKENSHARDS[i]._life <= 0)
                {
                    BROKENSHARDS[i].generate(5, 50, glm::vec2(0, 6.2830), .3, 0.1, enemy._color, glm::vec4(.2, .2, 0., .1), &SHARD, enemy._position);
                    break;
                }
            }
            enemy._health = 0;
            tower._health -= 1;
            HEALTHTEXT.resetText( "Health Points: " + NumberToString(tower._health));
            SCREENSHAKEDETAILS.setDetails( glm::vec2(10., 8.5), glm::vec2(6., -6.), 4);
            break;
        case 4:
            shatter_sound.Play(0);
            for (int i = 0; i < BROKENSHARDS.size(); i++)
            {
                if (BROKENSHARDS[i]._life <= 0)
                {
                    BROKENSHARDS[i].generate(3, 20, glm::vec2(0, 6.2830), .5, 0.1, glm::vec4(0.1, 0.1, 0.8, .8), glm::vec4(.2, .2, 0., .1), &SHARD, glm::vec2(tower._position[0], tower._position[1]));
                    break;
                }
            }
            enemy._resultant_velocity = 1.7;
            enemy._state = 0;
            tower._health -= 4;
            HEALTHTEXT.resetText( "Health Points: " + NumberToString(tower._health));
            SCREENSHAKEDETAILS.setDetails( glm::vec2(10., 8.5), glm::vec2(6., -6.), 4);
        default:;
        }
        if (tower._health <= 0)
        {
            GAMEOVER = 0;
            tower._health = 0;
            SCREENSHAKEDETAILS.setDetails( glm::vec2(10., 8.5), glm::vec2(70., -70.), 10);
            SCREENSHAKEDETAILS._interruptable = false;
            for (int i = 0; i < BROKENSHARDS.size(); i++)
            {
                if (BROKENSHARDS[i]._life <= 0)
                {
                    BROKENSHARDS[i].generate(5, 70, glm::vec2(0, 6.2830), .3, 0.1, glm::vec4(0.1, 0.1, 1., 1.), glm::vec4(.2, .2, 0., .1), &SHARD, glm::vec2(tower._position[0], tower._position[1]));
                    break;
                }
            }
        }
    }
}

void PearlTowerCollision( Pearl & pearl, Tower & tower )
{
    float correction = CircleCollidesCircle( glm::vec2(pearl._position[0], pearl._position[1]), pearl._radius,
                                             glm::vec2(tower._position[0], tower._position[1]), tower._radius);
    if (correction == 0.)
    {
        return;
    }
    float angle = atanf((pearl._position[1]-tower._position[1])/(pearl._position[0]-tower._position[0]));



    if (pearl._position[0] > tower._position[0])
    {
        pearl._position[1] += sinf(angle)*correction;
        pearl._position[0] += cosf(angle)*correction;
    }
    else
    {
        pearl._position[1] -= sinf(angle)*correction;
        pearl._position[0] -= cosf(angle)*correction;
    }
}



void VectorPopFront( std::vector<glm::vec2> & container )
{
    std::vector<glm::vec2> new_vector;
    if (container.size() > 0)
    {
        new_vector.resize(container.size()-1);
        for (int i = 1; i < container.size(); i++)
        {
            new_vector[i-1] = container[i];
        }
    }
    container = new_vector;
}

void HowToPlayScreen( Universal & universal)
{
    TextureGrid howtoplay;
    howtoplay.init( "assets/howtoplay.png", 800, 600, 1, 1, 1024, 1024, 0, 0);

    TextureGrid cursor;
    cursor.init( "assets/cursor1.png", 32, 32, 1, 1, 32, 32, 0, 0 );

    TTF_Font * Hobo42 = TTF_OpenFont("assets/Hobo.ttf", 42);
    Text back_text;
    back_text.init("Back", Hobo42);
while (universal._running)
{
    universal._api.handleEvents();
    if (universal._api._is_trying_to_exit)
    {
        universal._running = false;
        return;
    }

    if (PointInRect(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]), 790, 712, 590, 552 ))
    {
        if (universal._api._is_left_mouse_button_clicked)
            return;
        back_text.setColor(1., 1., 1., 1.);
    }
    else
    {
        back_text.setColor(.6, .7, 1., 1.);
    }


    StartDrawingTextures();

    universal._api.SHADERS[0].setColor(1, 1, 1, 1);

    howtoplay.render(0, 0, 0, universal._api.SHADERS[0], 0 );
    back_text.renderText(710, 540, 0, universal._api.SHADERS[0]);
    cursor.render(CURSOR_POSITION[0], CURSOR_POSITION[1], 0, universal._api.SHADERS[0], 0);
    EndDrawingTextures();

    universal._api.flipWindow();
    universal._api.delayFPS();
    universal._api.clearWindow();
}
}

void TellStory( Universal & universal)
{
    glClearColor(.1, .1, .1, 1.);

    TextureGrid cursor;
    cursor.init( "assets/cursor1.png", 32, 32, 1, 1, 32, 32, 0, 0 );

    TTF_Font * Hobo84 = TTF_OpenFont("assets/Hobo.ttf", 28);

    Text back_text;
    back_text.init("Skip", Hobo84);
    Text D[10];
    D[0].init("Once Upon a time, in a far far away land..", Hobo84);
    D[1].init("There was a princess, a diamond princess, who loved a pearl knight..", Hobo84);
    D[2].init("Her father, the King was enraged by the idea of his", Hobo84);
    D[3].init("daughter shamely falling in love with a mere pearl being.", Hobo84);
    D[4].init("Upon hearing the news of his daughter going as far as marrying", Hobo84);
    D[5].init("that commoner, the king sent all his troops to end the couple's lives,", Hobo84);
    D[6].init("preferring to bring an honorable end to his blood line once and for all..", Hobo84);
    D[7].init("The courageous pearl knight decides to protect his beloved one,", Hobo84);
    D[8].init("regardless of the consequences!", Hobo84);

    for (int i = 0; i< 9; i++)
    {
        D[i]._text_texture._center[0] = D[i]._text_texture._clip_width/2.;
        D[i]._text_texture._center[1] = D[i]._text_texture._clip_height/2.;
    }
    int timer = SDL_GetTicks();
while (universal._running)
{
    universal._api.handleEvents();
    if (universal._api._is_trying_to_exit)
    {
        universal._running = false;
        return;
    }

    if (PointInRect(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]), 790, 712, 590, 552 ))
    {
        if (universal._api._is_left_mouse_button_clicked)
            return;
        back_text.setColor(1., 1., 1., 1.);
    }
    else
    {
        back_text.setColor(.6, .7, 1., 1.);
    }


    StartDrawingTextures();
    if (SDL_GetTicks() - timer < 5000 )
    {
        D[0].renderText(400, 100, 0, universal._api.SHADERS[0]);
        D[1].renderText(400, 200, 0, universal._api.SHADERS[0]);
    }
    else if (SDL_GetTicks() - timer < 10000 )
    {
        D[2].renderText(400, 100, 0, universal._api.SHADERS[0]);
        D[3].renderText(400, 200, 0, universal._api.SHADERS[0]);
    }
    else if (SDL_GetTicks() - timer < 16000 )
    {
        D[4].renderText(400, 100, 0, universal._api.SHADERS[0]);
        D[5].renderText(400, 200, 0, universal._api.SHADERS[0]);
    }
    else if (SDL_GetTicks() - timer < 21000 )
    {
        D[6].renderText(400, 100, 0, universal._api.SHADERS[0]);
    }
    else if (SDL_GetTicks() - timer < 27000 )
    {
        D[7].renderText(400, 100, 0, universal._api.SHADERS[0]);
        D[8].renderText(400, 200, 0, universal._api.SHADERS[0]);
    }
    else
        return;

    universal._api.SHADERS[0].setColor(1, 1, 1, 1);

    back_text.renderText(710, 540, 0, universal._api.SHADERS[0]);
    cursor.render(CURSOR_POSITION[0], CURSOR_POSITION[1], 0, universal._api.SHADERS[0], 0);
    EndDrawingTextures();

    universal._api.flipWindow();
    universal._api.delayFPS();
    universal._api.clearWindow();
}
}

int GameMenu( Universal & universal)
{
    glClearColor(.1, .1, .1, 1.);

    TextureGrid cursor;
    cursor.init( "assets/cursor1.png", 32, 32, 1, 1, 32, 32, 0, 0 );

    TTF_Font * Hobo84 = TTF_OpenFont("assets/Hobo.ttf", 84);
    TTF_Font * RAGE104 = TTF_OpenFont("assets/STENCIL.TTF", 68);

    Text Title;
    Title.init( "Protect A-Diamond!", RAGE104);

    Title.setColor(1, 1, 0., 1);

    Text start_game_text;
    start_game_text.init("Play!", Hobo84);
    start_game_text._text_texture._center[0] = start_game_text._text_texture._clip_width/2.;
    start_game_text._text_texture._center[1] = start_game_text._text_texture._clip_height/2.;
    start_game_text._text_texture.rotate( (-10./180.)*3.141, start_game_text._text_texture._center[0], start_game_text._text_texture._center[1]);
    start_game_text.setColor(.6, .7, 1., 1.);

    Text how_to_play_text;
    how_to_play_text.init("How To Play!", Hobo84);
    how_to_play_text._text_texture._center[0] = how_to_play_text._text_texture._clip_width/2.;
    how_to_play_text._text_texture._center[1] = how_to_play_text._text_texture._clip_height/2.;
    how_to_play_text._text_texture.scale( .7, .7, how_to_play_text._text_texture._center[0], how_to_play_text._text_texture._center[1]);
    how_to_play_text._text_texture.rotate( (-10./180.)*3.141, how_to_play_text._text_texture._center[0], how_to_play_text._text_texture._center[1]);
    how_to_play_text.setColor(.6, .7, 1., 1.);

    Text exit_game;
    exit_game.init("Exit..", Hobo84);
    exit_game._text_texture._center[0] = exit_game._text_texture._clip_width/2.;
    exit_game._text_texture._center[1] = exit_game._text_texture._clip_height/2.;
    exit_game._text_texture.scale( .5, .5, exit_game._text_texture._center[0], exit_game._text_texture._center[1]);
    exit_game._text_texture.rotate( (-10./180.)*3.141, exit_game._text_texture._center[0], exit_game._text_texture._center[1]);
    exit_game.setColor(.6, .7, 1., 1.);



while (universal._running)
{
    universal._api.handleEvents();
    if (universal._api._is_trying_to_exit)
    {
        universal._running = false;
        return 0;
    }

    if (PointInRect(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]), 480, 320, 300, 230 ))
    {
        if (universal._api._is_left_mouse_button_clicked)
            return 1;
        start_game_text.setColor(1., 1., 1., 1.);
    }
    else
    {
        start_game_text.setColor(.6, .7, 1., 1.);
    }

    if (PointInRect(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]), 545, 255, 407, 342 ))
    {
        if (universal._api._is_left_mouse_button_clicked)
            HowToPlayScreen( universal );
        how_to_play_text.setColor(1., 1., 1., 1.);
    }
    else
    {
        how_to_play_text.setColor(.6, .7, 1., 1.);
    }

    if (PointInRect(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]), 445, 355, 485, 440 ))
    {
        if (universal._api._is_left_mouse_button_clicked)
            universal._running = false;
        exit_game.setColor(1., 1., 1., 1.);
    }
    else
    {
        exit_game.setColor(.6, .7, 1., 1.);
    }

    StartDrawingTextures();

    universal._api.SHADERS[0].setColor(1, 1, 1, 1);

    Title.renderText( 60, 100, 0,  universal._api.SHADERS[0]);

    start_game_text.renderText(400, 260, 0, universal._api.SHADERS[0]);
    how_to_play_text.renderText(400, 370, 0, universal._api.SHADERS[0]);
    exit_game.renderText(400, 460, 0, universal._api.SHADERS[0]);
    cursor.render(CURSOR_POSITION[0], CURSOR_POSITION[1], 0, universal._api.SHADERS[0], 0);
    EndDrawingTextures();

    universal._api.flipWindow();
    universal._api.delayFPS();
    universal._api.clearWindow();
}
}

void Universal::mainLoop()
{
    TextureGrid BG;
    BG.init("assets/bg.png", 1024, 1024, 1, 1, 1024, 1024, 100, 100);
    SHARD.init("assets/shardy.png", 64, 64, 1, 1, 64, 64, 32, 32);


    BROKENSHARDS.resize(50);
    //emittertest.generate(5, 70, glm::vec2(0, 6.2830), .7, 0.3, glm::vec4(.7, .7, 0., .9), glm::vec4(.2, .2, 0., .1), &shard, glm::vec2(400, 300));


    TTF_Font * Hobo42 = TTF_OpenFont("assets/Hobo.ttf", 42);
    TTF_Font * Rage64 = TTF_OpenFont("assets/STENCIL.TTF", 104);
    Text GameOver, YouWin;
    GameOver.init( "GAME OVER", Rage64);
    YouWin.init("You Win!", Rage64);

    GREAT.init( "GREAT!", Hobo42 );
    GREAT.setColor( 0., .5, 1, 1 );
    FANTASTIC.init( "FANTASTIC!", Hobo42);
    FANTASTIC.setColor( 1., .9, 0, 1 );
    INCREDIBLE.init( "INCREDIBLE!", Hobo42);
    FANTASTIC.setColor( 1., .0, .9, 1 );
    BINGO.init("RAEG MODE!", Rage64);
    BINGO.setColor(1., .2, .2, 1.);
    BINGO._text_texture._center[0] = BINGO._text_texture._clip_width/2.;
    BINGO._text_texture._center[1] = BINGO._text_texture._clip_height/2.;
    BINGO._text_texture.rotate( (-15./180.)*3.141, BINGO._text_texture._clip_width/2., BINGO._text_texture._clip_height/2. );
    glm::vec4 bingo_colors[6];
    bingo_colors[0] = glm::vec4(1., 0., 0., 1.);
    bingo_colors[1] = glm::vec4(0., 0., 1., 1.);
    bingo_colors[2] = glm::vec4(1., 1., 0., 1.);
    bingo_colors[3] = glm::vec4(1., 0., 1., 1.);
    bingo_colors[4] = glm::vec4(0., 1., 0.5, 1.);
    bingo_colors[5] = glm::vec4(1., .9, 0.4, 1.);



    srand( time(nullptr));
    SDL_ShowCursor( 0 );
    TTF_Font * Hobo32 = TTF_OpenFont("assets/Hobo.ttf", 32);

    SCORETEXT.init("Score: 0", Hobo32);
    WAVETEXT.init("Wave: 1", Hobo32);
    COMBOTEXT.init (" ", Hobo32);

    HEALTHTEXT.init("Health Points: 100", Hobo32);
    HEALTHTEXT.setColor(1.0, 0.6, 0.6, 1.0);




    float trackpoints_timer = SDL_GetTicks();
    //trackpoints.push_back(glm::vec2(400, 300));
    ChargeEnemy.init( "assets/pearldasheffect.png", 198, 188, 5, 3, 1024, 1024, 96, 94 );
    ChargeEnemy.scale(0.5, 0.5, 96, 94);
    Music BGmusic;
    BGmusic.LoadMusicFile( "assets\\bgmsc.ogg");
    BGmusic.Play();

    //Sound shatter_sound;
    shatter_sound.LoadWavFile( "assets/enemyshattering1.wav" );


    GameMenu(*this);

    Tower tower;

    tower._grid.init( "assets/tower.png", 256, 256, 1, 1, 256, 256, 128, 128);
    tower._grid.scale( 0.5, 0.5, 128, 128 );
    tower.init( 400, 300, 122, 100, 0.5);

    std::vector<Enemy1> enemies;

    enemy1_grid.init( "assets/enemy1.png", 64, 64, 1, 1, 64, 64, 32, 32);
    enemy2_grid.init( "assets/enemy2.png", 64, 64, 1, 1, 64, 64, 32, 32);
    enemy3_grid.init( "assets/enemy3.png", 128, 128, 1, 1, 128, 128, 64, 64);
    enemy4_grid.init( "assets/enemy4.png", 64, 64, 1, 1, 64, 64, 32, 32);

    /*enemies.resize(ENEMIESCOUNT);
    for (int i = 0; i < enemies.size(); i++)
    {
        float x_point = Randint( 0,  1000) -100;
        float y_point = sqrtf( (500*500)-((x_point-400)*(x_point-400)) );
        if (Randint(0, 1) == 0)
            y_point *= -1;
        y_point += 300;
        float r = float(Randint(0, 100))/100.;
        float g = float(Randint(0, 100))/100.;
        float b = float(Randint(0, 100))/100.;
        enemies[i].init1(x_point, y_point, 32, .7 ,10, &enemy1_grid, &ChargeEnemy,  glm::vec4(r, g, b, 1.));
        enemies[i]._type = 1;
        enemies[i]._allowed_distance_from_tower = Randint(enemies[i]._min_distance_from_tower, enemies[i]._max_distance_from_tower);
    }*/

    TextureGrid cursor;
    cursor.init( "assets/cursor1.png", 32, 32, 1, 1, 32, 32, 0, 0 );

    TextureGrid target;
    target.init( "assets/target_cross.png", 64, 64, 1, 1, 64, 64, 32, 32);

    Pearl HeroPearl;
    HeroPearl.init( 400, 300, 0.3, 43 );

    int after_effect_time = 1;
    int after_effect_counter = 0;
    std::vector<glm::vec2> after_positions;


    LoadWaves();
    GenerateWave(CURRENT_WAVE-1);


    //glClearColor(.3, .7, .3, 1. );

    glUseProgram(_api.SHADERS[0]._shader_program_id);

    int enemies_count = 1000;
    TellStory( *this );
while (_running)
{



    if ((SDL_GetTicks()-RAGETIMER)/1000. >= RAGE_TIME)
    {
        HeroPearl._rage_mode = false;
    }
    if (CURRENT_WAVE == 31)
    {
        GAMEOVER = 2;
        SCREENSHAKEDETAILS.setDetails( glm::vec2(10., 8.5), glm::vec2(70., -70.), 10);
        SCREENSHAKEDETAILS._interruptable = false;
    }
    ShakeScreen( _api.SHADERS[0]);

    if (enemies_count == 0 && GAMEOVER == 1)
    {
        WAVETEXT.resetText("Wave: " + NumberToString(CURRENT_WAVE+1));
        GenerateWave(CURRENT_WAVE++);

    }


    enemies_count = 0;
    for (int i = 0; i < AQEEQENEMIES.size(); i++)
    {
        if (AQEEQENEMIES[i]._health <= 0.)
        {
            continue;
        }
        enemies_count ++;
        AQEEQENEMIES[i].move(tower._position[0], tower._position[1]);

    }
    for (int i = 0; i < FAKEENEMIES.size(); i++)
    {
        if (FAKEENEMIES[i]._health <= 0.)
        {
            continue;
        }
        enemies_count ++;
        FAKEENEMIES[i].move(tower._position[0], tower._position[1]);

    }
    for (int i = 0; i < TOPAZENEMIES.size(); i++)
    {
        if (TOPAZENEMIES[i]._health <= 0.)
        {
            continue;
        }
        enemies_count ++;
        TOPAZENEMIES[i].move(HeroPearl._position[0], HeroPearl._position[1]);
    }
    for (int i = 0; i < PLATINUMENEMIES.size(); i++)
    {
        if (PLATINUMENEMIES[i]._health <= 0.)
        {
            continue;
        }
        enemies_count ++;
        PLATINUMENEMIES[i].move(tower._position[0], tower._position[1]);
    }

    if (HeroPearl._state == 1 || HeroPearl._state == 2)
    {
        HeroPearl.move( trackpoints[0].x, trackpoints[0].y );
        if (HeroPearl._state == 2)
        {
            if ((HeroPearl._position[0] == trackpoints[0].x && HeroPearl._position[1] == trackpoints[0].y) ||
                 (CircleCollidesCircle(trackpoints[0], 0, glm::vec2(tower._position[0], tower._position[1]), tower._radius) != 0.))
            {
                VectorPopFront(trackpoints);
            }
            if (trackpoints.size() == 0)
            {
                HeroPearl._state = 0;
            }
        }

        if (HeroPearl._state == 1)
        {
            while (after_positions.size() != 0)
            {
                after_positions.pop_back();
            }
            if (glm::distance(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]), trackpoints[trackpoints.size()-1]) > 80. && trackpoints.size() <= 10)
            {
                trackpoints.push_back(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]));
            }
        }
    }
    else
    {
        HeroPearl.move( CURSOR_POSITION[0], CURSOR_POSITION[1] );
    }

    tower.move(HeroPearl._position[0], HeroPearl._position[1]);

    PearlTowerCollision( HeroPearl, tower );
    for (int i = 0; i < AQEEQENEMIES.size(); i++)
    {
        if (AQEEQENEMIES[i]._health <= 0.) continue;
        PearlEnemyCollision( HeroPearl, AQEEQENEMIES[i]);
        EnemyTowerCollision( AQEEQENEMIES[i], tower);
    }
    for (int i = 0; i < FAKEENEMIES.size(); i++)
    {
        if (FAKEENEMIES[i]._health <= 0.) continue;
        PearlEnemyCollision( HeroPearl, FAKEENEMIES[i]);
        EnemyTowerCollision( FAKEENEMIES[i], tower);
    }
    for (int i = 0; i < TOPAZENEMIES.size(); i++)
    {
        if (TOPAZENEMIES[i]._health <= 0.) continue;
        PearlEnemyCollision( HeroPearl, TOPAZENEMIES[i]);
        EnemyTowerCollision( TOPAZENEMIES[i], tower);
    }
    for (int i = 0; i < PLATINUMENEMIES.size(); i++)
    {
        if (PLATINUMENEMIES[i]._health <= 0.) continue;
        PearlEnemyCollision( HeroPearl, PLATINUMENEMIES[i]);
        EnemyTowerCollision( PLATINUMENEMIES[i], tower);
    }
    _api.handleEvents();


    if (HeroPearl._state == 2 && after_effect_counter++ == after_effect_time)
    {
        after_effect_counter = 0;
        after_positions.push_back(glm::vec2(HeroPearl._position[0], HeroPearl._position[1]));
        if (after_positions.size() > 7)
        {
            VectorPopFront(after_positions);
        }
    }

    if (_api._left_mouse_button_down)
    {
        if (HeroPearl._state != 2)
        {
            SHATTERCOUNT = 0;
            HeroPearl._state = 1;
            if (trackpoints.size() == 0)
                trackpoints.push_back(glm::vec2(CURSOR_POSITION[0], CURSOR_POSITION[1]));
        }
    }
    else
    {
        if (HeroPearl._state == 1 && HeroPearl._current_charge_level > 0)
            HeroPearl._state = 2;
    }


    if (_api._is_trying_to_exit)
    {
        _running = false;
    }

    HeroPearl.updateFrame();

    StartDrawingTextures();
    BG.render(0, 0, 0, _api.SHADERS[0], 0);
    if (GAMEOVER == 1)
    {

    _api.SHADERS[0].setColor(1, 1, 1, 1);

    tower.render( _api.SHADERS[0] );

    for (int i = 1; i < 11; i++)
    {
        if (trackpoints.size() >= i+1)
        {
            target.render( trackpoints[i].x, trackpoints[i].y, 0, _api.SHADERS[0], 0);
        }
    }

    for (int i = 0; i < AQEEQENEMIES.size(); i++)
    {
        if (AQEEQENEMIES[i]._health <= 0.) continue;
        AQEEQENEMIES[i].render(_api.SHADERS[0]);
        AQEEQENEMIES[i].update();
    }
    for (int i = 0; i < FAKEENEMIES.size(); i++)
    {
        if (FAKEENEMIES[i]._health <= 0.) continue;
        FAKEENEMIES[i].render(_api.SHADERS[0]);
        FAKEENEMIES[i].update();
    }
    for (int i = 0; i < TOPAZENEMIES.size(); i++)
    {
        if (TOPAZENEMIES[i]._health <= 0.) continue;
        TOPAZENEMIES[i].render(_api.SHADERS[0]);
        TOPAZENEMIES[i].update();
    }
    for (int i = 0; i < PLATINUMENEMIES.size(); i++)
    {
        if (PLATINUMENEMIES[i]._health <= 0.) continue;
        PLATINUMENEMIES[i].render(_api.SHADERS[0]);
        PLATINUMENEMIES[i].update();
    }

    HeroPearl.render(_api.SHADERS[0]);
    if (HeroPearl._state == 2 && after_positions.size() > 0)
    {
        for (int i = 0; i < after_positions.size(); i++)
        {
            HeroPearl._animations[9-i].render(after_positions[i].x, after_positions[i].y, _api.SHADERS[0]);
        }
    }

    if (COMBOTEXT._alpha > 0.)
    {
        COMBOTEXT._alpha -= .02;
        if (COMBOTEXT._alpha < 0.)
        {
            COMBOTEXT._alpha = 0.;
        }
    }
    COMBOTEXT.renderText( COMBOTEXTPOS.x, COMBOTEXTPOS.y, 0, _api.SHADERS[0]);

    SCORETEXT.renderText( 10, WINDOW_HEIGHT-36, 0, _api.SHADERS[0]);
    WAVETEXT.renderText( 650, WINDOW_HEIGHT-36, 0, _api.SHADERS[0]);
    HEALTHTEXT.renderText(300, WINDOW_HEIGHT-36, 0, _api.SHADERS[0]);

    if (HeroPearl._rage_mode)
    {
        if (BINGO._alpha > 0.)
        {
            BINGO._alpha -= .01;
            float bingo_size = ((SDL_GetTicks()-RAGETIMER)/1000.)*1.3;
            BINGO._text_texture.scale(bingo_size, bingo_size, BINGO._text_texture._clip_width/2., BINGO._text_texture._clip_height/2.);
            if (BINGO._alpha < 0.)
            {
                BINGO._alpha = 0.;
            }
        }
        BINGO.renderText( WINDOW_WIDTH/2., WINDOW_HEIGHT/2., 0, _api.SHADERS[0]);
    }
    }
    for (int i = 0; i< BROKENSHARDS.size(); i++)
    {
        BROKENSHARDS[i].updateShards();
        BROKENSHARDS[i].renderShards(_api.SHADERS[0]);
    }
    //emittertest.updateShards();
    //emittertest.renderShards( _api.SHADERS[0]);
    _api.SHADERS[0].setColor(1, 1, 1, .5);
    cursor.render( CURSOR_POSITION[0], CURSOR_POSITION[1], 0, _api.SHADERS[0], 0 );

    if (GAMEOVER == 0)
    {
        GameOver.renderText(100, 300, 0, _api.SHADERS[0]);
    }
    if (GAMEOVER == 2)
    {
        YouWin.renderText(100, 300, 0, _api.SHADERS[0]);
    }


    EndDrawingTextures();

    _api.flipWindow();
    _api.delayFPS();
    _api.clearWindow();
}

}
