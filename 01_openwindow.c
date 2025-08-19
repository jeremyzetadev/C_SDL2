#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "01 OpenWindow"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
};


void game_cleanup(struct Game *game);
bool sdl_initialize(struct Game *game);

int main(){
    // Initialize game window and renderer
    struct Game game = {
        .window = NULL,
        .renderer = NULL
    };

    if (!sdl_initialize(&game)){
        game_cleanup(&game);
        printf("error happened");
        exit(0);
    }

    SDL_RenderClear(game.renderer);
    SDL_RenderPresent(game.renderer);
    SDL_Delay(5000);

    printf("All good!");

    game_cleanup(&game);
    return 0;
}

void game_cleanup(struct Game *game){
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);
    SDL_Quit();
}

bool sdl_initialize(struct Game *game){
    if (SDL_Init(SDL_INIT_EVERYTHING)== -1){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!game->window){
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if(!game->renderer){
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}
