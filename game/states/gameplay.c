/* Banked Graphics */
extern const unsigned char *BgTiles[];
extern const unsigned char *Hammer[];
extern const unsigned char *Bg[];
extern const unsigned char *Player[];
extern const unsigned char *SplashTiles[];
extern const unsigned char *SplashMap[];
extern const unsigned char *Breaks[];
extern const unsigned char *MenuTiles[];
extern const unsigned char *MenuMap[];
extern const unsigned char *GameOverTiles[];
extern const unsigned char *GameOverMap[];
extern const unsigned char *OpenLedge[];
extern const unsigned char *PauseTiles[];
extern const unsigned char *PausedBackground[];
extern const unsigned char *HammerScoreMap[];
extern const unsigned char *NoPauseBackground[];

int *movements[12][4] = { 
    { 0, 0, 1, 0 }, // 0
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
    { 1, 0, 0, 1 } // 11
};

int *positions[12][2] = {
    { 20, 128 },
    { 70 , 128 },
    { 140, 128 },
    { 20, 96 },
    { 70, 96 },
    { 140, 96 },
    { 20, 64 },
    { 66, 64 },
    { 140, 64 },
    { 20, 32 },
    { 66, 32 },
    { 140, 32 }
};

int *hazard_positions[12][2] = {
    {17, 124}, // 0
    {90, 124}, // 1
    {0, 0}, // 2
    {17, 84}, // 3
    {70, 84}, // 4
    {123, 84}, // 5
    {17, 55}, // 6
    {70, 55}, // 7
    {123, 55}, // 8
    {17, 25}, // 9
    {78, 25},  // 10
    {139, 25} // 11
};

int *score_positions[22][2] = {
    {30, 30},
    {0, 17},
    {1, 17},
    {2, 17},
    {3, 17},
    {4, 17},
    {5, 17},
    {6, 17},
    {7, 17},
    {8, 17},
    {9, 17},
    {10, 17},
    {11, 17},
    {12, 17},
    {13, 17},
    {14, 17},
    {15, 17},
    {16, 17},
    {17, 17},
    {18, 17},
    {19, 17},
    {20, 17}
};

UINT8 offset_x = 0;
UINT8 can_process = 0;
UINT8 hammer_tile = 20;
UINT8 player_tile = 0;
UINT8 player_position = 0;
UINT8 requires_scrolling_x = 0;
UINT8 requires_scrolling_y = 0;
UINT8 has_reached = 1;
UINT8 has_hammer = 0;
UINT8 paused = 0;
UINT8 previous_direction = 0;
unsigned int frame_counter = 0;
int player_x_diff = 0;
int player_y_diff = 0;
int player_direction = 2; // 0 left 1 up 2 right 3 down
int timer = 0;
int player_x = positions[0][0];
int player_y = positions[0][1];
int player_new_x = player_x;
int player_new_y = player_y;
int throw_hazard = 0;
int thunder_active = 0;
int hazard_trigger = 0;
int hazard_diff = 0;
int hazard_room = -1;
int can_generate_hazard = 1;
int hazard_timer = 0;
int score = 0;

void reset_game(){
    int i = 0;
    offset_x = 0;
    can_process = 0;
    hammer_tile = 20;
    player_tile = 0;
    player_position = 0;
    requires_scrolling_x = 0;
    requires_scrolling_y = 0;
    has_reached = 1;
    has_hammer = 0;
    frame_counter = 0;
    player_x_diff = 0;
    player_y_diff = 0;
    player_direction = 2; // 0 left 1 up 2 right 3 down
    timer = 0;
    player_x = positions[0][0];
    player_y = positions[0][1];
    player_new_x = player_x;
    player_new_y = player_y;
    throw_hazard = 0;
    thunder_active = 0;
    hazard_trigger = 0;
    hazard_room = -1;
    can_generate_hazard = 1;
    hazard_timer = 0;
    score = 0;

    for (i = 0; i < 40; i++) {
        move_sprite(i, 255, 255);
    }
}

void process_hazards() {
    if (thunder_active == 1)  {
        thunder();

        if (thunder_counter > 21) {
            thunder_counter = 0;
            thunder_active = 0;

            if (can_generate_hazard == 1) {
                hazard_room = generate_random_number(11);
                if (hazard_room != 2) {
                    can_generate_hazard = 0;
                    play_sound(1);
                    hazard_diff+=20;
                }
            }
        }
    }
}

void show_hazard() {
    int hazard_x, hazard_y, z;
    int total = 400 - hazard_diff;
    if (can_generate_hazard == 0) { // means it's already going
        hazard_x = hazard_positions[hazard_room][0];
        hazard_y = hazard_positions[hazard_room][1];

        move_sprite(5, hazard_x, hazard_y);
        move_sprite(6, hazard_x + 8, hazard_y);

        hazard_timer+=1;
        if (hazard_timer % 10 == 0) {
            set_sprite_prop(5, 0x10U);
            set_sprite_prop(6, 0x10U);
        }
        if (hazard_timer % 10 != 0) {
            set_sprite_prop(5, 0x00);
            set_sprite_prop(6, 0x00); 
        }

        if (hazard_timer > total/2) {
            set_sprite_tile(5, 54);
            set_sprite_tile(6, 56);
            play_sound(0);
        }

        if (hazard_timer == total) {
             for(z = 0; z < 9; z++) {
                if (z >= 0 && z < 2) {
                    BGP_REG = 0xE4; 
                    OBP0_REG = 0xE4; 
                }
                if (z > 3 && z < 5) {
                    BGP_REG = 0x90;
                    OBP0_REG = 0x90;
                }
                if (z > 6 && z < 8) {
                    BGP_REG = 0x80;
                    OBP0_REG = 0x80;
                    play_sound(2);
                }
                cicled_delay(1);
            }
            HIDE_SPRITES;
            for (z = 129; z <= 150; z++) {
                set_bkg_data(z, z, 0xff); 
            }
            
            fadeout();

            // init game over and fadein
            SWITCH_ROM_MBC1(3);
            set_bkg_data(0x01, 53, GameOverTiles); 
            set_bkg_tiles(0, 0, 20, 18, GameOverMap);

            fadein();

            state = 2;
        }
    }
}

void reset_hammer() {
    int i;
    set_sprite_tile(3, 20);
    set_sprite_tile(4, 22);
    play_sound(0);
    for(i = 0; i < 30; i+=1) {
        scroll_sprite(3, 0, -1);
        scroll_sprite(4, 0, -1);
        cicled_delay(1);
    }
    has_hammer = 0;
    hammer_tile = 20;
    set_sprite_tile(3, 20);
    set_sprite_tile(4, 22);
    move_sprite(3, 143, 122);
    move_sprite(4, 151, 122);
}

void reset_hazard() {
    can_generate_hazard = 1;
    hazard_timer = 0;
    hazard_room = -1;
    thunder_active = 0;
    move_sprite(5, 256, 256);
    move_sprite(6, 256, 256);
    set_sprite_tile(5, 50);
    set_sprite_tile(6, 52);
}

void pickup_hammer(){
    play_sound(0);
    has_hammer = 1;
}

void check_hammer_status(){
    if (player_position == 2) { 
        if (has_hammer == 0) {
            pickup_hammer();
        }
    }
}

void open_ledge() {
    SWITCH_ROM_MBC1(3);
    if (player_position == 6) {
        set_bkg_tiles(1, 4, 2, 1, OpenLedge);
    }
    if (player_position == 3) {
        set_bkg_tiles(1, 8, 2, 1, OpenLedge);
    }
    if (player_position == 8) {
        set_bkg_tiles(17, 4, 2, 1, OpenLedge);
    }
}

void close_ledge() {
    SWITCH_ROM_MBC1(3);
    set_bkg_tiles(0, 0, 20, 17, Bg);
}

void requires_offset() {
    if (player_direction == 0 && (player_position == 1 || player_position == 4)) {
        scroll_sprite(0, +4, 0);
        scroll_sprite(1, +4, 0);
        scroll_sprite(2, +4, 0);
        if (has_hammer) {
            scroll_sprite(3, +4, 0);
            scroll_sprite(4, +4, 0);
        }
        player_x += 4;
    }
    if (player_direction == 2 && (player_position == 5 || player_position == 8 || player_position == 11)) {
        scroll_sprite(0, +2, 0);
        scroll_sprite(1, +2, 0);
        scroll_sprite(2, +2, 0);
        if (has_hammer) {
            scroll_sprite(3, +2, 0);
            scroll_sprite(4, +2, 0);
        }
        player_x += 2;
    }
    if (player_direction == 2 && player_position == 10) {
        scroll_sprite(0, -4, 0);
        scroll_sprite(1, -4, 0);
        scroll_sprite(2, -4, 0);
        if (has_hammer) {
            scroll_sprite(3, -4, 0);
            scroll_sprite(4, -4, 0);
        }
        player_x -= 4;
    }
    if (player_direction == 0 && (player_position == 9 || player_position == 6)) {
        scroll_sprite(0, -2, 0);
        scroll_sprite(1, -2, 0);
        scroll_sprite(2, -2, 0);
        if (has_hammer) {
            scroll_sprite(3, -2, 0);
            scroll_sprite(4, -2, 0);
        }
        player_x -= 2;
    }
    if (player_direction == 2 && (player_position == 9 || player_position == 6)) {
        scroll_sprite(0, -4, 0);
        scroll_sprite(1, -4, 0);
        scroll_sprite(2, -4, 0);
        if (has_hammer) {
            scroll_sprite(3, -4, 0);
            scroll_sprite(4, -4, 0);
        }
        player_x -= 4;
    }
    requires_scrolling_x = 0;
    has_reached = 1;
}

void scroll_player_sprite() {
    if (requires_scrolling_x == 1){
        if (player_x > player_new_x) {
            scroll_sprite(0, -2, 0);
            scroll_sprite(1, -2, 0);
            scroll_sprite(2, -2, 0);
            if (has_hammer) {
                scroll_sprite(3, -2, 0);
                scroll_sprite(4, -2, 0);
            }
            player_x -=2;
        }
        if (player_x < player_new_x) {
            scroll_sprite(0, 2, 0);
            scroll_sprite(1, 2, 0);
            scroll_sprite(2, 2, 0);
            if (has_hammer) {
                scroll_sprite(3, 2, 0);
                scroll_sprite(4, 2, 0);
            }
            player_x +=2;
        }
        if (player_x == player_new_x) {
            requires_offset();
            check_hammer_status();
        }
    } 
    else if (requires_scrolling_y == 1) {
        if (player_y > player_new_y) {
            scroll_sprite(0, 0, -2);
            scroll_sprite(1, 0, -2);
            scroll_sprite(2, 0, -2);
            if (has_hammer) {
                scroll_sprite(3, 0, -2);
                scroll_sprite(4, 0, -2);
            }
            player_y -=2;
        }
        if (player_y < player_new_y) {
            open_ledge(player_x, player_y);
            scroll_sprite(0, 0, 2);
            scroll_sprite(1, 0, 2);
            scroll_sprite(2, 0, 2);
            if (has_hammer) {
                scroll_sprite(3, 0, 2);
                scroll_sprite(4, 0, 2);
            }
            player_y +=2;
        }
        if (player_y == player_new_y) {
            requires_scrolling_y = 0;
            has_reached = 1;
            close_ledge();
        }
    }
}

void process_player_position(){
    int can_move = movements[player_position][player_direction];
    if (can_move == 1) {
        if (player_direction == 2) {
            player_position += 1; 
        }
        if (player_direction == 0) {
            player_position -= 1;
        }
        if (player_direction == 1) {
            player_position += 3;
        }
        if (player_direction == 3) {
            player_position -= 3;
        }

        player_new_x = positions[player_position][0];
        player_new_y = positions[player_position][1];
    }
}

void process_input() {
    int z;
    switch(joypad()) {
        case J_START: 
            if (paused == 1) {
                SWITCH_ROM_MBC1(3);
                set_bkg_tiles(13, 17, 7, 1, NoPauseBackground);
                for (z = 1; z <= score; z++) {
                    set_bkg_tiles(score_positions[z][0], score_positions[z][1], 1, 1, HammerScoreMap);
                }
                waitpadup();
                paused = 0;
            }
            else if (paused == 0) {
                SWITCH_ROM_MBC1(3);
                set_bkg_data(26, 7, PauseTiles); 
                set_bkg_tiles(13, 17, 7, 1, PausedBackground);
                waitpadup();
                waitpad(J_START);
                paused = 1;
            }
            break;
        case J_RIGHT:
            if (has_reached == 1) {
                has_reached = 0;
                previous_direction = 2;
                player_direction = 2;
                requires_scrolling_x = 1;
                offset_x = 2;
                process_player_position();
            }
            break;
        
        case J_LEFT:
            if (has_reached == 1) {
                has_reached = 0;
                player_direction = 0;
                previous_direction = 0;
                requires_scrolling_x = 1;
                offset_x = 0;
                process_player_position();
            }
            break;

        case J_UP:
            if (has_reached == 1) {
                has_reached = 0;
                player_direction = 1;
                requires_scrolling_y = 1;
                process_player_position();
            }
            break;

        case J_DOWN:
             if (has_reached == 1) {
                has_reached = 0;
                player_direction = 3;
                requires_scrolling_y = 1;
                process_player_position();
            }
            break;

        case J_A: 
            if (has_hammer == 1) {
                if (player_position == hazard_room && hazard_room != 2) {
                    play_sound(0);
                    reset_hazard();
                    reset_hammer();
                    score += 1;
                    SWITCH_ROM_MBC1(3);
                    set_bkg_tiles(score_positions[score][0], score_positions[score][1], 1, 1, HammerScoreMap);
                }
            }
    }
}

void animate_player() {
    if (frame_counter % 5 == 0) {
        if (player_direction == 0 || previous_direction == 0) {
            set_sprite_tile(0, player_tile);
            set_sprite_tile(1, player_tile + 8);
            set_sprite_prop(0, get_sprite_prop(0) & ~S_FLIPX);
            set_sprite_prop(1, get_sprite_prop(1) & ~S_FLIPX);
            set_sprite_prop(2, get_sprite_prop(2) & ~S_FLIPX);
            move_sprite(2, player_x + 3, player_y-4);
        }
        if (player_direction == 2 || previous_direction == 2) {
            set_sprite_tile(1, player_tile);
            set_sprite_tile(0, player_tile + 8);
            set_sprite_prop(0, S_FLIPX);
            set_sprite_prop(1, S_FLIPX);
            set_sprite_prop(2, S_FLIPX);
            move_sprite(2, player_x + 6, player_y - 4);
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

        if (has_hammer == 1 && previous_direction == 0) {
            move_sprite(3, player_x+4, player_y-4);
            move_sprite(4, player_x+12, player_y-4); 
        }
        else if (has_hammer == 1 && previous_direction == 2) {
            move_sprite(3, player_x - 4, player_y - 4);
            move_sprite(4, player_x + 4, player_y - 4); 
        }
    }
    if (frame_counter <= 5) {
        frame_counter += 1;
    }
    else {
        frame_counter = 1;
    }
}

void init_game(){
    // Background
    SWITCH_ROM_MBC1(3);
    set_bkg_data(0, 26, BgTiles); 
    set_bkg_tiles(0, 0, 20, 18, Bg);
    SPRITES_8x16;
    
    // Player
    set_sprite_data(0, 20, Player);
    move_sprite(0, player_x, player_y);
    move_sprite(1, player_x + 8, player_y);
    move_sprite(2, player_x + 6, player_y - 4);
    set_sprite_prop(0, S_FLIPX);
    set_sprite_prop(1, S_FLIPX);
    set_sprite_prop(2, S_FLIPX);

    // Hammer
    set_sprite_data(20, 30, Hammer);
    set_sprite_tile(3, 20);
    set_sprite_tile(4, 22);
    move_sprite(3, 143, 122);
    move_sprite(4, 151, 122);

    // Breaks
    set_sprite_data(50, 8, Breaks);
    set_sprite_tile(5, 50);
    set_sprite_tile(6, 52);
   
    // Show
    SHOW_SPRITES;
    fadein();
    init_randomizer();
}