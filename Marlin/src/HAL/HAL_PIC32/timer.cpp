#ifdef ARDUINO_ARCH_PIC32 //nothing in this file should be compiled if not using a pic32

#include "../../Marlin.h"

#define STEPPER_VECTOR _TIMER_2_VECTOR
#define TEMP_VECTOR    _TIMER_4_VECTOR

#ifdef __PIC32MZ__
#define STEPPER_IRQ _TIMER_2_VECTOR
#define TEMP_IRQ    _TIMER_4_VECTOR
#else
#define STEPPER_IRQ _TIMER_2_IRQ
#define TEMP_IRQ    _TIMER_4_IRQ
#endif

void HAL_timer_isr_prologue(const uint8_t timer_num) {
    UNUSED(timer_num);
}

void HAL_timer_isr_epilogue(const uint8_t timer_num) {
    switch(timer_num) {
    case STEP_TIMER_NUM:
        clearIntFlag(STEPPER_IRQ);
        break;
    case TEMP_TIMER_NUM:
        clearIntFlag(TEMP_IRQ);
        break;
    default: break; //silence warnings
    }
}

//Enable and initialize the timer
void HAL_timer_start (const uint8_t timer_num, uint32_t frequency) {
    int pre = 1; //prescaler of 2

    int period = (HAL_TIMER_RATE) / pre / frequency - 1;

    switch(timer_num) {
    case STEP_TIMER_NUM:
        T2CON = TMR2 = TMR3 = PR2 = PR3 = 0; //clear registers
        T2CON |= (pre & 0x7) << 4;           //set prescaler
        T2CONSET = 1<<4;                     //set 32 bit mode
        T2CONSET = 1<<15;                    //turn on

        HAL_timer_set_compare(STEP_TIMER_NUM, period);

        setIntVector(STEPPER_VECTOR, stepper_isr);
        setIntPriority(STEPPER_VECTOR, 4, 0);
        clearIntFlag(STEPPER_IRQ);
        setIntEnable(STEPPER_IRQ);
        break;
    case TEMP_TIMER_NUM:
        T4CON = TMR4 = TMR5 = PR4 = PR5 = 0; //clear registers
        T4CON |= (pre & 0x7) << 4;           //set prescaler
        T4CONSET = 1<<4;                     //set 32 bit mode
        T2CONSET = 1<<15;                    //turn on

        HAL_timer_set_compare(TEMP_TIMER_NUM, period);

        setIntVector(TEMP_VECTOR, temperature_isr);
        setIntPriority(TEMP_VECTOR, 4, 1);
        clearIntFlag(TEMP_IRQ);
        setIntEnable(TEMP_IRQ);
        break;
    default: break; //silence warnings
    }
}

//Set/Get upper compare value
void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t count) {
    switch(timer_num) {
    case STEP_TIMER_NUM:
        PR2 = count & 0xFF;
        PR3 = count >> 16;
        break;
    case TEMP_TIMER_NUM:
        PR4 = count & 0xFF;
        PR5 = count >> 16;
        break;
    default: break; //silence warnings
    }
}
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
    switch(timer_num) {
    case STEP_TIMER_NUM:
        return (PR3 << 16) | PR2;
    case TEMP_TIMER_NUM:
        return (PR5 << 16) | PR4;
    default: return 0; //silence warnings
    }
}

//get current timer value
hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
    switch(timer_num) {
    case STEP_TIMER_NUM:
        return (TMR3 << 16) | TMR2;
    case TEMP_TIMER_NUM:
        return (TMR5 << 16) | TMR4;
    default: return 0; //silence warnings
    }
}

//if counter too high then bump up compare
void HAL_timer_restrain(const uint8_t timer_num, const uint16_t interval_ticks) {
    const hal_timer_t mincmp = HAL_timer_get_count(timer_num) + interval_ticks;
    if (HAL_timer_get_compare(timer_num) < mincmp) HAL_timer_set_compare(timer_num, mincmp);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
    switch(timer_num) {
    case STEP_TIMER_NUM:
        setIntEnable(STEPPER_VECTOR);
        break;
    case TEMP_TIMER_NUM:
        setIntEnable(TEMP_VECTOR);
        break;
    default: break; //silence warnings
    }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
    switch(timer_num) {
    case STEP_TIMER_NUM:
        clearIntEnable(STEPPER_VECTOR);
        break;
    case TEMP_TIMER_NUM:
        clearIntEnable(TEMP_VECTOR);
        break;
    default: break; //silence warnings
    }
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
    //disabling an interrupt returns it's previous state.
    //then we can restore the state and return it.
    uint32_t r = 0;
    switch(timer_num) {
    case STEP_TIMER_NUM:
        r = clearIntEnable(STEPPER_VECTOR);
        restoreIntEnable(STEPPER_VECTOR, r);
        break;
    case TEMP_TIMER_NUM:
        r = clearIntEnable(TEMP_VECTOR);
        restoreIntEnable(TEMP_VECTOR, r);
        break;
    default: break; //silence warnings
    }
    return (bool)r;
}

#endif