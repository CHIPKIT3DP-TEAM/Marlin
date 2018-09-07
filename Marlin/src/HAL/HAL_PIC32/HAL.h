#ifndef _HAL_H_
#define _HAL_H_

//===============================================================//
//                          Definitions                          //
//===============================================================//

//a lot of different processing happens if this is a 32 bit processor(which it is)
#define CPU_32_BIT

//these are redefined by our files
#undef _BV
#undef RX_BUFFER_SIZE

// For debuggging purposes
#define xstr(s) str(s)
#define str(s) #s

//===============================================================//
//                           Includes                            //
//===============================================================//
//Note:
// The order of these inclusions is actually very important here. HardwareSerial expects defitions from Board_Defs
//

//the Marlin main file includes a lot of the files and defitions from here
// this causes issues with redefining a lot of very specific macros.
//Including it now means we can make changes that won't be overwritten later
#include <Arduino.h>

#include <avr/pgmspace.h>   //program space storage compatibility definitions
#include <avr/delay.h>      //definitions for delay functions

#include "Board_Defs.h"     //Contains definitions specific to the board we are using
#include <HardwareSerial.h> //Serial Stuff
#include <DSPI.h>

#include "fastio.h"         //for IO-related macros
#include "spi_pins.h"       //to Define the pins used for SPI

#include "../shared/math_32bit.h" //for a single function regarding 32bit math
#include "../shared/HAL_SPI.h"    //define necessary SPI functions


#ifdef __INTELLISENSE__
#include "proc/p32mz2048efg100.h"
#endif

//definitions missing from the pic32 pgmspace header
#define pgm_read_float_near(x) pgm_read_float(x)
#define strstr_P strstr
#define pgm_read_ptr(x) ((void *)x) //TODO: verify this

//===============================================================//
//                            SERIAL                             //
//===============================================================//
#define NUM_SERIAL 1 //TODO: see how many Serial Interfaces we're actually going to use

#if (SERIAL_PORT < 0) && (SERIAL_PORT > NUM_SERIAL_PORTS)
//the WITHIN macro doens't seem to work here. throws preprocessor errors.
  #error "SERIAL_PORT must be from 0 to " xstr(NUM_SERIAL_PORTS)
#endif

//Detailed in the HardwareSerial.h file.
// 7 Serial Interfaces are detailed but only defined if they exist
// if we try to use one that isn't defined, the code above will throw an error
#if SERIAL_PORT == -1
  //  if Serial is USB then -1 = SERIAL_PORT_USBVIRTUAL = SERIAL_PORT_MONITOR
  //  otherwise -1 = 0 = SERIAL_PORT_MONITOR = SERIAL_PORT_HARDWARE
  #define MYSERIAL0 SERIAL_PORT_MONITOR
#elif SERIAL_PORT == 0
  #define MYSERIAL0 SERIAL_PORT_HARDWARE
#elif SERIAL_PORT == 1
  #define MYSERIAL0 SERIAL_PORT_HARDWARE1
#elif SERIAL_PORT == 2
  #define MYSERIAL0 SERIAL_PORT_HARDWARE2
#elif SERIAL_PORT == 3
#define MYSERIAL0 SERIAL_PORT_HARDWARE3
#elif SERIAL_PORT == 4
#define MYSERIAL0 SERIAL_PORT_HARDWARE4
#elif SERIAL_PORT == 5
#define MYSERIAL0 SERIAL_PORT_HARDWARE5
#elif SERIAL_PORT == 6
#define MYSERIAL0 SERIAL_PORT_HARDWARE6
#elif SERIAL_PORT == 7
#define MYSERIAL0 SERIAL_PORT_HARDWARE7
#endif

//===============================================================//
//                         Reset Sources                         //
//===============================================================//
//Notes:
// The Macros here aren't actually used anywhere.
// The functions are called on line 727 in Marlin.cpp but to get the reset source
//  they check against an integer which means the valus here can't be changed or
//  they become wrong.
// Pretty sure this is a bug in Marlin so we use a switch case to return the correct value
//
// Hidden deep in the bowels of the wiring header, there seems to be the indication that
//  the RCON register is reset by the bootloader on startup...which means the RCON register is now useless.
// Luckily this register(along with a bunch of other information) is saved before being cleared and 
//  stored in the RAM by the bootloader.

#define RST_POWER_ON   1
#define RST_EXTERNAL   2
#define RST_BROWN_OUT  4
#define RST_WATCHDOG   8
#define RST_JTAG       16
#define RST_SOFTWARE   32
#define RST_BACKUP     64

void HAL_clear_reset_source (void);
uint8_t HAL_get_reset_source (void);

//===============================================================//
//                             Timers                            //
//===============================================================//
typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define STEP_TIMER_NUM 0  //TODO: index of timer to use for stepper
#define TEMP_TIMER_NUM 1  //TODO: index of timer to use for temperature

//On all platforms, it seems the step timer and the pulse timer are exactly the same thing.
// I'd have to read into the code to verify this so for now just blindly trust
#define PULSE_TIMER_NUM (STEP_TIMER_NUM)

#define HAL_TIMER_RATE  F_CPU

#define TEMP_TIMER_FREQUENCY    1000

#define STEPPER_TIMER_PRESCALE  2
#define STEPPER_TIMER_RATE      (HAL_TIMER_RATE/STEPPER_TIMER_PRESCALE)
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000)

#define PULSE_TIMER_RATE         STEPPER_TIMER_RATE
#define PULSE_TIMER_PRESCALE     STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED()              HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)
#define TEMPERATURE_ISR_ENABLED()       HAL_timer_interrupt_enabled(TEMP_TIMER_NUM) //this isn't actually used anywhere

extern void __USER_ISR stepper_isr(void);
extern void __USER_ISR temperature_isr(void);

//used to define the ISR function. this should be the function signature without a block or a semicolon
#define HAL_STEP_TIMER_ISR void __USER_ISR stepper_isr(void)
#define HAL_TEMP_TIMER_ISR void __USER_ISR temperature_isr(void)


//Enable and initialize the timer
void HAL_timer_start (const uint8_t timer_num, uint32_t frequency);

//Set/Get upper compare value
void        HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t count);
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num);

//get current timer value
hal_timer_t HAL_timer_get_count(const uint8_t timer_num);

//if counter too high then bump up compare
void HAL_timer_restrain(const uint8_t timer_num, const uint16_t interval_ticks);

//enable timer interrupt
void HAL_timer_enable_interrupt(const uint8_t timer_num);

//disable timer interrupt
void HAL_timer_disable_interrupt(const uint8_t timer_num);

//is interrupt enabled
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

//executed before main interrupt
void HAL_timer_isr_prologue(const uint8_t timer_num);

//executed after main interrupt
void HAL_timer_isr_epilogue(const uint8_t timer_num);

//===============================================================//
//                          Pin Mapping                          //
//===============================================================//
//Marlin seems to use something called a "Pin Map" to translate a
// pin number to a hardware register.
//Every "pin" is, in essence, an "index" to it's corresponding
// hardware pin in the "pin map"
//The Arduino Platform already does this, which is why most of the
// HALs don't define these functions and instead just return their paramters
//This is a useful feature in the case that a platform does not have
// an arduino core such as the LPC1768 HAL.

//In the case that they do, the pin number and pin index are interchangable
// and all translation is performed through te macros in the fastio.h file.
typedef int8_t pin_t;

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin

//This function is intended to do a translation between the pin numbers sent in GCode
// Command Parameters and the actual pin indices.
//This is only used in the following GCodes:
//  - M42  - Switch I/O pin
//  - M43  - Pin report and debug
//  - M226 - Wait for pin state
//As we have an arduino based environment, It's not necessary to change the default numbers(as noted above)
//the default definition just takes the argument Code and turns it's paramter into an integer and returns it
//I don't think we need any more than that.
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

//this function(as the name implies) converts an analog pin number to it's corresponding
//digital pin number. The core already has a function for this but it has a different name.
#define analogInputToDigitalPin(x) analogInPinToChannel(x)


//===============================================================//
//                              ADC                              //
//===============================================================//
//The chipkit core makes this really easy

#define HAL_ANALOG_SELECT(pin) //TODO: does this need to do anything?
#define HAL_adc_init()         //core will handle init
#define HAL_START_ADC(pin)     analogReadConversionStart(pin)
#define HAL_READ_ADC()         analogReadConversion()
#define HAL_ADC_READY()        analogReadConversionComplete()

//===============================================================//
//                         Miscellaneous                         //
//===============================================================//
int freeMemory(void); //the number of bytes available on the heap

//why are there so many different ways to enable/disable interrupts
#define cli() disableInterrupts()
#define sei() enableInterrupts()

inline bool ISRS_ENABLED() {
  int r = disableInterrupts();
  restoreInterrupts(r);
  return r;
}
#define ENABLE_ISRS()  sei()
#define DISABLE_ISRS() cli()

void watchdog_init();
void watchdog_reset();

#endif