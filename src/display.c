#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;


bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    window_width = display_mode.w;    
    window_height = display_mode.h;

    window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width,
            window_height,
            SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void destroy_window(){
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("finished destroying resources\n");
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color){
    for (int dx = x; dx < (x + width); dx++) {
        for(int dy = y; dy < (y + height); dy++) {
            color_buffer[(window_width * dy) + dx] = color;
        }
    }
}

void draw_grid(void) {
    for (int y = 0; y < window_height; y+=10) {
        for (int x = 0; x < window_width; x+=10) {
            color_buffer[(window_width * y) + x] = 0xFF333333;
        }
    }
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
            color_buffer_texture,
            NULL,
            color_buffer,
            (int)(window_width * sizeof(uint32_t))
            );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}


void clear_color_buffer(uint32_t color) {
    for (int i = 0, buffer_size = window_height * window_width; i < buffer_size; i++){
        *(color_buffer+i) = color;
    }
}

