#ifndef RRIVASNAVARR_H
#define RRIVASNAVARR_H

#include <ctime>
#include<iostream>
#include<cstring>
#include <GL/glx.h>
#include<cmath>
#include "fonts.h"

#include "sslone.h"

//class Global;
class Wall;
//class Object;
//class PhysWorld;

/*class Global 
{
    public:
        int xres, yres;
        char keys[65536];
        Global();
};*/

/*class Wall 
{
    public:
        int l, w, x, y;
        Wall();
        ~Wall();
        Wall(int);
        virtual void render();
};*/

class Wall:public Object
{
    public:
        Wall(PhysWorld * in_member);
        ~Wall();
        void render();
};
#endif
