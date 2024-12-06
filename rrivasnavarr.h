#ifndef RRIVASNAVARR_H
#define RRIVASNAVARR_H

#include <ctime>
#include<iostream>
#include<cstring>
#include <GL/glx.h>
#include<cmath>
#include "fonts.h"

#include "sslone.h"

class Image {
public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
                if (fname[0] == '\0')
                        return;
                //printf("fname **%s**\n", fname);
                int ppmFlag = 0;
                char name[40];
                strcpy(name, fname);
                int slen = strlen(name);
                char ppmname[80];
                if (strncmp(name+(slen-4), ".ppm", 4) == 0)
                        ppmFlag = 1;
                if (ppmFlag) {
                        strcpy(ppmname, name);
                } else {
                        name[slen-4] = '\0';
                        //printf("name **%s**\n", name);
                        sprintf(ppmname,"%s.ppm", name);
                        //printf("ppmname **%s**\n", ppmname);
                        char ts[100];
                        //system("convert eball.jpg eball.ppm");
                        sprintf(ts, "convert %s %s", fname, ppmname);
                        system(ts);
                }
                //sprintf(ts, "%s", name);
                //sprintf(ts, "%s", name);
                FILE *fpi = fopen(ppmname, "r");
                if (fpi) {
                        char line[200];
                        fgets(line, 200, fpi);
                        fgets(line, 200, fpi);
                        //skip comments and blank lines
                        while (line[0] == '#' || strlen(line) < 2)
                                fgets(line, 200, fpi);
                        sscanf(line, "%i %i", &width, &height);
                        fgets(line, 200, fpi);
                        //get pixel data
                        int n = width * height * 3;
                        data = new unsigned char[n];
                        for (int i=0; i<n; i++)
                                data[i] = fgetc(fpi);
                        fclose(fpi);
                } else {
                        printf("ERROR opening image: %s\n",ppmname);
                        exit(0);
                }
                if (!ppmFlag)
                        unlink(ppmname);
        }
};


class Wall;

class Wall:public Object
{
    public:
        Wall(PhysWorld * in_member, float width, float height);
        ~Wall();
        void render();
};

class Map: public BaseMap
{
    public:
        Map();
        ~Map();
        void render();
};

class Menu
{
    public:
        Menu();
        ~Menu();
        void render();
};


#endif
