#include <iostream>
#include "sslone.h"
#include "jsalazar.h"
#include "rrivasnavarr.h"
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
   bulletCount = 5;

    for (int i = 0; i < bulletCount; i++) {
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
    delete myBullet;  
    }
}

void Bomb::fireWeapon()
{
    if (myBomb == NULL) {
        cout << "firing bomb!" << endl;
        member->printArr();
        myBomb = new BombObject(member);
        myBomb->clearFilter();
         
        Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
        Flt xdir = cos(rad);
        Flt ydir = sin(rad);

        myBomb->pos[0] = parent->ship->pos[0];
        myBomb->pos[1] = parent->ship->pos[1];
        myBomb->vel[0] = xdir*4;
        myBomb->vel[1] = ydir*4;
        myBomb->boom = false;

        if (barr[0] != NULL) {
            for (int i = 0; i < bulletCount; i++) {
                cout << "removing bullet" << endl;
                myBullet = barr[i];
                parent->ship->remFilter(myBullet);
                myBullet->clearFilter();
                parent->ship->remFilter(myBullet); 
                member->remObject(myBullet);
                delete barr[i];
                barr[i] = NULL;
            }
        }
    }

}


void Bomb::physics()
{

    if (myBomb != NULL) {

        if (myBomb->boom) {
            //myBomb = NULL;
            //delete myBomb;
            myBomb->angle = 90;

            cout << "went into boom physics" << endl;
            //Flt rad = (2*PI) * ((float)i/8) * ((double)(rand()*5)/1000.0);
            //Flt xdir = cos(rad);
            //Flt ydir = sin(rad); 
            //int shift = -70;
            for (int i = 0; i < bulletCount; i++) {
                Flt rad = (2*PI) * ((float)i/8) * ((double)(rand()*5)/1000.0);
            Flt xdir = cos(rad);
            Flt ydir = sin(rad);
                
                
                barr[i] = new Bullet(member);    
                myBullet = barr[i]; 
                myBullet->member = member;
                myBullet->addFilter(parent->ship);
		cout << "added to filter" << endl;
                parent->ship->addFilter(myBullet); 
                member->addObject(myBullet);
                cout << "making a bullet member: " << myBullet << endl;
                myBullet->pos[0] = myBomb->pos[0];
                myBullet->pos[1] = myBomb->pos[1];
                myBullet->vel[0] = (-xdir)*2;
                myBullet->vel[1] = (-ydir)*2;
                //myBomb->angle += 30;
                //shift += 35;
                //rad = ((myBomb->angle) / 360.0f)*PI*2.0;
                //rad = ((parent->ship->angle+shift) / 360.0f) * PI * 2.0;
                //xdir = cos(rad);
                //ydir = cos(rad);
            }
            delete myBomb;
            myBomb = NULL;
            return;
        }

        myBomb->pos[0] += myBomb->vel[0];     
        myBomb->pos[1] += myBomb->vel[1];
    }

    if (barr[0] != NULL) {
        for (int i = 0; i < bulletCount; i++) {
            myBullet = barr[i];
            myBullet->pos[0] += myBullet->vel[0] * myBullet->xBounce;
            myBullet->pos[1] += myBullet->vel[1] * myBullet->yBounce;
        }



    }

}
void Bomb::render()
{

    if (barr[0] != NULL) {
        for (int i = 0; i < bulletCount; i++) {
            myBullet = barr[i];
            glPushMatrix();
            glColor3f(parent->ship->color[0],parent->ship->color[1],parent->ship->color[2]);
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


//Shotgun Definition
Shotgun::Shotgun(int in_rate = 10, Player * in_parent = NULL, 
        PhysWorld * in_member = NULL) : Weapon(in_rate, in_parent)
{
    Weapon(in_rate, in_parent);
    member = in_member;
    this->parent = in_parent;
    myBullet = NULL;
    isBullet = false;

    for (int i = 0; i < 3; i++) {
        sarr[i] = NULL;
    }

}

Shotgun::~Shotgun()
{  
    for (int i = 0; i < 3; i++) {
        cout << "removing bullet" << endl;
        myBullet = sarr[i];
        parent->ship->remFilter(myBullet);
        member->remObject(myBullet);
    }
    delete myBullet;
}

void Shotgun::fireWeapon()
{

    if (isBullet) {
        for (int i = 0; i < 3; i++) {
            cout << "removing bullet" << endl;
            myBullet = sarr[i];
            parent->ship->remFilter(myBullet);
            myBullet->clearFilter();
            parent->ship->remFilter(myBullet);
            member->remObject(myBullet);
            delete sarr[i];
        }
        isBullet = false;
    }

    if(!isBullet) {
        Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
        Flt xdir = cos(rad);
        Flt ydir = sin(rad);
        int shift = 70;
        for (int i = 0; i < 3; i++) {
            sarr[i] = new Bullet(member);    
            myBullet = sarr[i]; 
            myBullet->member = member;
            myBullet->addFilter(parent->ship);
            cout << "added to filter" << endl;
            parent->ship->addFilter(myBullet);          
            member->addObject(myBullet);
            cout << "making a bullet member: " << myBullet << endl;
            myBullet->pos[0] = parent->ship->pos[0];
            myBullet->pos[1] = parent->ship->pos[1];
            myBullet->vel[0] = (xdir)*2;
            myBullet->vel[1] = (ydir)*2;
            shift += 14;
            rad = ((parent->ship->angle+shift) / 360.0f) * PI * 2.0; 
            xdir = cos(rad);
            ydir = sin(rad);
        }
        isBullet = true;
    }
}


void Shotgun::physics()
{

    if (sarr[0] != NULL) {
        for (int i = 0; i < 3; i++) {
            myBullet = sarr[i];
            myBullet->pos[0] += myBullet->vel[0] * myBullet->xBounce;
            myBullet->pos[1] += myBullet->vel[1] * myBullet->yBounce;
        }
    }



}
void Shotgun::render()
{

    if (sarr[0] != NULL) {
        for (int i = 0; i < 3; i++) {
            myBullet = sarr[i];
            glPushMatrix();
            glColor3f(parent->ship->color[0],
                    parent->ship->color[1],
                    parent->ship->color[2]);
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

}



/*
//Map 3 Definitions
Map3::Map3()
{
    float h,w;

    float wC[3] = {0,0,255};

    extern PhysWorld * myPhysWorld;
    extern Global gl;
    
    w = 75.0f;
    h = 75.0f;
    level[0] = new Wall(myPhysWorld, h, w);  
    level[0]->pos[0] = (int)((float)gl.xres *.2f); 
    level[0]->pos[1] = (int)((float)gl.xres *.75f);  
    myPhysWorld-> addObject(level[0]);

    level[1] = new Wall(myPhysWorld, 30.0f, gl.xres);
    level[1]->pos[1] = 15;
    myPhysWorld->addObject(level[1]);

    level[2] = new Wall(myPhysWorld, 30.0f, gl.xres);
    level[2]->pos[1] = gl.yres-15;
    myPhysWorld->addObject(level[2]);


    level[3] = new Wall(myPhysWorld, gl.xres, 30.0f);
    level[3]->pos[0] = gl.xres-15;
    myPhysWorld->addObject(level[3]);

    level[4] = new Wall(myPhysWorld, gl.xres, 30.0f);
    level[4]->pos[0] = 15;
    myPhysWorld->addObject(level[4]);




}

void Map3::render()
{
    for(int i = 0; i < 5; i++) {

        if (level[i] != NULL) {

            Object * l = level[i];
            glPushMatrix();
            glColor3f(l->color[0], l->color[1], l->color[2]);
            glTranslatef(l->pos[0], l->pos[1], 1);
            glBegin(GL_POLYGON);
            glVertex2f(l->w/2,l->h/2);
            glVertex2f(l->w/2, -l->h/2);
            glVertex2f(  -l->w/2,  -l->h/2);
            glVertex2f(-l->w/2,l->h/2 );
            glEnd();
            glPopMatrix();

        }


    }

}

Map3::~Map3()
{

}*/
