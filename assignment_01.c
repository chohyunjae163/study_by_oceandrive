/*
* This example code $WHAT_IT_DOES.
*
* This code is public domain. Feel free to use it for any purpose!
*/

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
SDL_Texture * render_target = NULL;
SDL_FRect rect1;
SDL_FRect rect2;
SDL_FRect rect3;

static const int w = 640;
static const int h = 480;
static const int speed = 10;

int p1_x = 0;
int p1_y = 0;
int p1_w = 25;
int p1_h = 100;
int p2_x = 0;
int p2_y = 0;
int p2_w = 25;
int p2_h = 100;
int p3_x = 0;
int p3_y = 0;
int p3_w = 25;
int p3_h = 25;
int x_mov = 1;
int y_mov = 1;
int x_dir = 1;
int y_dir = 1;
int wall_up_x = 5;
int wall_up_y = 5;
int wall_up_w = 620;
int wall_up_h = 25;
int wall_down_x = 5;
int wall_down_y = 480 - 30;
int wall_down_w = 620;
int wall_down_h = 25;

Uint64 last_tick = 0;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	SDL_SetAppMetadata("Example HUMAN READABLE NAME", "1.0", "com.example.CATEGORY-NAME");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("pong2024", w, h, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	render_target = SDL_CreateTexture(renderer, 
																	SDL_PIXELFORMAT_RGBA8888, 
																	SDL_TEXTUREACCESS_STREAMING, 
																	w,h);

	if (!render_target) {
		SDL_Log("Couldn't create texture: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	p1_x = 50;
	p1_y = 200;
	p2_x = 550;
	p2_y = 200;
	p3_x = 320;
	p3_y = 240;

	last_tick = SDL_GetTicks();


	int x = SDL_clamp(SDL_rand(speed),5,9);
	int y = SDL_sqrt(( speed * speed ) - (x * x));
	x_mov = x;
	y_mov = y;

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}

	if (event->type == SDL_EVENT_KEY_DOWN) {
		switch (event->key.scancode) {
			case SDL_SCANCODE_W:
				p1_y += -20;
				break;
			case SDL_SCANCODE_S:
				p1_y += 20;
				break;
			case SDL_SCANCODE_UP:
				p2_y += -20;
        break;
			case SDL_SCANCODE_DOWN:
				p2_y += 20;
				break;
		}
	}

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
	Uint64 cur_tick = SDL_GetTicks();

	//update p3 position every 0.03 sec
	if (cur_tick >= last_tick + 30) {
		bool is_hit = false;

		//check collision with p1,
		const int p1_wall_x = p1_x + p1_w;
		if (p3_x <= p1_wall_x && p3_x > p1_x) {
			if (p3_y < (p1_y + p1_h) && p3_y > p1_y) {
				//is hit!
				// go right
				p3_x = p1_wall_x;
				x_dir = 1;
				is_hit = true;
				SDL_Log("collision p1");
			}
		}
		
		//check collision with p2,
		const int p2_wall_x = p2_x;
		if ((p3_x + p3_w) >= p2_wall_x && p3_x < p2_wall_x) {
			if (p3_y < (p2_y + p2_h) && p3_y > p2_y) {
				// go left
				//is hit!
				p3_x = p2_wall_x - p3_w;
				x_dir = -1;
				is_hit = true;
				SDL_Log("collision p2");
			}
		}

		//check top - bottom collision
		if (p3_y < 30) {
			y_dir = 1;
		}
		else if (p3_y + p3_h > 450) {
			y_dir = -1;
		}

		x_mov += is_hit;
		y_mov += is_hit;
		p3_x += x_mov * x_dir;
		p3_y += y_mov * y_dir;
		last_tick = cur_tick;
	}

	//rendering starts
	/* as you can see from this, rendering draws over whatever was drawn before it. */
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
	SDL_RenderClear(renderer);  /* start with a blank canvas. */

	int *pixels = NULL;
	int pitch = w * 4;
	SDL_LockTexture(render_target,
											 NULL,
											 &pixels, 
											 &pitch // the number of bytes in a row of pixel data, including padding between lines.
											 ); 
	for (int y = 0; y < h; ++y){
		for (int x = 0; x < w; ++x){
			pixels[x + y * w] =  ((x == 320 && ((y / 10) & 1) == 0  ) || y < 30 || y > 450) ? 0xFFFFFFFF : 0x000000FF;
		}
	}

	SDL_UnlockTexture(render_target);

	SDL_RenderTexture(renderer, render_target, NULL, NULL);
	
	//draw rect1 
	{
		rect1.x = p1_x;
		rect1.y = p1_y;
		rect1.w = p1_w;
		rect1.h = p1_h;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer,&rect1);
	}


	//draw rect2
	{
		rect2.x = p2_x;
		rect2.y = p2_y;
		rect2.w = p2_w;
		rect2.h = p2_h;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer,&rect2);
	}

	//draw rect 3
	{
		rect3.x = p3_x;
		rect3.y = p3_y;
		rect3.w = p3_w;
		rect3.h = p3_h;
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer,&rect3);
	}

	
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	/* SDL will clean up the window/renderer for us. */
}
