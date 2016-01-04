/**
* @copyright   Николай Крашенинников
* @project     Bottom project
* @file        timetest.ino
* @brief       Программа для оценки точности выдерживания Raspberry Pi собственной дискретности, часть для Arduino.
*
* Файл реализации точки страта проектов на Arduino: функции #setup() и #loop().
*/

#include <limits.h>

#include "library.h"

#define LENGTH  ( 256 + 128 )
#define PIN     ( 2 )

unsigned long mass[LENGTH];
size_t pos;

void pin_change()
{
    noInterrupts();
    if (pos < LENGTH) {
        mass[pos++] = micros();
    }
    interrupts();
}

void setup()
{
    pinMode(PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN), pin_change, CHANGE);
    Serial.begin(115200);

    pos = 0;
}

void loop()
{
    if (l_kbhit()) {
        switch (l_getch()) {
        case 'r':
        case 'R':
            reset_loop();
            break;
        default:
            serial_print_info();
        }
    }
}

void serial_print_info()
{
    Serial.println();
    Serial.print(F("DATA: "));
    Serial.println(pos);

    unsigned long avg = 0;
    unsigned long min = ULONG_MAX;
    unsigned long max = 0;
    unsigned long diff;
    for (size_t i = 1; i < pos; ++i) {
        diff = mass[i] - mass[i - 1];
        Serial.println(diff);
        if (diff > max) {
            max = diff;
        }
        if (diff < min) {
            min = diff;
        }
        avg += diff;
    }
    avg /= pos - 1;

    Serial.print(F("Average: "));
    Serial.println(avg);
    Serial.print(F("MIN: "));
    Serial.println(min);
    Serial.print(F("MAX: "));
    Serial.println(max);

    Serial.println();
}

void reset_loop()
{
    noInterrupts();
    pos = 0;
    Serial.println(F("Reset completed"));
    interrupts();
}
