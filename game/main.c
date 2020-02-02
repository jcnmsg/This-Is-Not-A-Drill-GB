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

/* void init_menu() {

}*/

/* Starts here */
void main(){
    DISPLAY_ON;
    
    splash();

    //init_menu();
    
    while(1) {
        while(state == 0) { // 0: Main Menu
            waitpad(J_START);
            init_game();
            state = 1;
        }
        while(state == 1){
            cicled_delay(1);
            animate_player();
            process_input();
            scroll_player_sprite();
            timer+=1;
        }
    } 
}
