#include <iostream>
#include "sslone.h"
#include "jsalazar.h"
using namespace std;
/*
   string Bomb::getWeapon()
   {
   return this->type;
   }
   */ 


Bomb::Bomb(int in_rate = 10, Player * in_parent = NULL , PhysWorld * in_member = NULL) : Weapon(in_rate, in_parent)
{
    Weapon(in_rate, in_parent);
    member = in_member;
    this->parent = in_parent;
    nbullets = 0;
    isBullet = false;
    boom = false;
}

Bomb::~Bomb()
{  

}

void Bomb::fireWeapon()
{
    mybullet = new Bullet(member);
    mybullet->pos[0] = parent->ship->pos[0];
    mybullet->pos[1] = parent->ship->pos[1];
    mybullet->color[0] = 0;
    mybullet->color[1] = 0;
    mybullet->color[2] = 0;
    cout << "Firing Bomb" << endl;
    isBullet = true;
    boom = false;
    xBounce = 1;
    yBounce = 1;
    Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
    //convert angle to a vector
    Flt xdir = cos(rad);
    Flt ydir = sin(rad);
    mybullet->vel[0] = xdir*4;
    mybullet->vel[1] = ydir*4;

}
void Bomb::physics()
{
    Global gl;
    if (isBullet) { 
        //  mybullet->vel[0] = 2; 
        // mybullet->vel[1] = 2;
        if (!boom) { 
            mybullet->pos[0] += mybullet->vel[0];
            mybullet->pos[1] += mybullet->vel[1];
        }

        if (mybullet->pos[0] < 0.0) {
            mybullet->xBounce *= -1;
            mybullet->pos[0] = 5;
            boom = true;
        }
        if (mybullet->pos[0] > (float)gl.xres) {
            mybullet->xBounce *= -1;
            mybullet->pos[0] =(float)gl.xres - 5;
            boom = true;
        }

        if (mybullet->pos[1] < 0.0) {
            mybullet->yBounce *= -1;
            mybullet->pos[1] = 5;
            boom = true;
        }
        if (mybullet->pos[1] > (float)gl.yres) {
            mybullet->yBounce *= -1;
            mybullet->pos[1] =(float)gl.yres - 5;
            boom = true;
        }


    }


}




void Bomb::render()
{
    if(isBullet) {
        if (!boom) {
            glPushMatrix();
            glColor3f(mybullet->color[0], mybullet->color[1], mybullet->color[2]);
            glTranslatef(mybullet->pos[0], mybullet->pos[1], 1);
            glBegin(GL_POLYGON);
            glVertex2f(0.0f, 7.0f);
            glVertex2f(  0.0f,  0.0f);
            glVertex2f(  7.0f,  0.0f);
            glVertex2f(7.0f, 7.0f);
            glEnd();
            glPopMatrix();

        } else {

            glPushMatrix();
            glColor3f(mybullet->color[0], mybullet->color[1], mybullet->color[2]);
            glTranslatef(mybullet->pos[0], mybullet->pos[1], 1);
            glBegin(GL_POLYGON);
            glVertex2f(0.0f, 30.0f);
            glVertex2f(  0.0f,  0.0f);
            glVertex2f(  30.0f,  0.0f);
            glVertex2f(30.0f, 30.0f);
            glEnd();
            glPopMatrix();

        }

    }
}

