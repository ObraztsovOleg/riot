/*
 * Copyright (C) 2016-2018 Unwired Devices
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */
 
/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Test application for the M24SR NFC memory driver
 *
 * @author      Alexander Ugorelov <alex_u@unwds.com>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "board.h"

#include "xtimer.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#include "m24sr.h"
#include "m24sr_params.h"

/* This will only work on small memories. Modify if you need to test NVRAM
 * memories which do not fit inside free RAM */
static uint8_t buf_out[0x100];
static uint8_t buf_in[0X100];


/**
 * @brief xxd-like printing of a binary buffer
 */
static void print_buffer(const uint8_t * buf, size_t length) {
    static const unsigned int bytes_per_line = 16;
    static const unsigned int bytes_per_group = 16;
    unsigned long i = 0;
    while (i < length) {
        unsigned int col;
        for (col = 0; col < bytes_per_line; ++col) {
            /* Print hex data */
            if (col == 0) {
                printf("\n%08" PRIx32 ": ", i);
            }
            else if ((col % bytes_per_group) == 0) {
                putchar(' ');
            }
            if ((i + col) < length) {
                printf("%02x ", buf[i + col]);
            } else {
                putchar(' ');
                putchar(' ');
            }
        }
        putchar(' ');
        for (col = 0; col < bytes_per_line; ++col) {
            if ((i + col) < length) {
                /* Echo only printable chars */
                if (isprint(buf[i + col])) {
                    putchar(buf[i + col]);
                } else {
                    putchar('.');
                }
            } else {
                putchar(' ');
            }
        }
        i += bytes_per_line;
    }
    /* end with a newline */
    puts("\n");
}

/* weak PRNG for generating "random" test data */
static uint8_t lcg_rand8(void) {
    static const uint32_t a = 1103515245;
    static const uint32_t c = 12345;
    static uint32_t val = 123456; /* seed value */
    val = val * a + c;
    return (val >> 16) & 0xff;
}


int main(void)
{
    uint32_t i;
    uint32_t start_delay = 3;
    /* allocate device descriptor */
    static m24sr_t dev;

    m24sr_params_t m24sr_params = {
        .i2c = (I2C_DEV(0)), 
        .i2c_addr = (0x56),
        .gpo_pin = GPIO_PIN(PORT_A, 5),
        .rfdisable_pin = GPIO_UNDEF,
        .pwr_en_pin = GPIO_UNDEF,
        .priority = I2C_OPEN_SESSION,
        .token_mode = I2C_TOKEN_RELEASE_SW
    };

    puts("M24SR NFC memory driver test application\n");
    gpio_init(GPIO_PIN(PORT_B, 0), GPIO_OUT);


    puts("Initializing M24SR memory device descriptor... ");
    if (m24sr_eeprom_init(&dev, &m24sr_params) == 0) {
        puts("[OK]");
    }
    else {
        puts("[Failed]\n");
        return 1;
    }

    puts("M24SR memory init done.\n");

    puts("!!! This test will erase everything on the M24SR memory !!!");
    puts("!!! Unplug/reset/halt device now if this is not acceptable !!!");
    printf("\tWaiting for %ld seconds before continuing...\n", start_delay);
    xtimer_sleep(start_delay);

    puts("Writing blockwise to device");
    memset(buf_out, 0xff, sizeof(buf_out));
    if (m24sr_eeprom_write(&dev, buf_out, 0, dev.memory.chipsize) != dev.memory.chipsize) {
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("Reading back blockwise");
    memset(buf_in, 0x00, sizeof(buf_in));
    if (m24sr_eeprom_read(&dev, buf_in, 0, dev.memory.chipsize) != dev.memory.chipsize) {
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("Verifying contents...");
    if (memcmp(buf_in, buf_out, dev.memory.chipsize) != 0) {
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("Writing blockwise address complement to device");
    for (i = 0; i < dev.memory.chipsize; ++i) {
        buf_out[i] = (~(i)) & 0xff;
    }
    if (m24sr_eeprom_write(&dev, buf_out, 0, dev.memory.chipsize) != dev.memory.chipsize) {
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("buf_out:");
    print_buffer(buf_out, sizeof(buf_out));

    puts("Reading back blockwise");
    memset(buf_in, 0x00, sizeof(buf_in));
    if (m24sr_eeprom_read(&dev, buf_in, 0, dev.memory.chipsize) != dev.memory.chipsize) {
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("Verifying contents...");
    if (memcmp(buf_in, buf_out, dev.memory.chipsize) != 0) {
        puts("buf_in:");
        print_buffer(buf_in, sizeof(buf_in));
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("Generating pseudo-random test data...");

    for (i = 0; i < dev.memory.chipsize; ++i) {
        buf_out[i] = lcg_rand8();
    }

    puts("buf_out:");
    print_buffer(buf_out, sizeof(buf_out));

    puts("Writing blockwise data to device");
    if (m24sr_eeprom_write(&dev, buf_out, 0, dev.memory.chipsize) != dev.memory.chipsize) {
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("Reading back blockwise");
    memset(buf_in, 0x00, sizeof(buf_in));
    if (m24sr_eeprom_read(&dev, buf_in, 0, dev.memory.chipsize) != dev.memory.chipsize) {
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("Verifying contents...");
    if (memcmp(buf_in, buf_out, dev.memory.chipsize) != 0) {
        puts("buf_in:");
        print_buffer(buf_in, sizeof(buf_in));
        puts("[Failed]\n");
        return 1;
    }
    puts("[OK]");

    puts("All tests passed!");

    while (1) {
        gpio_toggle(GPIO_PIN(PORT_B, 0));
        xtimer_usleep(500000);
    }
    return 0;
}