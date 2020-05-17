#ifndef GAME_H
#define GAME_H

#include "my_sdl_util.h"
#include "physics.h"
#include "math.h"
#include "stile.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>


#define N_OB 1
#define N_OBST 2

typedef struct OBJ OBJ;
typedef struct CAM CAM;
typedef enum STATE STATE;

enum STATE{IDLE, WALK, RUN, JUMP, FALLING};

struct CAM{
    float h, w;
    float lock_area_h, lock_area_w;
    VECTOR pos;
};

struct OBJ{
    //animation
    char animation;//if true apply animation; experimetnal

    STATE state;

    size_t hp;

    bool gravity;
    bool move;
    PHYSOBJ pho;

    int img;//number of image

    //control; AI or human
    char control;//0 human, otherwise AI

    char parried;//game specifc
};

STILE tile;

OBJ objs[N_OB];//moving objects
OBJ obstacles[N_OBST];//environment

CAM cam;

//////SDL/////////////////
SDL_Renderer *g_renderer;//for accelerated rendering
const Uint8* keystates;

void g_init();//pre-game loop initializations
void g_play();//start game loop

void g_input();
void g_physics();//apply physics to game objects
void g_update();
void g_render();

void g_cam_follow(VECTOR pos, float w, float h);//follow object with the provided info

char collides(PHYSOBJ *pho);//check if an object collides with the environment
//this is only a wrapper over the function from stile.h
//we should replace this

float clamp(const float x, const float ref);

#endif
