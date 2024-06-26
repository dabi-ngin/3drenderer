#include "display.h"

// SDL window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// buffers
SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;

// window vars
int window_width = 800;
int window_height = 600;

// initialize_window: generates SDL_window values based on currently set `window_width` and `window_height`
// returning a bool which we use to validate success of process. True=success, false=failure.
// Also automatically sets window to fullscreen by default.
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

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}


// destroy_window: frees memory consumed by color_buffer, renderer and window.
void destroy_window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// draw_pixel: takes x, y as cartesian coordinates in a 2D plane to populate color_buffer entry 
// as value of arg `color`.
void draw_pixel(int x, int y, uint32_t color) {
    if(x >=0 && x < window_width && y >= 0 && y < window_height) {
        color_buffer[(window_width * y) + x] = color;
    }
}

// draw_rectangle: takes arguments `x` and `y` as integers for offset from top-left of screen (0 point in buffer)
// and draws rectangle from left-to-right and bottom-to-top populating each entry in color_buffer with value of
// argument `color`.
void draw_rectangle(int x, int y, int width, int height, uint32_t color){
    for (int i = 0; i <  width; i++) {
        for(int j = 0; j <  height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

// draw_grid: steps 10 pixels across x and y in our color_buffer and assigns this a uint32_t value of 0xFF333333.
void draw_grid(void) {
    for (int y = 0; y < window_height; y+=10) {
        for (int x = 0; x < window_width; x+=10) {
            draw_pixel(x, y, 0xFF333333);
        }
    }
}

// render_color_buffer: calles `SDL_UpdateTexture` and `SDL_RenderCopy` to pass our color_buffer 
// and color_buffer_texture variables to SDL libs which render it in a window.
void render_color_buffer(void) {
    SDL_UpdateTexture(
            color_buffer_texture,
            NULL,
            color_buffer,
            (int)(window_width * sizeof(uint32_t))
            );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

// clear_color_buffer: takes uint32_t `color` as arg and sets each entry in color_buffer array to `color`.
void clear_color_buffer(uint32_t color) {
    for (int i = 0, buffer_size = window_height * window_width; i < buffer_size; i++){
        *(color_buffer+i) = color;
    }
}


void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    // Find out how much we should increment in both x and y each step
    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_y = y0;
    float current_x = x0;

    for (int i = 0; i < side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}
