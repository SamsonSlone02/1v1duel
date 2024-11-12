#include "rrivasnavarr.h"
#include "sslone.h"
using namespace std;

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

Wall::Wall(PhysWorld * in_member = NULL) : Object(in_member)
{
    objectType = WALL;
    
   // xy coordinates of position
    // w width (default value)
    // l depends on xy end coordinates
    //      so x2 y2? or direction boolean?
    /*w = 5.0f;
    l = in;  
    x = 0.0f;
    y = 0.0f;*/  
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
