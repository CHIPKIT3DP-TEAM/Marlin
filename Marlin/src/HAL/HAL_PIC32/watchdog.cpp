#ifdef ARDUINO_ARCH_PIC32 //nothing in this file should be compiled if not using a pic32

#include "../../Marlin.h"

#if ENABLED(USE_WATCHDOG)

void watchdog_init() {
  //the exact timeout value seems to vary between 4 and 8
  // for now use 4:
  // 0xc = 4.096s
  // 0xd = 8.192s
  WDTCONbits.WDTWINEN = 0;
#ifdef __PIC32MZ__
  WDTCONbits.RUNDIV = 0xC;
#else
  WDTCONbits.SWDTPS = 0xC;
#endif

  WDTCONbits.ON = 1;
}

void watchdog_reset() {
#ifdef __PIC32MZ__
  WDTCONbits.WDTCLRKEY = 0x5743;
#else
	WDTCONbits.WDTCLR = 1;
#endif
}

#endif // USE_WATCHDOG
#endif // ARDUINO_ARCH_PIC32
