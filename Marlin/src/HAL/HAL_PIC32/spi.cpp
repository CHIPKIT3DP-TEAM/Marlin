#ifdef ARDUINO_ARCH_PIC32

#include "HAL.h"

//spiRate values
// SPI_FULL_SPEED      0
// SPI_HALF_SPEED      1
// SPI_QUARTER_SPEED   2
// SPI_EIGHTH_SPEED    3
// SPI_SIXTEENTH_SPEED 3
// SPI_SPEED_5         5
// SPI_SPEED_6         6

//DSPI0 spi0; // SPI Interface 1
//DSPI1 spi1; // SPI Interface 2
DSPI2   spi2; // SD Card
//DSPI3 spi3; // MRF24

#define spi spi2

// Initialize SPI bus
void spiBegin(void) {
  //TODO: set pins maybe?
  spi.begin();
}

// Configure SPI for specified SPI speed
void spiInit(uint8_t spiRate){
  uint32_t speed = _DSPI_SPD_DEFAULT;

  switch(spiRate) {
  case SPI_FULL_SPEED: break;
  case SPI_HALF_SPEED:
    speed /= 2L;
    break;
  case SPI_QUARTER_SPEED:
    speed /= 4L;
    break;
  case SPI_EIGHTH_SPEED:
    speed /= 8L;
    break;
  case SPI_SIXTEENTH_SPEED:
    speed /= 16L;
    break;
  case SPI_SPEED_5:
    speed /= 32L;
    break;
  case SPI_SPEED_6:
    speed /= 64L;
    break;
  default: break; //to avoid compiler warnings
  }
  spi.setSpeed(speed);
  //TODO: do we need to call begin here?
  // in most other hals, begin is called at the end of init
  // but in a couple(AVR, DUE) the opposite is the case
  // no idea what is necessary...
  spiBegin();
}

// Write single byte to SPI
void spiSend(uint8_t b){
  spi.transfer(b);
}

// Read single byte from SPI
uint8_t spiRec(void){
  return (uint8_t)spi.transfer(0);
}

// Read from SPI into buffer
void spiRead(uint8_t* buf, uint16_t nbyte){
  spi.transfer(nbyte, (uint8_t)0, buf);
}

// Write token and then write from 512 byte buffer to SPI (for SD card)
void spiSendBlock(uint8_t token, const uint8_t* buf){
  spiSend(token);
  //apparently the transfer functions don't use const on their buffer arguments...
  spi.transfer(512, const_cast<uint8_t *>(buf));
}

// Begin SPI transaction, set clock, bit order, data mode
void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode){
  //NOTE: this function currently isn't used anywhere

  //TODO: it isn't defined whether this function would use an absolute clock value
  // or one of the speed macros defined in the HAL_SPI header. however,
  // considering the variable is 32 bits, I'm assuming it's absolute.
  //Re-evaluate in the future.
  spi.setSpeed(spiClock);

  //TODO: the DSPI library does not support LSB_First SPI Data. is this a problem?
  // this function, and by extension the bitorder macros, are never called so
  // I don't belive this is an issue for now. but in the future we may need to switch
  // the chipkit SPI library which does provide support for changing the bit order
  UNUSED(bitOrder); //mark unused so we don't get compiler warnings

  //TODO: exactly what each "data mode" does on the marlin side isn't defined and none
  // of these macros are ever used so just do a 1-to-1 map with the DSPI mode macros for now.
  //Re-evaluate in the future.
  switch(dataMode) {
  case SPI_DATAMODE_0:
    spi.setMode(DSPI_MODE0);
  case SPI_DATAMODE_1:
    spi.setMode(DSPI_MODE1);
  case SPI_DATAMODE_2:
    spi.setMode(DSPI_MODE2);
  case SPI_DATAMODE_3:
    spi.setMode(DSPI_MODE3);
  }
}


#endif
