#ifdef ARDUINO_ARCH_PIC32 //nothing in this file should be compiled if not using a pic32

#include "../../Marlin.h"

#include <malloc.h>

void HAL_clear_reset_source(void) {((void)0);}

uint8_t HAL_get_reset_source(void)  {
    //definitions are providied for all bits of the RCON register
    // even thought I'm pretty sure we won't hit a deadman or wake up reset
    // in this program

    int rcon = getImageHeaderInfoStructure()->pRamHeader->rcon & 0xFF;
    if(rcon & 1<<0) return RST_POWER_ON;
    if(rcon & 1<<1) return RST_BROWN_OUT;
    if(rcon & 1<<2) return RST_POWER_ON; //waking up idle
    if(rcon & 1<<3) return RST_POWER_ON; //waking up sleep
    if(rcon & 1<<4) return RST_WATCHDOG;
    if(rcon & 1<<5) return RST_WATCHDOG; //deadman
    if(rcon & 1<<6) return RST_SOFTWARE;
    if(rcon & 1<<7) return RST_EXTERNAL;
    return 0;
}

int freeMemory(void) {
    //lucky us theres a function for this.
    //http://man7.org/linux/man-pages/man3/mallinfo.3.html

    //TODO:
    //it may be the case that if mallinfo is called without malloc being called first
    // mallinfo will return an empty struct.
    // this line will guarantee that won't happen.
    //further testing is required to see if this is even neccessary
    free(malloc(1));

    return mallinfo().fordblks;
}

#endif