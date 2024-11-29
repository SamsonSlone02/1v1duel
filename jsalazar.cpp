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


    for (int i = 0; i < 8; i++) {
        barr[i] = NULL;
    }

}

Bomb::~Bomb()
{  
    for (int i = 0; i < 8; i++) {
        cout << "removing bullet" << endl;
        myBullet = barr[i];
        parent->ship->remFilter(myBullet);
        member->remObject(myBullet);
    }

}

void Bomb::fireWeapon()
{

    if (myBomb == NULL) {
        cout << "firing bomb!" << endl;
        myBomb = new BombObject(member);

        Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
        Flt xdir = cos(rad);
        Flt ydir = sin(rad);

        myBomb->pos[0] = parent->ship->pos[0];
        myBomb->pos[1] = parent->ship->pos[1];
        myBomb->vel[0] = xdir*3;
        myBomb->vel[1] = ydir*3;
        myBomb->boom = false;
        
        for (int i = 0; i < 8; i++) {
        cout << "removing bullet" << endl;
        myBullet = barr[i];
        parent->ship->remFilter(myBullet);
        member->remObject(myBullet);
        delete barr[i];
        }

    }

}


void Bomb::physics()
{

    if (myBomb != NULL) {

        if (myBomb->boom) {
            //myBomb = NULL;
            //delete myBomb;

            cout << "went into boom physics" << endl;
            Flt rad = ((myBomb->angle) / 360.0f)*PI*2.0;
            Flt xdir = cos(rad);
            Flt ydir = sin(rad); 
            //int shift = 0;
            for (int i = 0; i < 8; i++) {
                barr[i] = new Bullet(member);    
                myBullet = barr[i]; 
                member->addObject(myBullet);
                cout << "making a bullet member: " << myBullet << endl;
                myBullet->pos[0] = myBomb->pos[0];
                myBullet->pos[1] = myBomb->pos[1];
                myBullet->vel[0] = (xdir)*3;
                myBullet->vel[1] = (ydir)*3;
                myBomb->angle = 90;
                rad = ((myBomb->angle) / 360.0f)*PI*-2.0;
                xdir = cos(rad);
                ydir = cos(rad);
            }
            
            delete myBomb;
            myBomb = NULL;
            return;
        }

        myBomb->pos[0] += myBomb->vel[0];     
        myBomb->pos[1] += myBomb->vel[1];
    }

    if (barr[0] != NULL) {
        for (int i = 0; i < 8; i++) {
            myBullet = barr[i];
            myBullet->pos[0] += myBullet->vel[0] * myBullet->xBounce;
            myBullet->pos[1] += myBullet->vel[1] * myBullet->yBounce;
        }



    }

}
void Bomb::render()
{

    if (barr[0] != NULL) {
        for (int i = 0; i < 8; i++) {
            myBullet = barr[i];
            glPushMatrix();
            glColor3f(0,0,0);
            glTranslatef(myBullet->pos[0], myBullet->pos[1] , 1);
            glBegin(GL_POLYGON);
            glVertex2f(myBullet->w/2,myBullet->h/2);
            glVertex2f(myBullet->w/2, -myBullet->h/2);
            glVertex2f(  -myBullet->w/2,  -myBullet->h/2);
            glVertex2f(-myBullet->w/2,myBullet->h/2 );
            glEnd();
            glPopMatrix();
        }
    } 
    if (myBomb != NULL) {
        glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(myBomb->pos[0], myBomb->pos[1] , 1);
        glBegin(GL_POLYGON);
        glVertex2f(myBomb->w/2,myBomb->h/2);
        glVertex2f(myBomb->w/2, -myBomb->h/2);
        glVertex2f(  -myBomb->w/2,  -myBomb->h/2);
        glVertex2f(-myBomb->w/2,myBomb->h/2 );
        glEnd();
        glPopMatrix();
        cout << "boom status: " << myBomb->boom << endl;
    } 

}

// Bomb Object definitions


BombObject::BombObject(PhysWorld * in_member = NULL) : Object(in_member) 
{
    member = in_member;
    h = 18;
    w = 18;
    objectType = BOMB;
    if (member != NULL) {
        member->addObject(this);
    }
    boom = false;
    angle = 0.0;

}

BombObject::~BombObject()
{

}

void BombObject::handleCollision(Object * in_object)
{

    switch(in_object->objectType)
    {

        case NON:
            break;
        case SHIP:


            break;
        case BULLET:
            break;
        case WALL:
            cout << "wall touched" << endl;
            boom = true;
            member->remObject(this);
            break;

        case ITEMBOX:
            break;
        case BOMB:
            break;
    }

}
