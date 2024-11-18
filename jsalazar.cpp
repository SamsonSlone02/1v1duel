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
}
void Bomb::physics()
{
    if (isBullet) {
        cout << "bomb x pos: " << mybullet->pos[0] << endl;
        cout << "bomb y pos: " << mybullet->pos[1] << endl;
    }
    

}




void Bomb::render()
{
    if(isBullet) {
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
    }

}

