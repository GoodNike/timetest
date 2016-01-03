/**
 * @copyright   Николай Крашенинников
 * @project     Bottom project
 * @file        main.cpp
 * @brief       Программа для оценки точности выдерживания RaspberryPi собственной дискретности.
 *
 * Программа циклично меняет состояние пина GPIO с HIGH на LOW и обратно. Пользователь задает номер
 * пина для выдачи, задает дискретность работы, мс, через командную строку. В случае успешного
 * запуска, программа не возвращает управление.
 */

#include <iostream>
#include <cstdlib>

#include <string.h>
#include <sched.h>
#include <sys/mman.h>

#include <bcm2835.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Usage: timetest <pin_number> <disc>" << std::endl;
        return 1;   // error
    }

    uint8_t pin = std::atoi(argv[1]);
    std::cout << "Using pin: " << (uint32_t) pin << std::endl;
    unsigned int ms = std::atoi(argv[2]);
    std::cout << "Using disc: " << (uint32_t) ms << std::endl;

    if (!bcm2835_init()) {
        std::cout << "bcm2835_init() failure" << std::endl;
        return 1;   // error
    }

    sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sp);
    mlockall(MCL_CURRENT | MCL_FUTURE);

    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    uint8_t pin_level = HIGH;

    while (true)
    {
        bcm2835_gpio_write(pin, pin_level);
        if (pin_level == HIGH) {
            pin_level = LOW;
        } else {
            pin_level = HIGH;
        }
        bcm2835_delay(ms);
    }

    bcm2835_close();

    return 0;   // ok
}
