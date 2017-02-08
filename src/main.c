#include "spacetime_obstacles.h"

int main(void) {

	initialize_game();

	//initialize_timer();

	//Main game loop
	while(game_continue) {
		show_introduction();

		while(!gameover) {

			//delete the ship
			clear_ship();

			//delete the enemies
			clear_enemies();

			//query for keypress, update the info depending on key
			if(keypressed()) {
				getInput();
			}

			update_ship();

			//update the enemies
			update_enemies();

			//draw the ship to buffer
			draw_ship();

			//draw enemies to buffer
			draw_enemies();

			//display the score
			draw_score();

			//draw the buffer to screen
			draw_screen();

			rest(20);
		}

		show_ending();
	}
}
END_OF_MAIN();


void initialize_game() {
	srand(time(NULL));

	allegro_init();
	install_keyboard();
	install_timer();
	initialize_timer();
	setup_sounds();
	play_sounds();
	setup_screen();
	setup_ship();
	setup_enemies();
}

void initialize_timer() {
	LOCK_VARIABLE(score);
	LOCK_FUNCTION(update_score);

	install_int(update_score, 1000);
}

void setup_screen() {	
	int ret;
	set_color_depth(32);
	ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message(allegro_error);
		return;
	}

	//load the background image
	BITMAP *temp = load_bitmap("./assets/The-Iris-Nebula.bmp", NULL);
	bg = create_bitmap(WIDTH-4, HEIGHT-16);
	clear_bitmap(bg);
	blit(temp, bg, temp->w/2 - bg->w/2, temp->h/2 - bg->h/2, 0, 0, bg->w, bg->h);
	destroy_bitmap(temp);

	//create the buffer
	buff = create_bitmap(WIDTH-4, HEIGHT-16);
	clear_bitmap(buff);
	blit(bg, buff, 0, 0, 0, 0, buff->w, buff->h);

	//draw border
	rect(screen, 0, 12, WIDTH-1, HEIGHT-1, BORDER);
	rect(screen, 1, 13, WIDTH-2, HEIGHT-2, BORDER);
	textprintf_ex(screen, font, 1, 1, FONT_COL, -1,
	 "SpaceTime Obstacles");
}

void setup_sounds() {
	if(install_sound(DIGI_AUTODETECT, MIDI_NONE,"") != 0) {
		allegro_message("Error initializing the sound system\n%s\n", allegro_error);
		return;
	}

	//load the wav files
	music = load_sample("./assets/backmusic.wav");
	if(!music) {
		allegro_message("Error loading music file.");
		return;
	}
}

void play_sounds() {
	int panning = 128;
	int pitch = 1000;
	int volume = 128;

	play_sample(music, volume, panning, pitch, TRUE);
	music_isPlaying = 1;
}

void toggle_music() {
		
	if(music_isPlaying) {
		stop_sample(music);
		music_isPlaying = 0;
	} else {
		play_sounds();
	}
	clear_keybuf();
}

void setup_ship() {
	ship_bmp = load_bitmap("./assets/speedship.bmp", NULL);
	
	spaceship = (SPRITE*)malloc(1 * sizeof(SPRITE));
	spaceship->x = WIDTH/2 - ship_bmp->w/2;
	spaceship->y = HEIGHT/2 - ship_bmp->h/2;
	spaceship->xspeed = 0;
	spaceship->yspeed = 0;
}

void setup_enemies() {
	enemy_bmp[0] = load_bitmap("./assets/enemy_vert.bmp", NULL);
	enemy_bmp[1] = load_bitmap("./assets/enemy_horz.bmp", NULL);

	enemies = (SPRITE*)malloc(NUM_ENEMIES * sizeof(SPRITE));
	int i;
	for(i = 0; i < NUM_ENEMIES; i++) {
		enemies[i].width = enemy_bmp[0]->w;
		enemies[i].height = enemy_bmp[0]->h;
		enemies[i].xspeed = rand()%7+1;
		enemies[i].yspeed = rand()%7+1;
		randomize_spawn(&enemies[i]);
	}
}

void show_introduction() {
	int low_h = HEIGHT/2 - 100;
	int high_h = HEIGHT/2 + 69;
	int low_w = WIDTH/2 - 300;
	int high_w = WIDTH/2 + 299;

	rectfill(buff, low_w, low_h, high_w,high_h, BLACK);
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 5, FONT_COL, -1,
		"SPACETIME OBSTACLES");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 20, FONT_COL, -1,
		"Avoid the oncoming enemies!");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 35, FONT_COL, -1,
		"But beware - they speed up over time.");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 60, FONT_COL, -1,
		"Use the arrow keys or W-A-S-D keys to move the ship.");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 75, FONT_COL, -1,
		"Press CTRL+H for help in game.");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 135, FONT_COL, -1, 
		"Survive for as long as possible,");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 150, FONT_COL, -1, 
		"to let the rest of your fleet escape!");
	draw_screen();

	//wait for the user
	while(!keypressed()){};

	//restore the screen
	blit(bg, buff, low_w, low_h, low_w, low_h, 600, 200);
	draw_screen();
}

void show_ending() {
	clear_keybuf();
	int low_h = HEIGHT/2 - 100;
	int high_h = HEIGHT/2 + 69;
	int low_w = WIDTH/2 - 300;
	int high_w = WIDTH/2 + 299;

	rectfill(buff, low_w, low_h, high_w,high_h, BLACK);
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 5, FONT_COL, -1,
		"SPACETIME OBSTACLES");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 20, FONT_COL, -1,
		"You've died!");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 35, FONT_COL, -1,
		"Your final score is: ");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 60, FONT_COL, -1,
		"%d", score);
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 75, FONT_COL, -1,
		"You survived for %d seconds.", score/100);
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 150, FONT_COL, -1, 
		"Hit Y to play again, or ESC to quit.");
	draw_screen();

	//wait for the user
	while(!key[KEY_ESC] || !key[KEY_Y]) {
		if(key[KEY_Y]) {
			gameover = 0;
			reset_game();
			break;
		}
		if(key[KEY_ESC]) {
			game_continue = 0;
			break;
		}
	};

	//restore the screen
	blit(bg, buff, low_w, low_h, low_w, low_h, 600, 200);
	draw_screen();
}

void show_help() {
	clear_keybuf();
	int low_h = HEIGHT/2 - 100;
	int high_h = HEIGHT/2 + 69;
	int low_w = WIDTH/2 - 300;
	int high_w = WIDTH/2 + 299;

	rectfill(buff, low_w, low_h, high_w,high_h, BLACK);
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 5, FONT_COL, -1,
		"SPACETIME OBSTACLES");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 20, FONT_COL, -1,
		"Use the arrow keys or W-A-S-D keys to move the ship.");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 35, FONT_COL, -1,
		"CTRL+H will show the help screen (this screen).");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 50, FONT_COL, -1,
		"CTRL+M will toggle the music.");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 75, FONT_COL, -1,
		"The goal is to survive for as long as you can.");
	textprintf_centre_ex(buff, font, WIDTH / 2, low_h + 150, FONT_COL, -1, 
		"Good luck!");
	draw_screen();

	//wait for the user
	while(!keypressed()){};

	if(key[KEY_ESC]) {
		exit(1);
	}
	//restore the screen
	blit(bg, buff, low_w, low_h, low_w, low_h, 600, 200);
	draw_screen();
}

void reset_game() {
	blit(bg, buff, 0, 0, 0, 0, buff->w, buff->h);

	//draw border
	rect(screen, 0, 12, WIDTH-1, HEIGHT-1, BORDER);
	rect(screen, 1, 13, WIDTH-2, HEIGHT-2, BORDER);
	textprintf_ex(screen, font, 1, 1, FONT_COL, -1,
	 "SpaceTime Obstacles");

	setup_ship();
	setup_enemies();
	score = 0;
}

void paused() {
	clear_keybuf();
	int p = 1;
	while(p) {
		if(keypressed()) {
			if(key[KEY_P]) {
				p = 0;
			}
			if(key[KEY_ESC]) {
				exit(1);
			}
		}
	}
	clear_keybuf();
}

void clear_ship() {	 
	
	blit(bg, buff, spaceship->x, spaceship->y, spaceship->x, spaceship->y, 
		ship_bmp->w, ship_bmp->h);
	
	/*rectfill(buff, spaceship->x, spaceship->y, 
		spaceship->x + ship_bmp->w, spaceship->y + ship_bmp->h, BACKGROUND);*/
}

void clear_enemies() {
	int i;
	for(i = 0; i < NUM_ENEMIES; i++){
		blit(bg, buff, enemies[i].x, enemies[i].y, enemies[i].x, enemies[i].y, 
			enemy_bmp[0]->w, enemy_bmp[0]->h);
		/*rectfill(buff, enemies[i].x, enemies[i].y,
			enemies[i].x + enemy_bmp->w, enemies[i].y + enemy_bmp->h, BACKGROUND);*/
	}
}

void update_ship() {

	spaceship->x += spaceship->xspeed;
	if(spaceship->x < 0) {
		spaceship->x = 0;
	} else
	if(spaceship->x + ship_bmp->w > WIDTH) {
		spaceship->x = WIDTH - ship_bmp->w;
	}

	spaceship->y += spaceship->yspeed;
	if(spaceship->y < 14) {
		spaceship->y = 14;
	} else
	if(spaceship->y + ship_bmp->h > WIDTH) {
		spaceship->y = WIDTH - ship_bmp->h;
	}

	//Reset speed marking for next game cycle
	spaceship->xspeed = 0;
	spaceship->yspeed = 0;
}

void update_enemies() {
	int i;

	for(i = 0; i < NUM_ENEMIES; i++) {

		if(enemies[i].spawn_count > enemies[i].spawn_delay) {

			switch(enemies[i].wall_side) {
				case 0:
					enemies[i].x += enemies[i].xspeed;
					if(enemies[i].x > WIDTH + enemy_bmp[0]->w) {
						randomize_spawn(&enemies[i]);
						enemies[i].xspeed += 1;
					}
					break;
				case 1:
					enemies[i].y += enemies[i].yspeed;
					if(enemies[i].y > HEIGHT + enemy_bmp[0]->h) {
						randomize_spawn(&enemies[i]);
						enemies[i].yspeed += 1;
					}
					break;
				case 2:
					enemies[i].x -= enemies[i].xspeed;
					if(enemies[i].x < 0 - enemy_bmp[0]->w) {
						randomize_spawn(&enemies[i]);
						enemies[i].xspeed += 1;
					}
					break;
				case 3:
					enemies[i].y -= enemies[i].yspeed;
					if(enemies[i].y < 0 - enemy_bmp[0]->h) {
						randomize_spawn(&enemies[i]);
						enemies[i].yspeed += 1;
					}
					break;
			}
		}
		if(can_collide) {
			collision(spaceship, &enemies[i]);
		}
	}

}

void update_score() {
	score += 100;
}
END_OF_FUNCTION(update_score)	

void collision(SPRITE *player, SPRITE *B) {
	int players_left = player->x - 10;
	int players_right = player->x + 10;
	int players_top = player->y - 10;
	int players_bot = player->y + 10;
	//printf("X:%d Y:%d L:%d R:%d T:%d B:%d\n", player->x, player->y, players_left, players_right, players_top, players_bot);

	if( (players_left > B->x - B->width) && (players_right < B->x + B->width) &&
		(players_top > B->y - B->height) && (players_bot < B->y + B->height)) {
			gameover = 1;
		}	
}

void randomize_spawn(SPRITE *enemy) {
	enemy->wall_side = rand()%4;
	enemy->spawn_delay = rand()%spawn_rate;
	enemy->spawn_count = 0;

	switch(enemy->wall_side) {
			case 0:
				enemy->x = 0-enemy->width;
				enemy->y = rand()%(HEIGHT - 2*enemy_bmp[0]->h) + enemy_bmp[0]->h;
				enemy->animdir = 0;
				break;
			case 1:
				enemy->x = rand()%(WIDTH - 2*enemy_bmp[0]->w) + enemy_bmp[0]->w;
				enemy->y = 0-enemy->height;
				enemy->animdir = 1;
				break;
			case 2:
				enemy->x = WIDTH;
				enemy->y = rand()%(HEIGHT - 2*enemy_bmp[0]->h) + enemy_bmp[0]->h;
				enemy->animdir = 2;
				break;
			case 3:
				enemy->x = rand()%(WIDTH - 2*enemy_bmp[0]->w) + enemy_bmp[0]->w;
				enemy->y = HEIGHT;
				enemy->animdir = 3;
				break;
		}
}

void draw_ship() {
	int x = spaceship->x;
	int y = spaceship->y;
	draw_sprite(buff, ship_bmp, x, y);
}

void draw_enemies() {
	int i;
	for(i = 0; i < NUM_ENEMIES; i++) {
		int x = enemies[i].x;
		int y = enemies[i].y;
		int wall = enemies[i].wall_side;

		if(enemies[i].spawn_count++ > enemies[i].spawn_delay) {
			//draw_sprite(buff, enemy_bmp[0], x, y);
			
			if(wall == 1) { //Top
				draw_sprite(buff, enemy_bmp[0], x, y);
			} else
			if(wall == 3) { //Bot
				draw_sprite_v_flip(buff, enemy_bmp[0], x, y);
			} else
			if(wall == 0) { //Left
				draw_sprite(buff, enemy_bmp[1], x, y);
			}
			else {			//Right
				draw_sprite_h_flip(buff, enemy_bmp[1], x, y);
			}
		}
	}
}

void draw_score() {
	//clear_bitmap(buff);
	rectfill(buff, buff->w - 100, 1, buff->w, 1+text_height(font), BACKGROUND);
	textprintf_right_ex(buff, font, buff->w, 1, FONT_COL, -1,
	"Score: %d", score); 
}

void draw_screen() {
	blit(buff, screen, 0, 0, 2, 14, buff->w, buff->h);
}

void getInput() {
	//hit ESC to quit
    if (key[KEY_ESC])   gameover = 1;

    //check for flag commands
    if (key[KEY_LCONTROL] && key[KEY_C]) {
    	can_collide = (can_collide + 1) % 2;
    	clear_keybuf();
    } 

    //toggle the music
    if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && key[KEY_M]) {
    	toggle_music();
    	clear_keybuf();
    } 

    //toggle the music
    if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && key[KEY_H]) {
    	show_help();
    	clear_keybuf();
    }
    
    if (key[KEY_P]) {
    	paused();
    }

    //WASD or Arrows
    if ((key[KEY_W] && key[KEY_A]) || (key[KEY_UP] && key[KEY_LEFT])) 		northwest();
    if ((key[KEY_W] && key[KEY_D]) || (key[KEY_UP] && key[KEY_RIGHT])) 		northeast();
    if ((key[KEY_S] && key[KEY_A]) || (key[KEY_DOWN] && key[KEY_LEFT]))  	southwest();
    if ((key[KEY_S] && key[KEY_D]) || (key[KEY_DOWN] && key[KEY_RIGHT])) 	southeast();

    if (key[KEY_W] || key[KEY_UP])     		forward();
    if (key[KEY_D] || key[KEY_RIGHT])     	right();
    if (key[KEY_A] || key[KEY_LEFT])     	left();
    if (key[KEY_S] || key[KEY_DOWN])     	backward();

   // rest(20);
}

void forward() {
	//spaceship->y -= spaceship->yspeed;
	spaceship->yspeed = -1*STRT_SPEED;
}

void left() {
	//spaceship->x -= spaceship->xspeed;
	spaceship->xspeed = -1*STRT_SPEED;
}

void right() {
	//spaceship->x += spaceship->xspeed;
	spaceship->xspeed = STRT_SPEED;
}

void backward() {
	//spaceship->y += spaceship->yspeed;
	spaceship->yspeed = STRT_SPEED;
}

void northwest() {
	spaceship->yspeed = -1*DIAG_SPEED;
	spaceship->xspeed = -1*DIAG_SPEED;
}

void northeast() {
	spaceship->yspeed = -1*DIAG_SPEED;
	spaceship->xspeed = DIAG_SPEED;
}

void southwest() {
	spaceship->yspeed = DIAG_SPEED;
	spaceship->xspeed = -1*DIAG_SPEED;
}

void southeast() {
	spaceship->yspeed = DIAG_SPEED;
	spaceship->xspeed = DIAG_SPEED;
}