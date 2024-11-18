#ifndef RRIVASNAVARR_H
#define RRIVASNAVARR_H

#include <ctime>
#include<iostream>
#include<cstring>
#include <GL/glx.h>
#include<cmath>
#include "fonts.h"

#include "sslone.h"

class Wall;

class Wall:public Object
{
    public:
        Wall(PhysWorld * in_member, float width, float height);
        ~Wall();
        void render();
};

class Map
{
    public:
        Map();
        ~Map();
        void render();
};
#endif
