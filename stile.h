#ifndef STILE_H
#define STILE_H

#define MAX_MAP_X 400 //default maximum number of x tiles
#define MAX_MAP_Y 400//default maximum number of y tiles

//this will be mainly used for allegro i guess
#define DFLT_W 7//defalt tile width
#define DFLT_H 7//default tile height

#define T_EMPTY 0 //empty tile

/****************************************************************
stile is a tile map library that can be used to create tile maps
fill them with all sorts of data among other things

types: stile : a tile map

functions:
          init: initializes a tile map
          fill: fills a certain area with a certain value
          clear: clears an area
          is_empty: checks whether a certain area is empty or not//this will  be heavily used for collision detection

*****************************************************************/


typedef struct STILE STILE;

struct STILE{

       char map[MAX_MAP_X][MAX_MAP_Y];

       unsigned int x;//width of the map
       unsigned int y;//height of the map

       unsigned int w;//tile width
       unsigned int h;//tile height
};

void stile_load();//loads stile from file, define later

void stile_init(STILE *s, unsigned int w, unsigned int h);//clears the map and resets variables
void stile_fill(STILE *s, unsigned int x, unsigned int y, unsigned int w, unsigned int h, char val);
char stile_is_full(STILE *s, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
//void stile_init(STILE *s);


#endif
