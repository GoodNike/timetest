/**
 * @copyright   Николай Крашенинников
 * @project     Bottom project
 * @file        main.cpp
 * @brief       Программа для оценки точности выдерживания Raspberry Pi собственной дискретности.
 *
 * Программа циклично меняет состояние пина GPIO с HIGH на LOW и обратно. Пользователь задает номер
 * пина для выдачи, задает дискретность работы, мс, через командную строку. В случае успешного
 * запуска, программа не возвращает управление.
 * 
 * @note Нумерация пинов GPIO в соответствии с http://elinux.org/RPi_Low-level_peripherals#Model_A.2B.2C_B.2B_and_B2
 */

#include <iostream>
#include <cstdlib>

#include <string.h>
#include <sched.h>
#include <sys/mman.h>

#include <bcm2835.h>

/**
 * Константа, определяющая сколько по времени занимает вызов функции bcm2835_st_read().
 * Иными словами, bcm2835_st_read() вернет количество милесекунд с момента старта процессора, но в
 * момент возврата из функции время с момента старта процессора уже увеличится на константу
 * ST_READ_TIME. Мы будем на ST_READ_TIME милесекунд дальше во времени, чем показывает
 * bcm2835_st_read().
 * Данная константа вычисляется заранее.
 */
const uint64_t ST_READ_TIME = 1;

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
    ms *= 1000;

    if (!bcm2835_init()) {
        std::cout << "bcm2835_init() failure" << std::endl;
        return 1;   // error
    }

    /*
     * Даннный прием повышения точности выдерживания циклов для приложений взят из руководства к
     * библиотеке bcm2835 http://airspayce.com/mikem/bcm2835/index.html, раздел "Real Time
     * performance constraints".
     */
    sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sp);
    mlockall(MCL_CURRENT | MCL_FUTURE);

    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    uint8_t pin_level = HIGH;

    uint64_t st;
    while (true) {
        st = bcm2835_st_read();

        bcm2835_gpio_write(pin, pin_level);
        if (pin_level == HIGH) {
            pin_level = LOW;
        } else {
            pin_level = HIGH;
        }

        bcm2835_st_delay(st, ms + ST_READ_TIME);
    }

    bcm2835_close();

    return 0;   // ok
}
