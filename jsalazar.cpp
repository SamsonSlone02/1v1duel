#include <iostream>
#include "sslone.h"
#include "jsalazar.h"
#include "rrivasnavarr.h"
using namespace std;

Bomb::Bomb(int in_rate = 10, Player * in_parent = NULL, 
           PhysWorld * in_member = NULL) : Weapon(in_rate, in_parent)
{
 
     	Weapon(in_rate, in_parent);
    member = in_member;
    this->parent = in_parent;
    myBomb = NULL;
    bulletCount = 5;
    count = 250;
    for (int i = 0; i < bulletCount; i++) {
        barr[i] = NULL;
    }

}
string Bomb::getWeapon()
{
    return this->type;
}

Bomb::~Bomb()
{  
    for (int i = 0; i < bulletCount; i++) {
        cout << "removing bullet" << endl;
        myBullet = barr[i];
        //parent->ship->remFilter(myBullet);
        member->remObject(myBullet);
        delete myBullet;  
    }
delete myBomb;
}

void Bomb::fireWeapon()
{
    if (myBomb == NULL && count >= 250) {
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
        count = 0;
    }

}


void Bomb::physics()
{

    if (myBomb != NULL) {

        if (myBomb->boom) {
            myBomb->angle = 90;

            for (int i = 0; i < bulletCount; i++) {
                Flt rad = (2*PI) * ((float)i/8) * ((double)(rand()*5)/1000.0);
                Flt xdir = cos(rad);
                Flt ydir = sin(rad);


                barr[i] = new Bullet(member);    
                myBullet = barr[i]; 
                myBullet->member = member;
                myBullet->addFilter(parent->ship);
                parent->ship->addFilter(myBullet); 
                member->addObject(myBullet);
                myBullet->pos[0] = myBomb->pos[0];
                myBullet->pos[1] = myBomb->pos[1];
                myBullet->vel[0] = (-xdir)*2;
                myBullet->vel[1] = (-ydir)*2;
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

    /*
       if (count >= 100) {
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
       }*/

    count++;


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

    Rect r;
    r.bot = parent->ship->pos[1] - 35;
    r.left = parent->ship->pos[0] - 15;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, type);
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
    //h = 10;
    //w = 10; 
    //myBullet->w = 10;
    //myBullet->h = 10;
    
    for (int i = 0; i < 3; i++) {
        sarr[i] = NULL;
    }

}

string Shotgun::getWeapon()
{
    return this->type;
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
            myBullet->vel[0] = (xdir)*3;
            myBullet->vel[1] = (ydir)*3;
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
            //glVertex2f(myBullet->w/2,myBullet->h/2);
            //glVertex2f(myBullet->w/2, -myBullet->h/2);
            //glVertex2f(  -myBullet->w/2,  -myBullet->h/2);
            //glVertex2f(-myBullet->w/2,myBullet->h/2 );
            glVertex2f(4,4);
            glVertex2f(4, -4);
            glVertex2f(  -4,  -4);
            glVertex2f(-4,4);
           
            glEnd();
            glPopMatrix();
        }
    } 

    Rect r;
    r.bot = parent->ship->pos[1] - 35;
    r.left = parent->ship->pos[0] - 15;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, type);

}




//Map 3 Definitions

Map3::Map3()
{
	float h,w;

	float wC[3] = {0,0,0};

	extern PhysWorld * myPhysWorld;
	extern Global gl;
	levelCount = 24;

	//Borders
	level[0] = new Wall(myPhysWorld, 30.0f, gl.xres);
	level[0]->pos[1] = 15;
	myPhysWorld->addObject(level[0]);

	level[1] = new Wall(myPhysWorld, 30.0f, gl.xres);
	level[1]->pos[1] = gl.yres-15;
	myPhysWorld->addObject(level[1]);


	level[2] = new Wall(myPhysWorld, gl.xres, 30.0f);
	level[2]->pos[0] = gl.xres-15;
	myPhysWorld->addObject(level[2]);

	level[3] = new Wall(myPhysWorld, gl.xres, 30.0f);
	level[3]->pos[0] = 15;
	myPhysWorld->addObject(level[3]);

	//Walls
	w = 1000.0f;
	h = 20.0f;
	level[4] = new Wall(myPhysWorld, h, w);
	level[4]->setColor(wC[0],wC[1],wC[2]);
	level[4]->pos[0] = (int)((float)gl.xres * .56f);
	level[4]->pos[1] = (int)((float)gl.yres * .729f);
	myPhysWorld->addObject(level[4]);

	w = 500.0f;
	h = 20.0f; 
	level[5] = new Wall(myPhysWorld, h, w);
	level[5]->setColor(wC[0],wC[1],wC[2]);
	level[5]->pos[0] = (int)((float)gl.xres * .259f);
	level[5]->pos[1] = (int)((float)gl.yres * .853f);
	myPhysWorld->addObject(level[5]);

	// Universal Distance/width/height = 20; 
	//top check
	w = 20.0f;
	h = 100.0f;
	level[6] = new Wall(myPhysWorld, h, w);
	level[6]->setColor(wC[0],wC[1],wC[2]);
	level[6]->pos[0] = (int)((float)gl.xres * .439f);
	level[6]->pos[1] = (int)((float)gl.yres * .902f);//.776f);
	myPhysWorld->addObject(level[6]);   

	//Restricted area
	w = 50.0f;
	h = 50.0f;
	//wC[3] = {255,255,0};
	level[7] = new Wall(myPhysWorld, h, w);
	level[7]->setColor(wC[0],wC[1],wC[2]);
	level[7]->pos[0] = (int)((float)gl.xres * .50f);
	level[7]->pos[1] = (int)((float)gl.yres * .50f);
	myPhysWorld->addObject(level[7]);

	w = 20.0f;
	h = 297.0f;
	level[8] = new Wall(myPhysWorld, h, w);
	level[8]->setColor(wC[0],wC[1],wC[2]);
	level[8]->pos[0] = (int)((float)gl.xres * .908f);
	level[8]->pos[1] = (int)((float)gl.yres * .90f);
	myPhysWorld->addObject(level[8]);


	//right check
	w = 100.0f;
	h = 20.0f;
	level[9] = new Wall(myPhysWorld, h, w);
	level[9]->setColor(wC[0],wC[1],wC[2]);
	level[9]->pos[0] = (int)((float)gl.xres * .945f);
	level[9]->pos[1] = (int)((float)gl.yres * .2f);
	myPhysWorld->addObject(level[9]);


	//left check
	w = 100.0f;
	h = 20.0f;
	level[10] = new Wall(myPhysWorld, h, w);
	level[10]->setColor(wC[0],wC[1],wC[2]);
	level[10]->pos[0] = (int)((float)gl.xres * .05f);
	level[10]->pos[1] = (int)((float)gl.yres * .721f);
	myPhysWorld->addObject(level[10]);

	//down check
	w = 20.0f;
	h = 100.0f;
	level[11] = new Wall(myPhysWorld, h, w);
	level[11]->setColor(wC[0],wC[1],wC[2]);
	level[11]->pos[0] = (int)((float)gl.xres * .359f);
	level[11]->pos[1] = (int)((float)gl.yres * .433f);
	myPhysWorld->addObject(level[11]);

	w = 20.0f;
	h = 100.0f;
	level[12] = new Wall(myPhysWorld, h, w);
	level[12]->setColor(wC[0],wC[1],wC[2]);
	level[12]->pos[0] = (int)((float)gl.xres * .206f);
	level[12]->pos[1] = (int)((float)gl.yres * .68f);
	myPhysWorld->addObject(level[12]);


	w = 100.0f;
	h = 20.0f;
	level[13] = new Wall(myPhysWorld, h, w);
	level[13]->setColor(wC[0],wC[1],wC[2]);
	level[13]->pos[0] = (int)((float)gl.xres * .234f);
	level[13]->pos[1] = (int)((float)gl.yres * .605f);
	myPhysWorld->addObject(level[13]);


	w = 20.0f;
	h = 100.0f;
	level[14] = new Wall(myPhysWorld, h, w);
	level[14]->setColor(wC[0],wC[1],wC[2]);
	level[14]->pos[0] = (int)((float)gl.xres * .275f);
	level[14]->pos[1] = (int)((float)gl.yres * .556f);
	myPhysWorld->addObject(level[14]);


	w = 100.0f;
	h = 20.0f;
	level[15] = new Wall(myPhysWorld, h, w);
	level[15]->setColor(wC[0],wC[1],wC[2]);
	level[15]->pos[0] = (int)((float)gl.xres * .317f);
	level[15]->pos[1] = (int)((float)gl.yres * .507f);
	myPhysWorld->addObject(level[15]);

	w = 20.0f;
	h = 100.0f;
	level[16] = new Wall(myPhysWorld, h, w);
	level[16]->setColor(wC[0],wC[1],wC[2]);
	level[16]->pos[0] = (int)((float)gl.xres * .359f);
	level[16]->pos[1] = (int)((float)gl.yres * .556f);
	myPhysWorld->addObject(level[16]);

	//using rn
	w = 100.0f;
	h = 20.0f;
	level[17] = new Wall(myPhysWorld, h, w);
	level[17]->setColor(wC[0],wC[1],wC[2]);
	level[17]->pos[0] = (int)((float)gl.xres * .484f);
	level[17]->pos[1] = (int)((float)gl.yres * .1f);//.557f);
	myPhysWorld->addObject(level[17]);

	w = 20.0f;
	h = 380.0f;
	level[18] = new Wall(myPhysWorld, h, w);
	level[18]->setColor(wC[0],wC[1],wC[2]);
	level[18]->pos[0] = (int)((float)gl.xres * .442f);
	level[18]->pos[1] = (int)((float)gl.yres * .386f);
	myPhysWorld->addObject(level[18]);

	w = 20.0f;
	h = 100.0f;
	level[19] = new Wall(myPhysWorld, h, w);
	level[19]->setColor(wC[0],wC[1],wC[2]);
	level[19]->pos[0] = (int)((float)gl.xres * .80f);
	level[19]->pos[1] = (int)((float)gl.yres * .3f);
	myPhysWorld->addObject(level[19]);

	w = 20.0f;
	h = 200.0f;
	level[20] = new Wall(myPhysWorld, h, w);
	level[20]->setColor(wC[0],wC[1],wC[2]);
	level[20]->pos[0] = (int)((float)gl.xres * .359f);
	level[20]->pos[1] = (int)((float)gl.yres * .124f);
	myPhysWorld->addObject(level[20]);

	w = 564.0f;
	h = 20.0f;
	level[21] = new Wall(myPhysWorld, h, w);
	level[21]->setColor(wC[0],wC[1],wC[2]);
	level[21]->pos[0] = (int)((float)gl.xres * .714f);
	level[21]->pos[1] = (int)((float)gl.yres * .163f);
	myPhysWorld->addObject(level[21]);

	w = 164.0f;
	h = 20.0f;
	level[22] = new Wall(myPhysWorld, h, w);
	level[22]->setColor(wC[0],wC[1],wC[2]);
	level[22]->pos[0] = (int)((float)gl.xres * .142f);
	level[22]->pos[1] = (int)((float)gl.yres * .729f);
	myPhysWorld->addObject(level[22]);


	w = 20.0f;
	h = 380.0f;
	level[23] = new Wall(myPhysWorld, h, w);
	level[23]->setColor(wC[0],wC[1],wC[2]);
	level[23]->pos[0] = (int)((float)gl.xres * .2f);
	level[23]->pos[1] = (int)((float)gl.yres * .386f);
	myPhysWorld->addObject(level[23]);



}

void Map3::render()
{
	//extern Global gl;


	/*
	   glColor3f(0,0,0);
	   glBegin(GL_POLYGON);
	   glVertex2i(0,0);
	   glVertex2i(gl.xres,0);
	   glVertex2i(gl.xres,gl.yres);
	   glVertex2i(0,gl.yres);
	   glEnd();
	   */

	for(int i = 0; i < levelCount; i++) {

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

}
