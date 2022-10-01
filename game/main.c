/* Global code and variables */
#include "global.c"
#include "states/gameplay.c"

/* Show splash screen and await button press (A) */
void splash(){
    SHOW_BKG;
    fadeout();
    SWITCH_ROM_MBC1(3);
    set_bkg_data(0x01, 133, SplashTiles); 
    set_bkg_tiles(0, 0, 20, 18, SplashMap);
    fadein();
    waitpad(J_A);
    fadeout();
}

void animate_menu() {
    if(frame_counter % 5 == 0) {
        set_sprite_tile(0, player_tile + 8);
        set_sprite_tile(1, player_tile);
        set_sprite_prop(0, S_FLIPX);
        set_sprite_prop(1, S_FLIPX);
        set_sprite_prop(2, S_FLIPX);

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

        if (hammer_tile <= 34) {
            set_sprite_tile(3, hammer_tile);
            set_sprite_tile(4, hammer_tile+2);
            hammer_tile += 4;
        }
        else {
            set_sprite_tile(3, 20);
            set_sprite_tile(4, 22);
            hammer_tile = 24;
        }
    }
    if (frame_counter <= 5) {
        frame_counter += 1;
    }
    else {
        frame_counter = 1;
    }
}


void init_menu() {
    SWITCH_ROM_MBC1(3);
    set_bkg_data(0x01, 180, MenuTiles); 
    set_bkg_tiles(0, 0, 20, 18, MenuMap);
    
    // Player
    SPRITES_8x16;
    set_sprite_data(0, 20, Player);
    move_sprite(0, 60, 140);
    move_sprite(1, 60 + 8, 140);
    move_sprite(2, 60 + 6, 140 - 4);
    set_sprite_prop(0, S_FLIPX);
    set_sprite_prop(1, S_FLIPX);
    set_sprite_prop(2, S_FLIPX);

    // Hammer
    set_sprite_data(20, 30, Hammer);
    set_sprite_tile(3, 20);
    set_sprite_tile(4, 22);
    move_sprite(3, 143, 140);
    move_sprite(4, 151, 140);

    SHOW_SPRITES;
    fadein();
}

void scroll_menu_sprites() {
    scroll_sprite(3, 1, 0);
    scroll_sprite(4, 1, 0);
    scroll_sprite(0, 1, 0);
    scroll_sprite(1, 1, 0);
    scroll_sprite(2, 1, 0);
}

/* Starts here */
void main(){
    int z = 0;

    DISPLAY_ON;
    
    splash();

    init_menu();
    
    while(1) {
        while(state == 0) { // 0: Main Menu
            animate_menu();
            scroll_menu_sprites();
            cicled_delay(1);
            switch(joypad()) {
                case J_START:
                    waitpadup();
                    fadeout();
                    init_game();
                    state = 1;
                break;
            }
        }
        while(state == 1){ // 3: Game loop
            cicled_delay(1);
            animate_player();
            process_input();
            scroll_player_sprite();
            timer+=1;
            hazard_trigger+=1;
            if (hazard_trigger > 30) {
                hazard_trigger = 0;
                throw_hazard = generate_random_number(7);
                if (throw_hazard < 4 && can_generate_hazard == 1) {
                    thunder_active = 1;
                }
            } 
            process_hazards();
            show_hazard();
        }
        while(state == 2) { // 2: Game Over
            waitpad(J_A);
            fadeout();
            reset_game();
            init_menu();
            fadein();
            state = 0;
        }
    } 
}
