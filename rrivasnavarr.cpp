#include "rrivasnavarr.h"
#include "sslone.h"
using namespace std;

extern Global gl;

/*Wall::Wall()
{
    extern Global gl;
    w = 5.0f;
    l = 4000.0f;//gl.xres;
    x = 0.0f;
    y = 0.0f;
}
*/
Wall::~Wall(){}

Wall::Wall(PhysWorld * in_member = NULL, float height = 100.0f, float width = 100.0f) : Object(in_member)
{
    objectType = WALL;

    pos[0] = gl.xres / 2;
    pos[1] = gl.yres / 2;
    h = height;
    w = width;
    //pos[1] = h / 2; 
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

Map::~Map(){}

void Map::render()
{
    uWall->render();
    dWall->render();
    lWall->render();
    rWall->render();
    mWall->render();
}

