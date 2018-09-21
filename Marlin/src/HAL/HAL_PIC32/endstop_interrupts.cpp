#ifdef ARDUINO_ARCH_PIC32

#include "../../inc/MarlinConfig.h"

#include "endstop_interrupts.h"

//We don't include Endstop Interrupts here because it get included
// via a HAL_PATH macro through src/module/enstops.h

/**
 * NOTE:
 * Change Notification is a relatively powerful future that seems to essentially
 * replace External Interrupts(or at least make it more specific)
 * Unfortunately, this feature isn't included in the chipkit core beyond
 * small allusions to it's existance. This means that to use this feature, we
 * have to implement it manually.
 *
 * All information regarding Change Notification Setups was "borrowed"
 * From the SoftwareSerial Library in the chipkit Core
 * It has been simplified slightly and reduced to fit only
 * the pic on the WiFire: a PPS compatible PIC32-MZ2048EFG100
 *
 * Before releasing this HAL it may be a good idea to expand the implementation
 * to more pic32s.
 *
 * NOTE2:
 * Change Notification is implemented here on all Ports that the pic32 has.
 * This poses a problem if we want to use a library that uses this
 * feature(like SoftwareSerial).
 * For the time being we will have to be careful not to use a library that
 * uses this feature. Maybe in the future we can limit Endstop pins to 1 or 2 ports.
 */

//declare the callback so we can set it later
void(* ISR_callback)(void);

//Setup an ISR for every port so we know which flag to clear.
// they all do the same thing.
void __USER_ISR cn_ISR_A(void) {
  ISR_callback();

#if defined(_PORTA)
  clearIntFlag(_CHANGE_NOTICE_A_VECTOR);
#endif
}

void __USER_ISR cn_ISR_B(void) {
  ISR_callback();

#if defined(_PORTB)
  clearIntFlag(_CHANGE_NOTICE_B_VECTOR);
#endif
}

void __USER_ISR cn_ISR_C(void) {
  ISR_callback();

#if defined(_PORTC)
  clearIntFlag(_CHANGE_NOTICE_C_VECTOR);
#endif
}

void __USER_ISR cn_ISR_D(void) {
  ISR_callback();

#if defined(_PORTD)
  clearIntFlag(_CHANGE_NOTICE_D_VECTOR);
#endif
}

void __USER_ISR cn_ISR_E(void) {
  ISR_callback();

#if defined(_PORTE)
  clearIntFlag(_CHANGE_NOTICE_E_VECTOR);
#endif
}

void __USER_ISR cn_ISR_F(void) {
  ISR_callback();

#if defined(_PORTF)
  clearIntFlag(_CHANGE_NOTICE_F_VECTOR);
#endif
}

void __USER_ISR cn_ISR_G(void) {
  ISR_callback();

#if defined(_PORTG)
  clearIntFlag(_CHANGE_NOTICE_G_VECTOR);
#endif
}

void __USER_ISR cn_ISR_H(void) {
  ISR_callback();

#if defined(_PORTH)
  clearIntFlag(_CHANGE_NOTICE_H_VECTOR);
#endif
}

void __USER_ISR cn_ISR_J(void) {
  ISR_callback();

#if defined(_PORTJ)
  clearIntFlag(_CHANGE_NOTICE_J_VECTOR);
#endif
}

void __USER_ISR cn_ISR_K(void) {
  ISR_callback();

#if defined(_PORTK)
  clearIntFlag(_CHANGE_NOTICE_K_VECTOR);
#endif
}

void _setupCNISR(void(* callback)(void)) {
  //to prevent enabling the ISRs multiple times
  static bool ISR_Enabled = false;
  if(ISR_Enabled) return;

  ISR_callback = callback;

//Unfortunately the PIC32MZ has a Separate Interrrupt Vector for each port so we have
// to turn on all of them.
//But we turn on Change Notification on a per-pin basis so this isn't detrimental
#if defined(_PORTA)
  CNCONAbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_A_VECTOR, cn_ISR_A);
  setIntPriority(_CHANGE_NOTICE_A_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_A_VECTOR);
#endif
#if defined(_PORTB)
  CNCONBbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_B_VECTOR, cn_ISR_B);
  setIntPriority(_CHANGE_NOTICE_B_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_B_VECTOR);
#endif
#if defined(_PORTC)
  CNCONCbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_C_VECTOR, cn_ISR_C);
  setIntPriority(_CHANGE_NOTICE_C_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_C_VECTOR);
#endif
#if defined(_PORTD)
  CNCONDbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_D_VECTOR, cn_ISR_D);
  setIntPriority(_CHANGE_NOTICE_D_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_D_VECTOR);
#endif
#if defined(_PORTE)
  CNCONEbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_E_VECTOR, cn_ISR_E);
  setIntPriority(_CHANGE_NOTICE_E_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_E_VECTOR);
#endif
#if defined(_PORTF)
  CNCONFbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_F_VECTOR, cn_ISR_F);
  setIntPriority(_CHANGE_NOTICE_F_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_F_VECTOR);
#endif
#if defined(_PORTG)
  CNCONGbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_G_VECTOR, cn_ISR_G);
  setIntPriority(_CHANGE_NOTICE_G_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_G_VECTOR);
#endif
#if defined(_PORTH)
  CNCONHbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_H_VECTOR, cn_ISR_H);
  setIntPriority(_CHANGE_NOTICE_H_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_H_VECTOR);
#endif
#if defined(_PORTJ)
  CNCONJbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_J_VECTOR, cn_ISR_J);
  setIntPriority(_CHANGE_NOTICE_J_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_J_VECTOR);
#endif
#if defined(_PORTK)
  CNCONKbits.ON = 1;
  setIntVector(_CHANGE_NOTICE_K_VECTOR, cn_ISR_K);
  setIntPriority(_CHANGE_NOTICE_K_VECTOR, 4, 0);
  clearIntFlag(_CHANGE_NOTICE_K_VECTOR);
#endif

//Enable all interrupts(not done above so we don't get interrupted during setup)
#if defined(_PORTA)
  setIntEnable(_CHANGE_NOTICE_A_VECTOR);
#endif
#if defined(_PORTB)
  setIntEnable(_CHANGE_NOTICE_B_VECTOR);
#endif
#if defined(_PORTC)
  setIntEnable(_CHANGE_NOTICE_C_VECTOR);
#endif
#if defined(_PORTD)
  setIntEnable(_CHANGE_NOTICE_D_VECTOR);
#endif
#if defined(_PORTE)
  setIntEnable(_CHANGE_NOTICE_E_VECTOR);
#endif
#if defined(_PORTF)
  setIntEnable(_CHANGE_NOTICE_F_VECTOR);
#endif
#if defined(_PORTG)
  setIntEnable(_CHANGE_NOTICE_G_VECTOR);
#endif
#if defined(_PORTH)
  setIntEnable(_CHANGE_NOTICE_H_VECTOR);
#endif
#if defined(_PORTJ)
  setIntEnable(_CHANGE_NOTICE_J_VECTOR);
#endif
#if defined(_PORTK)
  setIntEnable(_CHANGE_NOTICE_K_VECTOR);
#endif
  ISR_Enabled = true;
}

//setup a particular pin for Change Notifications
void _setupCNPin(pin_t pin) {
  uint8_t     port;
  uint16_t    pic_bitMask;
  p32_ioport *pic_port;

  //On a PPS Compatible PIC32 all pins are CN-capable
  // so no need to check

  //get the port and pin mask associated with a particular pin
  if ((port = digitalPinToPort(pin)) == NOT_A_PIN) return;
  pic_port = (p32_ioport *)portRegisters(port);
  pic_bitMask = digitalPinToBitMask(pin);

  //set the CN registers for the pin
  pic_port->cnen.set = pic_bitMask;
  pic_port->cncon.reg = 0x8000;

  //read from it(this is what SoftwareSerial does, idk if it's necessary)
  volatile int tmp = pic_port->port.reg;
}

//placing this function call inside a wrapping function eliminates the
// implicit class pointer. otherwise I would just pass it straight into
// the ISR setup function
void callback(void) { endstops.update(); }

//General Flow for this function:
//Turn on ISRs: this will only run the first time it's called
//for each endstop
//  check if endstop is set
//  check if the pin actually exists
//  enable the Change Notificaion Function for the pin
void setup_endstop_interrupts(void) {
  _setupCNISR(*callback);

#if HAS_X_MAX
  if(digitalPinToPort(X_MAX_PIN) != NOT_A_PIN)
    _setupCNPin(X_MAX_PIN);
#endif

#if HAS_X_MIN
  if(digitalPinToPort(X_MIN_PIN) != NOT_A_PIN)
    _setupCNPin(X_MIN_PIN);
#endif

#if HAS_Y_MAX
  if(digitalPinToPort(Y_MAX_PIN) != NOT_A_PIN)
    _setupCNPin(Y_MAX_PIN);
#endif

#if HAS_Y_MIN
  if(digitalPinToPort(Y_MIN_PIN) != NOT_A_PIN)
    _setupCNPin(Y_MIN_PIN);
#endif

#if HAS_Z_MAX
  if(digitalPinToPort(Z_MAX_PIN) != NOT_A_PIN)
    _setupCNPin(Z_MAX_PIN);
#endif

#if HAS_Z_MIN
  if(digitalPinToPort(Z_MIN_PIN) != NOT_A_PIN)
    _setupCNPin(Z_MIN_PIN);
#endif

#if HAS_Z2_MAX
  if(digitalPinToPort(HAS_Z2_MAX) != NOT_A_PIN)
    _setupCNPin(HAS_Z2_MAX);
#endif

#if HAS_Z2_MIN
  if(digitalPinToPort(Z2_MIN_PIN) != NOT_A_PIN)
    _setupCNPin(Z2_MIN_PIN);
#endif

#if HAS_Z_MIN_PROBE_PIN
  if(digitalPinToPort(Z_MIN_PROBE_PIN) != NOT_A_PIN)
    _setupCNPin(Z_MIN_PROBE_PIN);
#endif
}

#endif