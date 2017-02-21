//==========Wave.h==========
#ifndef Wave_H_INCLUDED
#define Wave_H_INCLUDED

//=================================
// included dependencies
#include "Enemy.h"
//=================================

typedef enum
{
	AQEEQ = 0,
	TOPAZ = 1,
	FAKE = 2,
	PLATINUM = 3,
} ENEMYTYPES;

typedef enum
{
    FAKEPTS = 1,
    AQEEQPTS = 2,
    TOPAZPTS = 4,
    PLATINUMPTS = 6,
} ENEMYPOINTS;


class Wave
{
public:
    void init(int must_types_count);
    int _points; //to "buy" enemies
    std::vector<ENEMYTYPES> _must_types;
    std::vector<int> _must_counts;
    int _sticks[4] = {0, 0, 0, 0}; //sticks for the "draw" for whom is to be in the wave
};


#endif  // Wave_H_INCLUDED
