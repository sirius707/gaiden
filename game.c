#include "game.h"

#define MAP_W 200
#define MAP_H 200
#define T_W 10
#define T_H 10

#define MAX_SPD 15.0f
#define SPD_X 60.0f


#define PLR_H_SPD 300
#define JMP_SPD 9.8*20


#define TICKS_PER_FRAME 1000.0f/30.0f
#define FPS 60.0f/1000.0f

SDL_Event sdl_event;
bool quit_event = 0;

bool jump_key = 0;
bool atk_key;
bool left_key;
bool up_key;
bool down_key;
bool right_key;
bool block_key;

bool attackers[N_OB];//if an index is true, then the object with the corresponding index is attacking

uint32_t delta_ticks = 0;
uint32_t elapsed_ticks = 0;
float delta_time;

void g_init()
{
    keystates = SDL_GetKeyboardState(NULL);

    stile_init(&tile, MAP_W, MAP_H);

    animations[0].active = 1;
    animations[0].sprite_h = 65;
    animations[0].sprite_w = 43;

    SDL_Surface *img = SDL_LoadBMP("hagane.bmp");
    animations[0].sprite_sheet = accelerate_surface(img, g_renderer);

    animations[0].frames[IDLE][0].row = 0;
    animations[0].frames[IDLE][0].column = 0;
    animations[0].frames[IDLE][0].active = 1;


    animations[0].frames[WALKING][0].row = 0;
    animations[0].frames[WALKING][0].column = 4;
    animations[0].frames[WALKING][0].active = 1;

    animations[0].frames[WALKING][1].row = 0;
    animations[0].frames[WALKING][1].column = 5;
    animations[0].frames[WALKING][1].active = 1;

    animations[0].frames[WALKING][2].row = 0;
    animations[0].frames[WALKING][2].column = 6;
    animations[0].frames[WALKING][2].active = 1;

    animations[0].frames[WALKING][3].row = 0;
    animations[0].frames[WALKING][3].column = 7;
    animations[0].frames[WALKING][3].active = 1;

    animations[0].frames[WALKING][4].row = 0;
    animations[0].frames[WALKING][4].column = 8;
    animations[0].frames[WALKING][4].active = 1;

    animations[0].frames[JUMP][0].row = 1;
    animations[0].frames[JUMP][0].column = 6;
    animations[0].frames[JUMP][0].active = 1;

    animations[0].frames[FALLING][0].row = 0;
    animations[0].frames[FALLING][0].column = 5;
    animations[0].frames[FALLING][0].active = 1;


    objs[1].pho.h = 30.0f;
    objs[1].pho.w = 20.0f;
    objs[1].pho.tmp_pos.x = 130.0f;
    objs[1].pho.tmp_pos.y = 100.0f;
    objs[1].pho.gravity_scale = 4;
    objs[1].gravity = 1;
    objs[1].state = IDLE;
    objs[1].controller = AI;

    objs[0].atk_info.active_frame = 0;
    objs[0].atk_info.x = 30;
    objs[0].atk_info.y = 10;
    objs[0].atk_info.width = 30;
    objs[0].atk_info.height = 30;


    objs[0].pho.h = 30.0f;
    objs[0].pho.w = 20.0f;
    objs[0].pho.tmp_pos.x = 70.0f;
    objs[0].pho.tmp_pos.y = 100.0f;
    objs[0].pho.gravity_scale = 4;
    objs[0].gravity = 1;
    objs[0].state = IDLE;
    objs[0].controller = HUMAN;
    objs[0].animation_id = 0;

    objs[2].pho.h = 30.0f;
    objs[2].pho.w = 20.0f;
    objs[2].pho.tmp_pos.x = 170.0f;
    objs[2].pho.tmp_pos.y = 100.0f;
    objs[2].pho.gravity_scale = 4;
    objs[2].gravity = 1;
    objs[2].state = IDLE;
    objs[2].controller = AI;

    obstacles[0].pho.h = 20;
    obstacles[0].pho.w = 400;
    obstacles[0].pho.pos.x = 0;
    obstacles[0].pho.pos.y = 500;

    obstacles[1].pho.h = 20;
    obstacles[1].pho.w = 50;
    obstacles[1].pho.pos.x = 130;
    obstacles[1].pho.pos.y = 490;

    obstacles[2].pho.h = 20;
    obstacles[2].pho.w = 400;
    obstacles[2].pho.pos.x = 170;
    obstacles[2].pho.pos.y = 400;

    obstacles[3].pho.h = 20;
    obstacles[3].pho.w = 400;
    obstacles[3].pho.pos.x = 600;
    obstacles[3].pho.pos.y = 450;

    tile.h = T_H;
    tile.w = T_W;

    for(int i = 0; i < N_OBST; i++)
        stile_fill(&tile, obstacles[i].pho.pos.x/tile.w, obstacles[i].pho.pos.y/tile.h, obstacles[i].pho.w / T_W, obstacles[i].pho.h / T_H, 1);

    for(int i = 0; i < N_OB; i++)
        attackers[i] = false;

    cam.lock_area_w = 50;
    cam.lock_area_h = 10;
    cam.w = 640;
    cam.h = 480;
    cam.pos.x = 0;
    cam.pos.y = 0;


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

                    if(sdl_event.type == SDL_KEYDOWN && sdl_event.key.repeat == 0){
                        if(sdl_event.key.keysym.sym == SDLK_LSHIFT){
                            atk_key = 1;
                        }
                    }else if(sdl_event.type == SDL_KEYUP){
                        if(sdl_event.key.keysym.sym == SDLK_LSHIFT)atk_key = 0;
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

       int i;
       PHYSOBJ *pho;

       for(i = 0; i < N_OB; i++){

                       VECTOR initial_pos;
                       pho = &objs[i].pho;

                       initial_pos = pho->pos;

                       if(objs[i].gravity){

                            PHYSOBJ shadow_pho = *pho;
                            shadow_pho.tmp_pos.y += G;

                            if(!collides(&shadow_pho))// this solves the camera jittering problem, not cache efficient, optimise
                                phy_gravity(pho);

                       }


                       //air resistance
                       //pho->vel.x *= 0.8;
                        float tmp_vel_x = fabs(pho->vel.x);
                        float tmp_vel_y = fabs(pho->vel.y);
                           //phy_move_y(pho);
                           pho->tmp_pos.y += pho->vel.y * delta_time;
                           if(collides(pho)){
                                     //resolve collision
                                     pho->y_collision = pho->vel.y;


                                     float sum = tmp_vel_x + tmp_vel_y;

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
                                 pho->x_collision = pho->vel.x;

                                 float sum = tmp_vel_x + tmp_vel_y;

                                 float unit_vel_x = -pho->vel.x/sum;

                                 pho->vel.x  = 0;


                                 while(collides(pho)){


                                                      pho->tmp_pos.x += unit_vel_x;


                                 }


                             }else{
                                pho->x_collision = 0;
                             }

                           phy_commit(pho);//lmao they should haha

                           pho->movement = pho->vel.y;


       }


}


inline void g_update()
{
    gameplay_fsm_step();

    //react to attacks
    for(int i = 0; i < N_OB; i++){
        if(attackers[i]){
            for(int j = 0; j < N_OB; j++){
                if(i == j)continue;

                int x1, y1, h1, w1;
                int x2, y2, h2, w2;

                x1 = objs[i].pho.pos.x + objs[i].atk_info.x;
                y1 = objs[i].pho.pos.y + objs[i].atk_info.y;
                w1 = objs[i].pho.w + objs[i].atk_info.width;
                h1 = objs[i].pho.h + objs[i].atk_info.height;

                x2 = objs[j].pho.pos.x;
                y2 = objs[j].pho.pos.y;
                w2 = objs[j].pho.w;
                h2 = objs[j].pho.h;

                if(aabb_coll(x1, y1, x2, y2, h1, w1, h2, w2)){
                    //process
                    printf("attacked");
                    break;
                }

            }
            attackers[i] = false;
        }
    }
}

inline void g_render()
{

        g_cam_follow(objs[0].pho.tmp_pos, objs[0].pho.w, objs[0].pho.h);

        SDL_Rect rect;

        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
        SDL_RenderClear(g_renderer);


        //render player and enemies and ???
        for(int i = 0; i < N_OB; i++){

            rect.h = objs[i].pho.h;
            rect.w = objs[i].pho.w;
            rect.x = objs[i].pho.pos.x - cam.pos.x;
            rect.y = objs[i].pho.pos.y - cam.pos.y;


            SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(g_renderer, &rect);

            uint8_t ani_id = objs[i].animation_id;
            uint8_t clip = objs[i].current_animation_clip;
            uint8_t frame = objs[i].current_frame;
            SDL_Rect src;
            src.h = animations[ani_id].sprite_h;
            src.w = animations[ani_id].sprite_w;
            src.x = animations[ani_id].frames[clip][frame].column * src.w;
            src.y = animations[ani_id].frames[clip][frame].row * src.h;

            if(objs[i].flip){
                SDL_RenderCopyEx(g_renderer, animations[i].sprite_sheet, &src, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);
            }else{
                SDL_RenderCopy(g_renderer, animations[i].sprite_sheet, &src, &rect);
            }

        }


        //render environment
        for(int i = 0; i < N_OBST; i++){
            if(obstacles[i].pho.pos.x < cam.pos.x + cam.w && obstacles[i].pho.pos.y < cam.pos.y + cam.h){

                rect.h = obstacles[i].pho.h;
                rect.w = obstacles[i].pho.w;
                rect.x = obstacles[i].pho.pos.x - cam.pos.x;
                rect.y = obstacles[i].pho.pos.y - cam.pos.y;

                SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(g_renderer, &rect);
            }
        }


        //render tiles
        for(int j = 0; j < MAX_MAP_Y; j++){
            for(int i = 0; i < MAX_MAP_X; i++){
                 if(tile.map[i][j]){
                    if(i * tile.w < (cam.pos.x + cam.w) && i * tile.w > (cam.pos.x - cam.w) && j * tile.h < (cam.pos.y + cam.h) && j * tile.h > (cam.pos.y - cam.h)){//render tiles only if they are in camera range
                        rect.h = tile.h;
                        rect.w = tile.w;
                        rect.x = i * tile.w - cam.pos.x;
                        rect.y = j * tile.h - cam.pos.y;
                        SDL_SetRenderDrawColor(g_renderer, 255, 0, 255, 255);
                        SDL_RenderDrawRect(g_renderer, &rect);
                    }
                 }
            }
        }


                            //render camera
        rect.h = cam.h;
        rect.w = cam.w;
                rect.x = cam.pos.x - cam.pos.x;
                rect.y = cam.pos.y - cam.pos.y;
                SDL_SetRenderDrawColor(g_renderer, 2, 0, 255, 255);
                SDL_RenderDrawRect(g_renderer, &rect);

                        //render lock area in camera
        //        rect.h = cam.lock_area_h;
        //        rect.w = cam.lock_area_w;
        //        rect.x = cam.pos.x + cam.w/2 - cam.lock_area_w/2 - cam.pos.x;
        //        rect.y = cam.pos.y + cam.h/2 - cam.lock_area_h/2 - cam.pos.y;
        //        SDL_SetRenderDrawColor(g_renderer, 2, 0, 255, 255);
        //        SDL_RenderDrawRect(g_renderer, &rect);

        SDL_RenderPresent(g_renderer);

}


void g_play()
{

    g_init();
     uint64_t t_begin = 0, t_end = 0;
     float elapsed_ms = 0;
     //install_int(s_draw_flag, 80);
     while(!quit_event){

                 t_begin = SDL_GetPerformanceCounter();

                 //input
                 g_input();

                 //update, game logic
                 g_update();

                 //physix
                 g_physics();


                 //animate
                 anim_step();

                //render
                g_render();

                g_tick();
                //rest(50);
                //SDL_Delay(50);


                t_end = SDL_GetPerformanceCounter();
                elapsed_ms = (t_end - t_begin)/(float)SDL_GetPerformanceFrequency()*1000.0f;

                SDL_Delay((TICKS_PER_FRAME - elapsed_ms));



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

inline bool g_within_cam_lock(PHYSOBJ *pho)
{
    float lock_x = cam.pos.x + 640/2;
    float lock_y = cam.pos.y + 480/2;

    return aabb_coll(cam.pos.x, cam.pos.y, lock_x, lock_y, cam.h, cam.w, cam.lock_area_h, cam.lock_area_w);

}

inline void g_cam_follow(VECTOR pos, float w, float h)
{
    int lock_x = cam.pos.x + cam.w/2 - cam.lock_area_w*2;
    int lock_y = cam.pos.y + cam.h/2 - cam.lock_area_h/2;
    int x_dir = 0;
    int y_dir = 0;

    x_dir = (pos.x < lock_x+w*2) * -1;
    x_dir += (pos.x + w) > (lock_x + cam.lock_area_w + w*2);

    y_dir = (pos.y < lock_y) * -1;
    y_dir += (pos.y + cam.lock_area_h) > lock_y;

    cam.pos.x = (pos.x + w/2) - cam.w/2;
    cam.pos.y = (pos.y + h/2) - cam.h/2;

    //stop camera form going over screen
    if(cam.pos.x < 0) cam.pos.x = 0;
    if(cam.pos.x + cam.w > 640*4) cam.pos.x = 640*4 - cam.w;

    if(cam.pos.y < 0) cam.pos.y = 0;
    if(cam.pos.y + cam.h > 480*4) cam.pos.y = 480*4 - cam.h;

    //printf("%d\n", objs[0].state);
}


inline void gameplay_fsm_step()
{
    for(int i = 0; i < N_OB; i++){
            //looks like we will need a separate state machine for AI controlled objects
            //because we need to disable the jump button in code
        if(objs[i].controller == HUMAN){
            gameplay_player_fsm(i);
        }else{
            gameplay_ai_fsm(i);
        }
    }
}


inline void gameplay_ai_fsm(int i)
{
            //objs[i].atk = atk_key;
            //objs[i].move_left = left_key;
            //objs[i].move_right = right_key;
            //objs[i].jump = jump_key;

            switch(objs[i].state){
            case IDLE:

                if(objs[i].move_left || objs[i].move_right){
                    objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));
                    objs[i].state = WALKING;
                }else if(objs[i].jump){
                    objs[i].pho.vel.y -= JMP_SPD;
                    objs[i].state = JUMP;
                }

                if(objs[i].atk){
                    objs[i].state = ATK;
                }
                break;
            case JUMP:

                //objs[i].pho.vel.x += 70 * right_key;
                //objs[i].pho.vel.x -= 70 * left_key;
                objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));

                if(objs[i].pho.y_collision){
                    objs[i].pho.vel.y *= 0.7;
                    objs[i].state = FALLING;
                    objs[i].jump = 0;
                    //jump_key = 0;
                }else if(objs[i].jump){
                    if(objs[i].pho.vel.y > -JMP_SPD*3){
                        objs[i].pho.vel.y -= JMP_SPD;
                    }else{
                        objs[i].pho.vel.y *= 0.7;
                        objs[i].state = FALLING;
                        objs[i].jump = 0;
                    }
                }else{
                    objs[i].pho.vel.y *= 0.7;
                    objs[i].state = FALLING;
                    objs[i].jump = 0;
                }
                break;
            case FALLING:
                //objs[i].pho.vel.x += 70 * right_key;
                //objs[i].pho.vel.x -= 70 * left_key;
                objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));

                if(objs[i].pho.y_collision){
                    objs[i].state = IDLE;
                }
                break;

            case WALKING:

               objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));

                if(!(objs[i].move_right || objs[i].move_left)){
                    objs[i].state = IDLE;
                }else if(objs[i].jump){
                    objs[i].pho.vel.y -= JMP_SPD;
                    objs[i].state = JUMP;
                }
                break;
            case ATK:

                objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));

                if(!(objs[i].move_right || objs[i].move_left)){
                    objs[i].state = IDLE;
                }else if(objs[i].jump){
                    objs[i].pho.vel.y -= JMP_SPD;
                    objs[i].state = JUMP;
                }
                break;

        }
}

int anim_step()
{
    for(int i = 0; i < N_OB; i++){
        int ani_id = objs[i].animation_id;
        int clip = objs[i].current_animation_clip;
        int frame = objs[i].current_frame;

        if(animations[ani_id].frames[clip][frame+1].active){
            objs[i].current_frame++;

        }else{
            objs[i].current_frame = 0;
        }

    }
}
inline void gameplay_player_fsm(int i)
{
            objs[i].atk = atk_key;
            objs[i].move_left = left_key;
            objs[i].move_right = right_key;
            objs[i].jump = jump_key;

            static bool jump_lock = false;

            if(left_key)objs[i].flip = left_key;
            if(right_key)objs[i].flip = 0;

             switch(objs[i].state){
            case IDLE:

                if(objs[i].move_left || objs[i].move_right){
                    objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));
                    objs[i].state = WALKING;

                    objs[i].current_animation_clip = WALKING;
                    objs[i].current_frame = 0;

                }else if(objs[i].jump){
                    objs[i].pho.vel.y -= JMP_SPD;
                    objs[i].state = JUMP;

                    objs[i].current_animation_clip = JUMP;
                    objs[i].current_frame = 0;
                }

                if(objs[i].atk){
                    objs[i].state = ATK;
                }
                break;
            case JUMP:
                //objs[i].pho.vel.x += 70 * right_key;
                //objs[i].pho.vel.x -= 70 * left_key;
                objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));

                if(objs[i].pho.y_collision){//if booped in the head by a ceiling
                    objs[i].pho.vel.y *= 0.7;
                    objs[i].jump = 0;

                    objs[i].state = FALLING;
                    objs[i].current_animation_clip = FALLING;
                    objs[i].current_frame = 0;
                    jump_lock = false;
                    //jump_key = 0;
                }

                if(objs[i].jump && !jump_lock){
                    if(objs[i].pho.vel.y > -JMP_SPD*3){
                        objs[i].pho.vel.y -= JMP_SPD;
                    }else{
                        jump_lock = true;
                        objs[i].pho.vel.y *= 0.7;
                        objs[i].jump = 0;
                    }
                }

                if(objs[i].pho.vel.y > 0){
                    objs[i].pho.vel.y *= 0.7;
                    objs[i].jump = 0;

                    objs[i].state = FALLING;
                    objs[i].current_animation_clip = FALLING;
                    objs[i].current_frame = 0;
                    jump_lock = false;
                }
                break;
            case FALLING:

                objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));

                if(objs[i].pho.y_collision){
                    if(objs[i].move_left || objs[i].move_right){
                        objs[i].state = WALKING;

                        objs[i].current_animation_clip = WALKING;
                        objs[i].current_frame = 0;

                    }else{
                        objs[i].state = IDLE;
                        objs[i].current_animation_clip = IDLE;
                        objs[i].current_frame = 0;
                    }
                }
                break;

            case WALKING:

               objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));
                if(!(objs[i].move_right || objs[i].move_left)){
                    objs[i].state = IDLE;

                    objs[i].current_animation_clip = IDLE;
                    objs[i].current_frame = 0;

                    if(objs[i].pho.y_collision == 0){
                        //objs[i].state = FALLING;
                        //objs[i].current_animation_clip = FALLING;
                        //objs[i].current_frame = 0;
                    }
                }else if(objs[i].jump){
                    objs[i].pho.vel.y -= JMP_SPD;
                    objs[i].state = JUMP;

                    objs[i].current_animation_clip = JUMP;
                    objs[i].current_frame = 0;

                }


                break;
            case ATK:

                objs[i].pho.vel.x = PLR_H_SPD  *(objs[i].move_right + (-objs[i].move_left));

                if(!(objs[i].move_right || objs[i].move_left)){
                    objs[i].state = IDLE;
                }else if(objs[i].jump){
                    objs[i].pho.vel.y -= JMP_SPD;
                    objs[i].state = JUMP;
                }
                break;

        }
}
