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
    //pos[1] = gl.yres / 2;
    h = height;
    w = width;
    pos[1] = h / 2; 
}

void Wall::render()
{
    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(pos[0] - w/2, pos[1] + h/2);
    glVertex2f(pos[0] - w/2, pos[1] - h/2);
    glVertex2f(pos[0] + w/2, pos[1] - h/2);
    glVertex2f(pos[0] + w/2, pos[1] + h/2);
    glEnd();

}
