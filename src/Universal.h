//==========universal.h==========
#ifndef UNI_H_INCLUDED
#define UNI_H_INCLUDED

//=================================
// included dependencies
#include <time.h>
#include <sstream>
#include "Api.h"
#include "Globals.h"
//=================================

class Universal
{
public:
    bool _running = true;
    API _api;
    Universal();
    void mainLoop();
};

#endif  // api_H_INCLUDED
