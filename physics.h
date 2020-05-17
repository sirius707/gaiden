#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>

#define G 9.8


#define SIDES 4 //number of square sides


//physics object: will be used to keep track of velocities and other physical properties
typedef struct VECTOR   VECTOR;
typedef struct PHYSOBJ PHYSOBJ;
typedef struct RECT       RECT;

struct VECTOR{
    float x;
    float y;
};

struct RECT{

    VECTOR points[SIDES];

};

struct PHYSOBJ{

    VECTOR vel;
    VECTOR tmp_vel;//temporary velocity

    VECTOR pos;

    VECTOR tmp_pos;

    float h, w;//well apparently not using size_t instead of float can make squares fucking disappear when rendered

    float gravity_scale;

    VECTOR acc;

    //true if a collision occurs
    char x_collision;
    char y_collision;

    bool movement; // true if movement ocurred
    float max_spd;//highest reachable speed

};


void phy_gravity(PHYSOBJ *o);
void phy_commit(PHYSOBJ *o);//commits tmp variables to normal spatial variables
void phy_move(PHYSOBJ *o);//adds velcity to temporary spatial variables
void phy_move_x(PHYSOBJ *o);//adds velcity to temporary spatial variables
void phy_move_y(PHYSOBJ *o);//adds velcity to temporary spatial variables

void phy_init(PHYSOBJ *o, float x, float y, float h, float w);//initializes an object, using files to load objects might render it useless
int phy_coll(PHYSOBJ *o1, PHYSOBJ *o2);//return true on collision, false other wise
int aabb_coll(float x1, float y1, float x2, float y2, float h1, float w1, float h2, float w2);
float x_distance(PHYSOBJ *const o1, PHYSOBJ *const o2);
float y_distance(PHYSOBJ *const o1, PHYSOBJ *const o2);
float distance(const VECTOR *a, const VECTOR *b);

void phy_load();//loads from file, define later

void putint(char *name, int n);
/*char inline get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y);//this is wrapped in intersect();
*/
char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y);

char intersect(const VECTOR *a, const VECTOR *b, const VECTOR *c, const VECTOR *d, VECTOR *res);//returns true if 2 lines intersect and adds the intersection point in res

float sq_distance(const VECTOR *a, const VECTOR *b);



#endif
