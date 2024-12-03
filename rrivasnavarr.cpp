#include "rrivasnavarr.h"
#include "sslone.h"
using namespace std;

extern Global gl;

Wall::~Wall(){}

Wall::Wall(PhysWorld * in_member = NULL, float height = 100.0f, float width = 100.0f) : Object(in_member)
{
    objectType = WALL;

    pos[0] = gl.xres / 2;
    pos[1] = gl.yres / 2;
    h = height;
    w = width;
}

void Wall::render()
{
    glColor3ub(0, 255, 0);
    glBegin(GL_POLYGON);
    glVertex2f(pos[0] - w/2, pos[1] + h/2);
    glVertex2f(pos[0] - w/2, pos[1] - h/2);
    glVertex2f(pos[0] + w/2, pos[1] - h/2);
    glVertex2f(pos[0] + w/2, pos[1] + h/2);
    glEnd();

}

Wall *uWall, *dWall, *lWall, *rWall, *mWall;

Map::Map()
{
    extern PhysWorld * myPhysWorld;

    dWall = new Wall(myPhysWorld, 10.0f, gl.xres);
    uWall = new Wall(myPhysWorld, 10.0f, gl.xres);
    uWall->pos[1] = gl.yres - 5.0f;
    lWall = new Wall(myPhysWorld, gl.yres, 10.0f);
    lWall->pos[0] = 5.0f;
    lWall->pos[1] = gl.yres / 2;
    rWall = new Wall(myPhysWorld, gl.yres, 10.0f);
    rWall->pos[0] = gl.xres - 5.0f;
    rWall->pos[1] = gl.yres / 2;
    mWall = new Wall(myPhysWorld, gl.yres / 2, 10.0f);
    mWall->pos[0] = gl.xres / 2;
    mWall->pos[1] = gl.yres / 4;
    dWall->pos[0] = gl.xres / 2;
    dWall->pos[1] = 5.0f;
    myPhysWorld->addObject(dWall); 
    myPhysWorld->addObject(uWall); 
    myPhysWorld->addObject(lWall); 
    myPhysWorld->addObject(rWall); 
    myPhysWorld->addObject(mWall); 
}

Map::~Map()
{
    extern PhysWorld * myPhysWorld;
    myPhysWorld->remObject(dWall);
    myPhysWorld->remObject(uWall);
    myPhysWorld->remObject(lWall);
    myPhysWorld->remObject(rWall);
    myPhysWorld->remObject(mWall);
}

void Map::render()
{
    uWall->render();
    dWall->render();
    lWall->render();
    rWall->render();
    mWall->render();
}

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

    glBindTexture(GL_TEXTURE_2D, gl.menu);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, gl.yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(gl.xres, gl.yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(gl.xres, 0);
    glEnd();

    if (gl.hoverButton) {
        glBindTexture(GL_TEXTURE_2D, gl.button1);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(gl.xres/2 - gl.xres/6.3, gl.yres/2 - gl.yres/5);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(gl.xres/2 - gl.xres/6.3, gl.yres/2 - gl.yres/10);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(gl.xres/2 + gl.xres/6.3, gl.yres/2 - gl.yres/10);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(gl.xres/2 + gl.xres/6.3, gl.yres/2 - gl.yres/5);
        glEnd();
    }
    glPopMatrix();
}

