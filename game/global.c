/* GBDK library */
#include <gb/gb.h>
#include <gb/console.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <rand.h>
#include <time.h>
#include "vendor/gbt_player.h"

/* Global variables */
UINT8 state = 0;

/* General functions */
void cicled_delay(UINT8 cicles){
    UINT8 l;
    for(l = 0; l < cicles; l++) {
        wait_vbl_done();
    }
}

/* Sound data */
//extern const unsigned char * song_Data[];

/* Sound functions */
void play_sound(UINT8 fx) {
    if (fx == 0) { // Laser blowing sound 
        NR52_REG = 0x80; // Turn on sound registers, setting it to 0x00 turns them off
        NR51_REG = 0x11; // Select channel to use: 0x11 - 1, 0x22 - 2, 0x33 - 3, 0x88 - 4, 0xFF - All
        NR50_REG = 0x77; // Volume, min: 0x00, max: 0x77
        
        NR10_REG = 0x1C; // Channel 1, Register 0 => Binary: 00001100
        NR11_REG = 0xC6; // Channel 1, Register 1 => Binary: 11000110 
        NR12_REG = 0x73; // Channel 1, Register 2 => Binary: 01110011
        NR13_REG = 0x00; // Channel 1, Register 3 => Binary: 00000000
        NR14_REG = 0xC3; // Channel 1, Register 4 => Binary: 11000011
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