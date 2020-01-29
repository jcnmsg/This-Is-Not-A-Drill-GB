/* Global code and variables */
#include "global.c"

/* Graphics */
#include "background/splash.c"

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

/* Starts here */
void main(){
    DISPLAY_ON;
    
    splash();
    
    while(1) {
        while(state == 0) { // 0: Main Menu
            cicled_delay(100);
            play_sound(0);
        }
    }
}
