/* GBDK library */
#include <gb/gb.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <rand.h>

/* Global variables */
UINT8 state = 0;

/* General functions */
void cicled_delay(UINT8 cicles){
    UINT8 l;
    for(l = 0; l < cicles; l++) {
        wait_vbl_done();
    }
}

UINT8 generate_random_number(UINT8 upper) {
    UINT8 num;
    num = (rand() % (upper + 1) );
    return num;
}

/* Sound functions */
void play_sound(UINT8 fx) {
    if (fx == 0) { // Hammer Pickup Sound
        NR52_REG = 0x80; 
        NR50_REG = 0x11;
        NR51_REG = 0xFF;
        
        NR10_REG = 0x32;
        NR11_REG = 0xc0;
        NR12_REG = 0x2f;
        NR13_REG = 0x6f;
        NR14_REG = 0x90;
    }
}

/* Visual FX Functions */
void fadeout() {
    UINT8 i;
    for (i=0; i<4; i++) {
        if (i == 0) {
            BGP_REG = 0xE4; // Background palette
            OBP0_REG = 0xE4; // Sprite palette
        }
        if (i == 1) {
            BGP_REG = 0x90;
            OBP0_REG = 0x90;
        }
        if (i == 2) {
            BGP_REG = 0x80;
            OBP0_REG = 0x80;
        }
        if (i == 3) {
            BGP_REG = 0x00;
            OBP0_REG = 0x00;
        }
        cicled_delay(2);
    }
}

void fadein() {
    UINT8 i;
    for (i=0; i<4; i++) {
        if (i == 0) {
            OBP0_REG = 0x00; // Sprite palette
            BGP_REG = 0x00; // Background palette
        }
        if (i == 1) {
            OBP0_REG = 0x80;
            BGP_REG = 0x80;
        }
        if (i == 2) {
            OBP0_REG = 0x90;
            BGP_REG = 0x90;
        }
        if (i == 3) {
            OBP0_REG = 0xE4; 
            BGP_REG = 0xE4;
        }
        cicled_delay(2);
    }
}