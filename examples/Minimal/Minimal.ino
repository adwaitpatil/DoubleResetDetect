/* Minimal.ino
 *
 * Copyright (C) 2018 Jens-Christian Skibakk
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <DoubleResetDetect.h>
#include <EEPROM.h>

// maximum number of seconds between resets that
// counts as a double reset 
#define DRD_TIMEOUT 2.0

// address to the block in the RTC user memory
// change it if it collides with another usage 
// of the address block
#define DRD_ADDRESS 0x00

// EEPROM adress at which to store the compile
// date/time to detect first run after flash.
// Edit accordingly if using EEPROM elsewhere.
#define EEPROM_ADDRESS 0x00
bool ipl(); // Define ipl function

DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("***************************************");
    Serial.println("* Minimal double reset detect example *");
    Serial.println("***************************************");
    Serial.println();

    if (drd.detect() && !ipl()) {
        Serial.println("** Double reset boot **");
    } else {
        Serial.println("** Normal boot **");
    }
}

void loop()
{
    // do stuff here
}

bool ipl() { // Determine if this is the first start after loading image
    char thisver[20] = __DATE__ __TIME__; // Sets at compile-time
    char savever[20] = "";
    bool _ipl = false;

    EEPROM.begin(20);
    EEPROM.get(EEPROM_ADDRESS, savever);
    if (strcmp (thisver, savever) != 0) {
        EEPROM.put(EEPROM_ADDRESS, thisver);
        EEPROM.commit();
        _ipl = true;
    }
    EEPROM.end();
    return _ipl;
}
