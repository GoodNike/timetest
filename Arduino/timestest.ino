#include "library.h"

#define LENGTH  ( 256 )
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
        serial_print_info();
        l_getch();
    }
}

void serial_print_info()
{
    Serial.println();
    Serial.println(F("DATA: "));
    Serial.println(pos);
    for (size_t i = 1; i < pos; ++i) {
        Serial.println(mass[i] - mass[i - 1]);
    }
    Serial.println();
}
