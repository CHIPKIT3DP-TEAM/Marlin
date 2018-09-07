//Defines which pins are to be used for SPI

#ifndef SPI_PINS_H_
#define SPI_PINS_H_

#include "wiring.h" //for the integer declarations

#define SS_PIN   PIN_DSPI2_SS
#define SCK_PIN  51 //TODO: I can't find a macro for this
#define MISO_PIN _DSPI2_MISO_PIN
#define MOSI_PIN _DSPI2_MOSI_PIN


#endif // SPI_PINS_H_