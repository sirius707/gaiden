#include "game.h"

void init();
void deinit();

///////SDL//////////////
SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;

int main( int argc, char* args[] )
{

    init();

    g_play();

    deinit();
	return 0;
}

void init()
{
 //Initialize SDL

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		exit(1);
	}
	else
	{
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			exit(1);
		}
		else
		{
			screenSurface = SDL_GetWindowSurface( window );
        }
	}
	g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


}

void deinit()
{
    SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow( window );
	SDL_Quit();
}


