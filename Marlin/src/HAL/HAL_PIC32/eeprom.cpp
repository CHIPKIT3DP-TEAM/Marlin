#ifdef ARDUINO_ARCH_PIC32

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)

#include "../shared/persistent_store_api.h"

#include <EEPROM.h>          //EEPROM Stuff

extern "C" {
#include <utility/Deeprom.h> //needed to fix Page Size
}

//called just before and just after intending to access the EEPROM

//NOTE:
//Normally we don't need to do anything here but the Emulated EEPROM library
// that PIC32's utilize(Deeprom, which is the backend of EEPROM) set the total
// usable address space to only 512 bytes.
//This is too small to be useful for Marlin so on start up we set it to the max size
// which is defined in MarlinConfig.h > HAL.h > Board_Defs.h > System_Defs.h
//This used to be modifiable from the EEPROM class but was removed in April 2017
// for compatibility's sake so now we need actually call the backend function.
bool PersistentStore::access_start() {
  static bool init = false;
  if(!init) {
    init = true;
    setMax(_EEPROM_PAGE_SIZE - 1);
  }
}
//nothing to do here though.
bool PersistentStore::access_finish() { return true; }

//The "Print Loop" utilized in the configuration store considers 
// `true` to be an indication of an error.
//It also expects the `pos` parameter to update itself which is why
// it's passed by reference.

//write a byte to the EEPROM, computing the CRC for each byte
bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    //only write bytes that have changed
    EEPROM.update(pos, *value);
    if (EEPROM.read(pos) != *value) {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM(MSG_ERR_EEPROM_WRITE);
      return true;
    }
    crc16(crc, value, 1);
    pos++;
    value++;
  };
  return false;
}

//read a byte from the EEPROM, computing the CRC for each byte
bool PersistentStore::read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool writing) {
  do {
    uint8_t c = EEPROM.read(pos);
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
  return false;
}

//the total l
size_t PersistentStore::capacity() {
    return EEPROM.length();
}

#endif
#endif
