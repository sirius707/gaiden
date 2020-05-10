#include "physics.h"

#include <math.h>
#include <stdlib.h>


#include <stdio.h>



/******************************************
notes to self.
consider changing the floats in pos to ints
if you're going to use a grid based system
to improve performance ;)

edit: changing the variables from float to int actually stopped the problem of STICKY walls i was getting
when performing a jump then moving on the ground
but now i get seemingly random crashes
edit: nvm floats are better
*******************************************/


//will add a gravity bool to the pho struct to enable and disable gravity

void phy_gravity(PHYSOBJ *o)
{
                  o->vel.y += G * o->gravity_scale;
}


void phy_commit(PHYSOBJ *o)
{

    o->pos.y = o->tmp_pos.y;
    o->pos.x = o->tmp_pos.x;

}

void phy_move(PHYSOBJ *o)
{
    o->tmp_pos.x += o->vel.x;
    o->tmp_pos.y += o->vel.y;
}

void phy_move_x(PHYSOBJ *o)
{
    o->tmp_pos.x += o->vel.x;
}

void phy_move_y(PHYSOBJ *o)
{
   o->tmp_pos.y += o->vel.y;
}

void phy_init(PHYSOBJ *o, float x, float y, float h, float w)
{
    o->acc.x = 0;
    o->acc.y = 0;


    o->max_spd = 10;

    o->tmp_pos.x = 0;
    o->tmp_pos.y = 0;

    o->vel.x = 0;
    o->vel.y = 0;

    o->h = h;
    o->w = w;

    o->tmp_pos.x = x;
    o->tmp_pos.y = y;

    o->pos.x = x;
    o->pos.y = y;

}
int phy_coll(PHYSOBJ *o1, PHYSOBJ *o2)
{
    int c1, c2, c3, c4;//conditions that must be fulfilled to know that there is no collision
    c1 = o1->tmp_pos.y >= o2->tmp_pos.y + o2->h;
    c2 = o1->tmp_pos.x >= o2->tmp_pos.x + o2->w;
    c3 = o2->tmp_pos.y >= o1->tmp_pos.y + o1->h;
    c4 = o2->tmp_pos.x >= o1->tmp_pos.x + o1->w;

    return !(c1 || c2 || c3 || c4);
}

inline int aabb_coll(float x1, float y1, float x2, float y2, float h1, float w1, float h2, float w2)
{

    int c1, c2, c3, c4;//conditions that must be fulfilled to know that there is no collision
    c1 = y1 >= y2 + h2;
    c2 = x1 >= x2 + w2;
    c3 = y2 >= y1 + h1;
    c4 = x2 >= x1 + w1;

    //printf("%f %f %f %f %f %f %f %f\n", x1, y1, x2, y2, w1, h1, w1, h2, w2 );
    return !(c1 || c2 || c3 || c4);

    return 1;
}


//returns distance squared
float sq_distance(const VECTOR *a, const VECTOR *b)
{
    float dist_x;
    float dist_y;

    dist_x = b->x - a->x;
    dist_y = b->y - a->y;

    return (dist_x*dist_x) + (dist_y*dist_y);
}

float distance(const VECTOR *a, const VECTOR *b)
{
    float dist_x;
    float dist_y;

    dist_x = b->x - a->x;
    dist_y = b->y - a->y;

    return sqrt((dist_x*dist_x) + (dist_y*dist_y));
}
