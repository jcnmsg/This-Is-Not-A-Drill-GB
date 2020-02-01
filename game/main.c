/* Global code and variables */
#include "global.c"

/* Graphics */
#include "background/splash.c"
#include "background/bg.c"
#include "background/bg_tiles.c"

#include "sprites/bbit.c"
#include "sprites/hammer.c"

UINT8 player_tile = 0;
unsigned int frame_counter = 0;
int player_x = 80;
int player_y = 141;
UINT8 player_direction = 0; // 0 left 1 right 2 up 3 down

const unsigned int *positions[13][4] = { 
    { 1, 0, 0, 0 }, // 0
    { 1, 1, 1, 0 }, // 1
    { 1, 0, 0, 0 }, // 2
    { 0, 0, 1, 0 }, // 3
    { 1, 0, 1, 1 }, // 4
    { 1, 1, 0, 0 }, // 5
    { 0, 0, 0, 1 }, // 6
    { 0, 1, 1, 0 }, // 7
    { 1, 0, 0, 1 }, // 8
    { 0, 0, 1, 1 }, // 9
    { 1, 0, 1, 1 }, // 10
    { 1, 0, 1, 0 }, // 11
    { 1, 0, 0, 1 }, // 12
};

/* Show splash screen and await button press (A) */
void splash(){
    SHOW_BKG;
    fadeout();
    set_bkg_data(0x01, splash_tiles, splash_dat); 
    set_bkg_tiles(0, 0, splash_cols, splash_rows, splash_map);
    fadein();
    waitpad(J_A);
    fadeout();
}

void process_player_position(){

}

void process_input() {
    switch(joypad()) {
		case J_RIGHT: 
            if (player_x < 149) {
                player_direction = 1;
                scroll_sprite(0, 1, 0);
                scroll_sprite(1, 1, 0);
                move_sprite(2, player_x + 6, player_y - 4);
                player_x += 1;
            }
			break;
		case J_LEFT: 
            if (player_x > 12) {
                player_direction = 0;
                scroll_sprite(0, -1, 0);
                scroll_sprite(1, -1, 0);
                move_sprite(2, player_x + 3, player_y - 4);
                player_x -= 1;
            }
        	break;
        case J_UP:
            if (player_y > 24) {
                player_direction = 2;
                scroll_sprite(0, 0, -1);
                scroll_sprite(1, 0, -1);
                move_sprite(2, player_x + 3, player_y - 4);
                player_y -= 1;
            }
            break;

        case J_DOWN: 
            if (player_y < 141) {
                player_direction = 3;
                scroll_sprite(0, 0, 1);
                scroll_sprite(1, 0, 1);
                move_sprite(2, player_x + 3, player_y - 4);
                player_y += 1;
            }
        default: 
            break;
    }
    process_player_position();
}

void animate_player() {
    if (frame_counter % 5 == 0) {
        if (player_direction == 0) {
            set_sprite_tile(0, player_tile);
            set_sprite_tile(1, player_tile + 8);
            move_sprite(2, player_x + 3, player_y - 4);
            set_sprite_prop(0, get_sprite_prop(0) & ~S_FLIPX);
            set_sprite_prop(1, get_sprite_prop(1) & ~S_FLIPX);
            set_sprite_prop(2, get_sprite_prop(2) & ~S_FLIPX);
        }
        if (player_direction == 1) {
            set_sprite_tile(1, player_tile);
            set_sprite_tile(0, player_tile + 8);
            move_sprite(2, player_x + 6, player_y - 4);
            set_sprite_prop(0, S_FLIPX);
            set_sprite_prop(1, S_FLIPX);
            set_sprite_prop(2, S_FLIPX);
        }

        if (player_tile == 0 || player_tile == 4) {
            set_sprite_tile(2, 18);
        }
        else {
            set_sprite_tile(2, 16);
        }

        if (player_tile <= 4) {
            player_tile +=2;
        }
        else {
            player_tile = 0;
        }
    }
    if (frame_counter <= 5) {
        frame_counter += 1;
    }
    else {
        frame_counter = 1;
    }
}

void init(){
    set_bkg_data(0, 14, BgTiles); 
    set_bkg_tiles(0, 0, 32, 32, Bg);
    SPRITES_8x16; // Activate 8*16 sprite mode, defaults to 8x8
    set_sprite_data(0, 20, Player);
    set_sprite_data(20, 8, Hammer);
    move_sprite(0, player_x, player_y);
    move_sprite(1, player_x + 8, player_y);
    set_sprite_tile(3, 20);
    set_sprite_tile(4, 22);
    move_sprite(3, 146, 136);
    move_sprite(4, 154, 136);
    SHOW_SPRITES;
    fadein();
}

/* Starts here */
void main(){
    DISPLAY_ON;
    
    splash();

    init();
    
    while(1) {
        while(state == 0) { // 0: Main Menu
            cicled_delay(1);
            animate_player();
            process_input();
        }
    }
}
