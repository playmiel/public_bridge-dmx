#include <Arduino.h>
#include <SPI.h>
#include <EEPROM_SPI_WE.h>
#include "ArduinoJson.h"
const int csPin = 15; // Chip select pin
const int wpPin = 9;  // Write protect pin (optional)
                      /* There are different options to create your EEPROM_SPI_WE object:
                       * EEPROM_SPI_WE myEEP = EEPROM_SPI_WE(csPin, wpPin);
                       * EEPROM_SPI_WE myEEP = EEPROM_SPI_WE(csPin);
                       * EEPROM_SPI_WE myEEP = EEPROM_SPI_WE(&SPI, csPin, wpPin);
                       * EEPROM_SPI_WE myEEP = EEPROM_SPI_WE(&SPI, csPin);
                       * If you don't define the wpPin and you connect it to GND or VCC,
                       * then protectStatusRegister() is the only function that won't work.
                       * Passing the SPI object allows you, for example, to use both SPI
                       * interfaces on the ESP32.
                       */

EEPROM_SPI_WE myEEP = EEPROM_SPI_WE(&SPI, csPin);
void setup()
{
    delay(2000);
    Serial.begin(115200);
    if (myEEP.init())
    {
        Serial.println("EEPROM connected");
    }
    else
    {
        Serial.println("EEPROM does not respond");
        while (1)
            ;
    }
    /* You can change the SPI clock speed. The default of is 8 MHz */
    myEEP.setSPIClockSpeed(4000000); // use AFTER init()!

    /* Select the page size of your EEPROM.
     * Choose EEPROM_PAGE_SIZE_xxx,
     * with xxx = 16, 32, 64, 128 or 256
     */
    myEEP.setPageSize(EEPROM_PAGE_SIZE_64);
    byte byteToWrite = 42;
    myEEP.write(10, byteToWrite); // write a byte to EEPROM address 10
    byte byteToRead = myEEP.read(10);
    Serial.print("Byte read: ");
    Serial.println(byteToRead);
    int intToWrite = -4242;
    int intToRead = 0;
    myEEP.put(10, intToWrite); // write an integer to EEPROM address 10
    myEEP.get(10, intToRead);
    Serial.print("Integer read: ");
    Serial.println(intToRead);
    float floatToWrite = 42.42;
    float floatToRead = 0.0;
    myEEP.put(10, floatToWrite);
    myEEP.get(10, floatToRead);
    Serial.print("Float read: ");
    Serial.println(floatToRead);
    String stringToWrite = "This is no poetry, I am just a simple test String";
    String stringToRead = "";
    /* For ESP32 and ESP8266 based boards String read/write does not work
       this way. Use char arrays instead: */
    // char stringToWrite[] = "This is no poetry, I am just a simple test String";
    // char stringToRead[100] = "";
    myEEP.put(110, stringToWrite); // write stringToWrite to address 110
    myEEP.get(110, stringToRead);
    Serial.print("String read: ");
    Serial.println(stringToRead);
    int intArrayToWrite[20];
    int intArrayToRead[20];
    for (unsigned int i = 0; i < (sizeof(intArrayToWrite) / sizeof(int)); i++)
    {
        intArrayToWrite[i] = 10 * i;
    }
    myEEP.put(250, intArrayToWrite);
    myEEP.get(250, intArrayToRead);
    for (unsigned int i = 0; i < (sizeof(intArrayToRead) / sizeof(int)); i++)
    {
        Serial.print("intArrayToRead[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(intArrayToRead[i]);
    }
    delay(100000);
}

void loop() {}