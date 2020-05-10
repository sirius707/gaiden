#include "stile.h"

void stile_init(STILE *s, unsigned int w, unsigned int h)
{
     s->x = w;
     s->y = h;
     
     
     int i, j;
     for(i = 0; i < w; i++){
              for(j = 0; j < h; j++){
                    s->map[i][j] = T_EMPTY;
              }
    }
}


void stile_fill(STILE *s, unsigned int x, unsigned int y, unsigned int w, unsigned int h, char val)
{
     int i, j;
     
     if(x > s->x || y > s->y ){
          return;
     }
     
     if(x < 0) x = 0;
     if(y < 0) y = 0;
     
     if(x + w > s->x)w = s->x - x;
     if(y + h > s->y)h = s->y - y;
     
     for(i = 0; i < w; i++){
           for(j = 0; j < h; j++){
                 //we should add bounds checking here
                 
                 s->map[x+i][y+j] = val;
                 //printf("%d %d\n", x+i, y+j);
                 
           }
     }
}


inline char stile_is_full(STILE *s, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{//detemines if an area is not empty
     int i, j;
     
     for(i = x; i <= x+w; i++){
             for(j = y; j <= y+h; j++){
                   if(s->map[i][j] != T_EMPTY){
                                return 1;
                                
                   }
             }
       }
       
     return 0;
       //in boolean functions you should always have 2 return statements for clarity
}
