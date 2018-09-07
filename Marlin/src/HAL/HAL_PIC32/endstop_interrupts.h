#ifndef _ENDSTOP_INTERRUPTS_H_
#define _ENDSTOP_INTERRUPTS_H_

#include "../../module/endstops.h"

// One ISR for all EXT-Interrupts
void endstop_ISR(void) { endstops.update(); }

#define MODE RISING //TODO: is it RISING or FALLING?

#warning \
"endstop interrupts are infeasible for the PIC32 due to it only having "
"5 external interrupt capable pins. this file is only included for completion's sake "
"and if ever in the future it becomes relevant. It is recommended to keep this feature disabled."

//TODO: Change Notification Interrupts may be useful here...but theres no support for them in the chipkit core
// the SoftwareSerial makes extensive use of them however and can be used as a reference for implementing them here

void setup_endstop_interrupts(void) {
  #if HAS_X_MAX
    #if digitalPinToInterrupt(X_MAX_PIN) != 0xFF
      #error "X_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(X_MAX_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_X_MIN
    #if digitalPinToInterrupt(X_MIN_PIN) != 0xFF
      #error "X_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(X_MIN_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_Y_MAX
    #if digitalPinToInterrupt(Y_MAX_PIN) != 0xFF
      #error "Y_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Y_MAX_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_Y_MIN
    #if digitalPinToInterrupt(Y_MIN_PIN) != 0xFF
      #error "Y_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Y_MIN_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_Z_MAX
    #if digitalPinToInterrupt(Z_MAX_PIN) != 0xFF
      #error "Z_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z_MAX_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_Z_MIN
    #if digitalPinToInterrupt(Z_MIN_PIN) != 0xFF
      #error "Z_MIN_PIN is not an INTERRUPT capable pin."
    #endif
     attachInterrupt(digitalPinToInterrupt(Z_MIN_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_Z2_MAX
    #if digitalPinToInterrupt(Z2_MAX_PIN) != 0xFF
      #error "Z2_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z2_MAX_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_Z2_MIN
    #if digitalPinToInterrupt(Z2_MIN_PIN) != 0xFF
      #error "Z2_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z2_MIN_PIN), endstop_ISR, MODE);
  #endif
  #if HAS_Z_MIN_PROBE_PIN
    #if digitalPinToInterrupt(Z_MIN_PROBE_PIN) != 0xFF
      #error "Z_MIN_PROBE_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z_MIN_PROBE_PIN), endstop_ISR, MODE);
  #endif
}

#endif //_ENDSTOP_INTERRUPTS_H_
