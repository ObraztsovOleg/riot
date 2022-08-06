#include <stdio.h>

#include "lptimer.h"
#include "periph/i2c.h"
#include "st95.h"

static uint8_t buff_data[256] = {};

static uint8_t length_uid = 0;
static uint8_t uid_full[10];
static uint8_t sak = 0;

static st95_t dev_st95t;

static st95_params_t st95_params = {.iface = ST95_IFACE_UART,
                                .uart = 1, .baudrate = 57600,
                                .spi = 0, .cs_spi = UNWD_GPIO_4, 
                                .irq_in = UNWD_GPIO_25, .irq_out = UNWD_GPIO_26, 
                                .ssi_0 = UNWD_GPIO_6, .ssi_1 = GPIO_UNDEF,
                                .vcc = UNWD_GPIO_24 };

int main(void) {

    if (st95_init(&dev_st95t, &st95_params)==ST95_OK){
        puts("\r\nST95 succesfully initialised!\r\n");
    }

    st95_sleep(&dev_st95t);

    while (true) 
    {
        length_uid = 0;
        sak = 0;
        memset(uid_full, 0x00, sizeof(uid_full));

        if (st95_is_wake_up(&dev_st95t)!= ST95_WAKE_UP) continue;

        if(st95_get_uid(&dev_st95t, &length_uid, uid_full, &sak) != ST95_OK) 
        {
            st95_sleep(&dev_st95t);
            continue;
        }

        printf("UID: %d bytes\r\n", length_uid);
        for(uint32_t i = 0; i < length_uid; i++) {
            printf("%02X ", uid_full[i]);
        }
        printf("\r\n");

        if (st95_read_data(&dev_st95t, buff_data, sizeof(buff_data)) == ST95_ERROR) {
            puts("\r\nError while reading\r\n");
            continue;
        }

        printf("Data: ");
        // PRINTBUFF(buff_data, sizeof(buff_data));
        printf("NDEF: %d bytes\n", ((buff_data[0] << 8) | buff_data[1]));
        for(uint32_t i = 2; i < sizeof(buff_data); i++) {
            printf("%c", buff_data[i]);
        }
        printf("\n");       

        lptimer_sleep(100);
    }

    return 0;
}