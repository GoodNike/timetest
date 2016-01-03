#include <string.h>
#include <sched.h>
#include <sys/mman.h>

#include <bcm2835.h>

int main(int argc, char **argv)
{
    const uint8_t PIN = 4;

    if (!bcm2835_init()) {
        return 1;   // error
    }

    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

    uint8_t pin_level = HIGH;

    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sp);
    mlockall(MCL_CURRENT | MCL_FUTURE);

    while (true)
    {
        bcm2835_gpio_write(PIN, pin_level);
        if (pin_level == HIGH) {
            pin_level = LOW;
        } else {
            pin_level = HIGH;
        }
        bcm2835_delay(8);
    }

    bcm2835_close();

    return 0;   // ok
}
