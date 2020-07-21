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


#define N_OB 3
#define N_OBST 4

typedef struct OBJ OBJ;
typedef struct CAM CAM;
typedef struct CONTROL_INFO CONTROL_INFO;
typedef struct ATK_INF ATK_INF;
typedef enum PLAYER_STATE PLAYER_STATE;
typedef enum CONTROLLER CONTROLLER;

enum PLAYER_STATE{IDLE, WALKING, RUN, JUMP, FALLING, ATK};
enum CONTROLLER{HUMAN, AI};

struct CONTROL_INFO{//info in this struct will be used to control game object either through AI or input
    bool move_left;
    bool move_right;
    bool jump;
    bool atk;
};

struct ATK_INF{
    float x, y;
    short active_frame;//activate at which frame
    float width, height;
};

//lock area x is always greater than cam.x by screen_w/2
//lock area y is always greater than cam.y by screen_h/2

struct CAM{
    float h, w;
    float lock_area_h, lock_area_w;
    VECTOR pos;
};

struct OBJ{
    //animation
    char animation;//if true apply animation; experimetnal

    PLAYER_STATE state;

    size_t hp;

    bool gravity;
    bool move;
    PHYSOBJ pho;

    ATK_INF atk_info;

    int img;//index of image

    //control; AI or human
    CONTROLLER controller;//0 human, otherwise AI, use ENUM instead
    struct CONTROL_INFO;

    char parried;//game specifc, use std bool instead
};



STILE tile;

OBJ objs[N_OB];//moving objects
OBJ obstacles[N_OBST];//environment

CAM cam;

//////SDL/////////////////
SDL_Renderer *g_renderer;//for accelerated rendering
const Uint8* keystates;

////////////////////////////////////////ESSENTIAL///////////////////////////////////
void g_init();//pre-game loop initializations
void g_play();//start game loop

void g_input();
void g_physics();//apply physics to game objects
void g_update();
void g_render();
///////////////////////////////////////////////////////////////////////////////////

void g_cam_follow(VECTOR pos, float w, float h);//follow object with the provided info

char collides(PHYSOBJ *pho);//check if an object collides with the environment
//this is only a wrapper over the function from stile.h
//we should replace this
bool g_within_cam_lock(PHYSOBJ *pho);

float clamp(const float x, const float ref);

void gameplay_fsm_step();

#endif
