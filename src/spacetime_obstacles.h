#ifndef _SPACETIME_OBSTACLES_H_
#define _SPACETIME_OBSTACLES_H_

#include "allegro.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//define setup constants
#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 800
#define HEIGHT 800

//define colours
#define BORDER makecol(255,255,255)
#define BACKGROUND makecol(0,0,0)
#define FONT_COL makecol(255,255,0)
#define BLACK makecol(0,0,0)

//define game mechanic factors
#define DIAG_SPEED 4
#define STRT_SPEED 10
#define NUM_ENEMIES 200

//define the sprite structure
typedef struct SPRITE {
	int x,y;
	int width,height;
	int xspeed,yspeed;
	int animdir;
	int wall_side;
	int spawn_delay, spawn_count;
} SPRITE;

//sprite structures
SPRITE *spaceship;
SPRITE *enemies;

//sprite bitmaps
BITMAP *ship_bmp;
BITMAP *enemy_bmp[2];

//screen buffer + background
BITMAP *buff;
BITMAP *bg;

//sound samples
SAMPLE *music;
int music_isPlaying;

//game variables
int game_continue = 1;
int gameover = 0;
int spawn_rate = 120;
volatile int score = 0;

//menu flags
int can_collide = 1;


//function prototypes
void initialize_game();
void setup_screen();
void setup_sounds();
void setup_ship();
void setup_enemies();

void initialize_timer();

void show_introduction();
void show_ending();
void show_help();
void reset_game();
void paused();
void play_sounds();
void toggle_music();

void clear_ship();
void clear_enemies();
void update_ship();
void collision(SPRITE *, SPRITE *);
void update_enemies();
void update_score();
void randomize_spawn();
void draw_ship();
void draw_enemies();
void draw_score();
void draw_screen();

void getInput();
void northwest();
void northeast();
void southwest();
void southeast();
void forward();
void left();
void right();
void backward();
#endif