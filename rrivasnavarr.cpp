#include "rrivasnavarr.h"
#include "sslone.h"
using namespace std;

extern Global gl;

// Wall Object handling
Wall::~Wall(){}

Wall::Wall(PhysWorld * in_member = NULL, float height = 100.0f, 
        float width = 100.0f) : Object(in_member)
{
    objectType = WALL;

    pos[0] = gl.xres/2;
    pos[1] = gl.yres/2;
    h = height;
    w = width;
}

// renders Wall appropriately
void Wall::render()
{
    glColor3ub(0, 0, 128);
    glBegin(GL_POLYGON);
    glVertex2f(pos[0] - w/2, pos[1] + h/2);
    glVertex2f(pos[0] - w/2, pos[1] - h/2);
    glVertex2f(pos[0] + w/2, pos[1] - h/2);
    glVertex2f(pos[0] + w/2, pos[1] + h/2);
    glEnd();

}

// make array of Walls for use in Map
Wall *wa[14];

// Map class for designing Map/Arena
Map::Map()
{
    extern PhysWorld * myPhysWorld;

    wa[0] = new Wall(myPhysWorld, 30.0f, gl.xres);
    wa[1] = new Wall(myPhysWorld, 30.0f, gl.xres);
    wa[1]->pos[1] = gl.yres - 5.0f;
    wa[2] = new Wall(myPhysWorld, gl.yres, 30.0f);
    wa[2]->pos[0] = 5.0f;
    wa[2]->pos[1] = gl.yres / 2;
    wa[3] = new Wall(myPhysWorld, gl.yres, 30.0f);
    wa[3]->pos[0] = gl.xres - 5.0f;
    wa[3]->pos[1] = gl.yres / 2;
    wa[0]->pos[0] = gl.xres / 2;
    wa[0]->pos[1] = 5.0f;
    wa[4] = new Wall(myPhysWorld, gl.yres/27, gl.xres/7);
    wa[4]->pos[0] = 3*gl.xres/14;
    wa[4]->pos[1] = 3*gl.yres/5;
    wa[5] = new Wall(myPhysWorld, gl.yres/4.2, gl.xres/48);
    wa[5]->pos[0] = 2*gl.xres/7;
    wa[5]->pos[1] = 7*gl.yres/10;
    wa[6] = new Wall(myPhysWorld, gl.yres/4.2, gl.xres/48);
    wa[6]->pos[0] = 5*gl.xres/7;
    wa[6]->pos[1] = 7*gl.yres/10;
    wa[7] = new Wall(myPhysWorld, gl.yres/27, gl.xres/7);
    wa[7]->pos[0] = 11*gl.xres/14;
    wa[7]->pos[1] = 3*gl.yres/5;
    wa[8] = new Wall(myPhysWorld, gl.yres/27, gl.xres/7);
    wa[8]->pos[0] = 3*gl.xres/14;
    wa[8]->pos[1] = 2*gl.yres/5;
    wa[9] = new Wall(myPhysWorld, gl.yres/4.2, gl.xres/48);
    wa[9]->pos[0] = 2*gl.xres/7;
    wa[9]->pos[1] = 3*gl.yres/10;
    wa[10] = new Wall(myPhysWorld, gl.yres/4.2, gl.xres/48);
    wa[10]->pos[0] = 5*gl.xres/7;
    wa[10]->pos[1] = 3*gl.yres/10;
    wa[11] = new Wall(myPhysWorld, gl.yres/27, gl.xres/7);
    wa[11]->pos[0] = 11*gl.xres/14;
    wa[11]->pos[1] = 2*gl.yres/5;
    wa[12] = new Wall(myPhysWorld, gl.yres/27, gl.xres/7);
    wa[12]->pos[0] = gl.xres/2;
    wa[12]->pos[1] = 3*gl.yres/5;
    wa[13] = new Wall(myPhysWorld, gl.yres/27, gl.xres/7);
    wa[13]->pos[0] = gl.xres/2;
    wa[13]->pos[1] = 2*gl.yres/5;

    // add Walls as Objects to Physics World for collision handling
    for (int i = 0; i < 14; i++)
        myPhysWorld->addObject(wa[i]);
}

// Deconstructor to remove Walls on Map Switch
Map::~Map()
{
    extern PhysWorld * myPhysWorld;

    for (int i = 0; i < 14; i++)
        myPhysWorld->remObject(wa[i]);
}

// render Walls in Map class
void Map::render()
{
    for (int i = 0; i < 14; i++)
        wa[i]->render();
}

// Menu functions for Main Menu to display images
Menu::~Menu(){}

Menu::Menu(){}

void Menu::render()
{
    // draws background
    glColor3f(255/255.0, 255/255.0, 255/255.0);
    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(gl.xres, 0);
    glVertex2i(gl.xres, gl.yres);
    glVertex2i(0, gl.yres);
    glEnd();

    // displays menu png
    glBindTexture(GL_TEXTURE_2D, gl.menu);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, gl.yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(gl.xres, gl.yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(gl.xres, 0);
    glEnd();

    // displays button only when necessary
    if (gl.hoverButton) {
        glBindTexture(GL_TEXTURE_2D, gl.button1);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(gl.xres/2 - gl.xres/6.3, gl.yres/2 - gl.yres/5);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(gl.xres/2 - gl.xres/6.3, gl.yres/2 - gl.yres/10);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(gl.xres/2 + gl.xres/6.3, gl.yres/2 - gl.yres/10);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(gl.xres/2 + gl.xres/6.3, gl.yres/2 - gl.yres/5);
        glEnd();
    }
    glPopMatrix();
}

