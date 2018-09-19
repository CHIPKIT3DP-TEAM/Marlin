# PIC32 Marlin Port Effort

## Compilation
Compilation is currently only supported through the Arduino IDE.

The project can be opened in the Arduino IDE via Marlin/Marlin.ino and compiled or uploaded that way. The board must be changed to the chipkit-wifire or Marlin may build for a different device

The Configuration files used the compile the HAL have also not been uploaded as they are too specific to the 3d printer.
The only necessary change to be made thus far is to change the board used to `BOARD_SHIELD3DP` on line 136(ish). You can also search for `MOTHERBOARD` and you should be able to find it

## Git Stuff
The upstream branch is updated incredibly often(approx. once per hour). To rebase our changes, the following commands are used
```bash
git remote add upstream https://github.com/MarlinFirmware/Marlin.git #if not already added
git checkout bugfix-2.0.x #if not already on this branch
git stash -a #if any local changes exist

git fetch upstream
git rebase upstream/bugfix-2.0.x
#resolve any merge conflicts here
git push --force
```
This will rewind all local changes and place them on top of the upstream changes. This will also help to keep the HAL updated.

## Summary of Changes
Each of these can be found by searching the codebase for "@zawata" which has been used to tag changes thusfar
- `Marlin/src/HAL/HAL_PIC32/*`
    - The main HAL for use with the chipkit-wifire
- `Marlin/src/core/boards.h`
    - Add a definition for the shield3dp we are using
- `Marlin/src/HAL/platforms.h`
    - Tell the compiler when to include our HAL
- `Marlin/src/HAL/shared/Delay.h`
    - Add a definition for a PIC32-specific cycle-delay function
- `Marlin/src/pins/pins.h`
    - Add logic for selecting a pin setup for our shield
- `Marlin/src/pins/pins_shield3DP.h`
    - A custom pin setup for our shield

## To be Done
- SPI
    - Testing
- Watchdog Timer
    - Testing
- ADC Interface
    - Testing
- Timers 
    - Review
    - Testing
- EEPROM Interface 
    - Testing
- I2C
    - Implementation
    - Do we have any devices requiring I2C?
- Endstop Interrupts 
    - Switch to Pin Change Notice Interrupts
    - See General Notes
- Displays
- Various TODOs
- Corrected PlatformIO support
    - See General Notes

## Done
- SPI
    - Implemented
    - See General Notes
- Serial Communication
    - Implemented
    - Tested
- Pin Functions
    - Researched
    - Implemented
- Watchdog Timer
    - Implemented
- ADC Interface
    - Implemented
- Timers 
    - Implemented
- EEPROM Interface 
    - Implemented
    - Tested

## General Notes
- Endstop Interrupts
    - This feature is mostly infeasible on the pic32. PIC32s only seem to have 5 external interrupts. The device is incredibly fast relative to the atmega328 Marlin was designed for so constant polling will not be as much of an issue as it would on slower devices.
    - This may actually be possible using "Change Notice" Pins but I'm unsure of how they work. there seems to be many more of them however and they seem to be able to fire an interrupt.
- SPI Support
    - Contrary to logical SPI Design assumptions, each SPI-capable device on the Wi-Fire is on it's own SPI bus. Because of the way Marlin is designed, a single API handles all SPI communication without differentiation for devices. This poses a problem in that we won't know at call time which SPI bus we are supposed to use. It is for this reason we can only use a single SPI bus. Luckily there seems to be only 2 SPI-capable devices that marlin supports, the MAX6675 thermocouple, and SD Cards. We aren't using that thermocouple so we only need our spi interface to support SD Cards.
- PlatformIO Support
    - PlatformIO support has been attempted but appears to fail on Windows as the pic32-g++ compiler ignores backslashes as folder separators. To fix this, we will likely need to send a bug report in [here](https://github.com/platformio/platform-microchippic32) telling them not to use python's `os.path.join` function and instead join with a forward slash universally.