#include "game.h"

#define MAP_W 400
#define MAP_H 400
#define T_W 10
#define T_H 10

#define MAX_SPD 15.0f
#define SPD_X 15.0f
#define JMP_SPD G*18.0f / 5

#define PLR_H_SPD 350
#define JMP_SPD 9.8*30

SDL_Event sdl_event;
bool quit_event = 0;

bool jump_key = 0;
bool atk_key;
bool left_key;
bool up_key;
bool down_key;
bool right_key;
bool block_key;


uint32_t delta_ticks = 0;
uint32_t elapsed_ticks = 0;
float delta_time;

void g_init()
{
    keystates = SDL_GetKeyboardState(NULL);

    stile_init(&tile, MAP_W, MAP_H);

    objs[0].pho.h = 50.0f;
    objs[0].pho.w = 20.0f;
    objs[0].pho.tmp_pos.x = 100.0f;
    objs[0].pho.tmp_pos.y = 100.0f;
    objs[0].pho.gravity_scale = 4.2;
    objs[0].gravity = 1;
    objs[0].state = IDLE;

    obstacles[0].pho.h = 50;
    obstacles[0].pho.w = 400;
    obstacles[0].pho.pos.x = 0;
    obstacles[0].pho.pos.y = 290;

    obstacles[1].pho.h = 50;
    obstacles[1].pho.w = 400;
    obstacles[1].pho.pos.x = 200;
    obstacles[1].pho.pos.y = 340;

    for(int i = 0; i < N_OBST; i++)
    stile_fill(&tile, obstacles[i].pho.pos.x / T_W, obstacles[i].pho.pos.y / T_H, obstacles[i].pho.w / T_W, obstacles[i].pho.h / T_H, 1);


}

void inline g_input()
{
    left_key = right_key = down_key = up_key = 0;

    while(SDL_PollEvent(&sdl_event)){
                    if(sdl_event.type == SDL_QUIT){
                        quit_event = 1;
                    }

                    if(sdl_event.type == SDL_KEYDOWN && sdl_event.key.repeat == 0){
                        if(sdl_event.key.keysym.sym == SDLK_SPACE){
                            jump_key = 1;
                        }
                    }else if(sdl_event.type == SDL_KEYUP){
                        if(sdl_event.key.keysym.sym == SDLK_SPACE)jump_key = 0;
                    }

    }

    if(keystates[SDL_SCANCODE_LEFT])
        left_key = 1;
    else if(keystates[SDL_SCANCODE_RIGHT])
        right_key = 1;


}




inline void g_physics()
{

       /************************************************
       *apply physics to game objects
       *************************************************/
       //collisions are handled only against objects that are drawn to the stilemap

       int i, j;
       PHYSOBJ *pho;

       for(i = 0; i < N_OB; i++){


                       pho = &objs[i].pho;

                       if(objs[i].gravity){
                            phy_gravity(pho);
                       }


                       //air resistance
                       //pho->vel.x *= 0.8;

                           //phy_move_y(pho);
                           pho->tmp_pos.y += pho->vel.y * delta_time;
                           if(collides(pho)){
                                 //resolve collision
                                 pho->y_collision = 1;

                                 float tmp_vel_x = fabs(pho->vel.x);
                                 float tmp_vel_y = fabs(pho->vel.y);

                                 float sum = tmp_vel_x + tmp_vel_y;


                                 float unit_vel_x = -pho->vel.x/sum;
                                 float unit_vel_y = -pho->vel.y/sum;

                                 pho->vel.y  = 0;


                                 while(collides(&objs[i].pho)){

                                                      pho->tmp_pos.y += unit_vel_y;


                                 }

                             }else{
                                pho->y_collision = 0;
                             }

                           //phy_move_x(pho);
                           pho->tmp_pos.x += pho->vel.x * delta_time;
                           if(collides(pho)){
                                 pho->x_collision = 1;

                                 float tmp_vel_x = fabs(pho->vel.x);
                                 float tmp_vel_y = fabs(pho->vel.y);

                                 float sum = tmp_vel_x + tmp_vel_y;

                                 float unit_vel_x = -pho->vel.x/sum;
                                 float unit_vel_y = -pho->vel.y/sum;

                                 pho->vel.x  = 0;


                                 while(collides(pho)){


                                                      pho->tmp_pos.x += unit_vel_x;


                                 }


                             }else{
                                pho->x_collision = 0;
                             }

                           phy_commit(pho);//lmao they should haha




       }


}


inline void g_update()
{
    for(int i = 0; i < N_OB; i++){

        switch(objs[i].state){
            case IDLE:
                //objs[i].pho.vel.x += 70 * right_key;
                //objs[i].pho.vel.x -= 70 * left_key;
                objs[i].pho.vel.x = PLR_H_SPD  *(right_key + (-left_key));

                if(jump_key){
                    objs[i].pho.vel.y -= JMP_SPD;
                    objs[i].state = JUMP;
                }
                break;
            case JUMP:

                //objs[i].pho.vel.x += 70 * right_key;
                //objs[i].pho.vel.x -= 70 * left_key;
                objs[i].pho.vel.x = PLR_H_SPD  *(right_key + (-left_key));

                if(jump_key){
                    if(objs[i].pho.vel.y > -JMP_SPD*3){
                        objs[i].pho.vel.y -= JMP_SPD;
                    }else{
                        objs[i].pho.vel.y *= 0.7;
                        objs[i].state = FALLING;
                        jump_key = 0;
                    }
                }else{
                    objs[i].pho.vel.y *= 0.7;
                    objs[i].state = FALLING;
                    jump_key = 0;
                }
                break;
            case FALLING:
                //objs[i].pho.vel.x += 70 * right_key;
                //objs[i].pho.vel.x -= 70 * left_key;
                objs[i].pho.vel.x = PLR_H_SPD  *(right_key + (-left_key));

                if(objs[i].pho.y_collision){
                    objs[i].state = IDLE;
                }
                break;

        }
    }


}

inline void g_render()
{
        SDL_Rect rect;

        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
        SDL_RenderClear(g_renderer);

        for(int i = 0; i < N_OB; i++){

            rect.h = objs[i].pho.h;
            rect.w = objs[i].pho.w;
            rect.x = objs[i].pho.pos.x;
            rect.y = objs[i].pho.pos.y;

            SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(g_renderer, &rect);
        }

        for(int i = 0; i < N_OBST; i++){

            rect.h = obstacles[i].pho.h;
            rect.w = obstacles[i].pho.w;
            rect.x = obstacles[i].pho.pos.x;
            rect.y = obstacles[i].pho.pos.y;

            SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(g_renderer, &rect);
        }

        SDL_RenderPresent(g_renderer);

}


void g_play()
{

    g_init();
     //install_int(s_draw_flag, 80);
     while(!quit_event){

                 //input
                 g_input();

                 //update, game logic
                 g_update();

                 //physix
                 g_physics();


                 //animate
                 for(int i = 0; i < N_OB; i++){
                       //anim_move(&objs[i].sprites);
                 }


                //render
                g_render();

                g_tick();
                //rest(50);
                //SDL_Delay(50);

                printf("%f\r", delta_time);

     }


}



inline char collides(PHYSOBJ *pho)//check if an object collides with the environment
{

     int w, h;
     int x, y;

     w = pho->w / T_W;
     h = pho->h / T_H;

     x = pho->tmp_pos.x/T_W;
     y = pho->tmp_pos.y/T_H;

      return stile_is_full(&tile, x, y, w, h);
}

inline float clamp(const float x, const float ref)
{
       return (x > ref) ? ref : x;
}

//sets delta time
inline void g_tick()
{
    static uint32_t tmp_ticks;

    tmp_ticks = SDL_GetTicks();
    delta_ticks = tmp_ticks - elapsed_ticks;
    elapsed_ticks = tmp_ticks;
    delta_time = delta_ticks / 1000.0f;
}


