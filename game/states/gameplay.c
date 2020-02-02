/* Banked Graphics */
extern const unsigned char *BgTiles[];
extern const unsigned char *Hammer[];
extern const unsigned char *Bg[];
extern const unsigned char *Player[];
extern const unsigned char *SplashTiles[];
extern const unsigned char *SplashMap[];


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
    { 20, 141 },
    { 70 , 141 },
    { 140, 141 },
    { 20, 105 },
    { 70, 105 },
    { 140, 105 },
    { 20, 65 },
    { 66, 65 },
    { 140, 65 },
    { 20, 33 },
    { 66, 33 },
    { 140, 33 }
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
unsigned int frame_counter = 0;
int player_x_diff = 0;
int player_y_diff = 0;
int player_direction = 2; // 0 left 1 up 2 right 3 down
int timer = 0;
int player_x = positions[0][0];
int player_y = positions[0][1];
int player_new_x = player_x;
int player_new_y = player_y;

void pickup_hammer(){
    move_sprite(3, 240, 240);
    move_sprite(4, 240, 240);
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
    if (player_position == 6) {
        set_bkg_tiles(1, 4, 2, 1, 0x02);
    }
    if (player_position == 3) {
        set_bkg_tiles(1, 8, 2, 1, 0x02);
    }
    if (player_position == 8) {
        set_bkg_tiles(17, 4, 2, 1, 0x02);
    }
}

void close_ledge() {
    set_bkg_tiles(0, 0, 32, 32, Bg);
}

void requires_offset() {
    if (player_direction == 0 && (player_position == 1 || player_position == 4)) {
        scroll_sprite(0, +4, 0);
        scroll_sprite(1, +4, 0);
        scroll_sprite(2, +4, 0);
        player_x += 4;
    }
    if (player_direction == 2 && (player_position == 5 || player_position == 8 || player_position == 11)) {
        scroll_sprite(0, +2, 0);
        scroll_sprite(1, +2, 0);
        scroll_sprite(2, +2, 0);
        player_x += 2;
    }
    if (player_direction == 2 && player_position == 10) {
        scroll_sprite(0, -4, 0);
        scroll_sprite(1, -4, 0);
        scroll_sprite(2, -4, 0);
        player_x -= 4;
    }
    if (player_direction == 0 && (player_position == 9 || player_position == 6)) {
        scroll_sprite(0, -2, 0);
        scroll_sprite(1, -2, 0);
        scroll_sprite(2, -2, 0);
        player_x -= 2;
    }
    if (player_direction == 2 && (player_position == 9 || player_position == 6)) {
        scroll_sprite(0, -4, 0);
        scroll_sprite(1, -4, 0);
        scroll_sprite(2, -4, 0);
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
            player_x -=2;
        }
        if (player_x < player_new_x) {
            scroll_sprite(0, 2, 0);
            scroll_sprite(1, 2, 0);
            scroll_sprite(2, 2, 0);
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
            player_y -=2;
        }
        if (player_y < player_new_y) {
            open_ledge(player_x, player_y);
            scroll_sprite(0, 0, 2);
            scroll_sprite(1, 0, 2);
            scroll_sprite(2, 0, 2);
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
    switch(joypad()) {
        case J_RIGHT:
            if (has_reached == 1) {
                has_reached = 0;
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
                requires_scrolling_x = 1;~
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
    }
}

void animate_player() {
    if (frame_counter % 5 == 0) {
        if (player_direction == 0) {
            set_sprite_tile(0, player_tile);
            set_sprite_tile(1, player_tile + 8);
            set_sprite_prop(0, get_sprite_prop(0) & ~S_FLIPX);
            set_sprite_prop(1, get_sprite_prop(1) & ~S_FLIPX);
            set_sprite_prop(2, get_sprite_prop(2) & ~S_FLIPX);
            move_sprite(2, player_x + 3, player_y-4);
        }
        if (player_direction == 2) {
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

        if (has_hammer == 0) {
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
        else {
            set_sprite_tile(3, 20);
            set_sprite_tile(4, 22);
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
    set_bkg_data(0, 20, BgTiles); 
    set_bkg_tiles(0, 0, 32, 32, Bg);
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
    move_sprite(3, 143, 134);
    move_sprite(4, 151, 134);
   
    // Show
    SHOW_SPRITES;
    fadein();
}