#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_TITLE "03 Background"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define IMAGE_FLAGS IMG_INIT_PNG

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
};


void game_cleanup(struct Game *game, int exit_status);
bool load_media(struct Game *game);
bool sdl_initialize(struct Game *game);

int main(){
    // Initialize game window and renderer
    struct Game game = {
        .window = NULL,
        .renderer = NULL,
        .background = NULL
    };

    if (!sdl_initialize(&game)){
        game_cleanup(&game, EXIT_FAILURE);
    }
    if (!load_media(&game)){
        game_cleanup(&game, EXIT_FAILURE);
    }

    bool isGameRunning = true;
    while(isGameRunning){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    game_cleanup(&game, EXIT_SUCCESS);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode){
                        case SDL_SCANCODE_ESCAPE:
                            game_cleanup(&game, EXIT_SUCCESS);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        SDL_RenderClear(game.renderer);
        SDL_RenderCopy(game.renderer, game.background, NULL, NULL);
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }

    printf("All good!");

    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
}

void game_cleanup(struct Game *game, int exit_status){
    SDL_DestroyTexture(game->background);
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);
    SDL_Quit();
    IMG_Quit();
    exit(exit_status);
}

bool load_media(struct Game *game){
    game->background = IMG_LoadTexture(game->renderer, "images/background.png");
    if (!game->background){
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

bool sdl_initialize(struct Game *game){
    if (SDL_Init(SDL_INIT_EVERYTHING)== -1){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    int img_init = IMG_Init(IMAGE_FLAGS);
    if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS){
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
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
