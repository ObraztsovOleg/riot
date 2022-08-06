#include <stdio.h>

#include "lptimer.h"
#include "periph/i2c.h"
#include "bmx280.h"
#include "opt3001.h"

static char lum_stack[THREAD_STACKSIZE_DEFAULT];

static opt3001_t dev_opt3001 = {
    .i2c = I2C_DEV(1)
};
static opt3001_measure_t measure_opt3001;

static bmx280_t dev_bmx280;
static bmx280_params_t bme280_params = {
    .i2c_dev = I2C_DEV(1),
    .i2c_addr = 0x76,
    .t_sb = BMX280_SB_0_5,
    .filter = BMX280_FILTER_OFF,
    .run_mode = BMX280_MODE_FORCED,
    .temp_oversample = BMX280_OSRS_X1,
    .press_oversample = BMX280_OSRS_X1,
    .humid_oversample = BMX280_OSRS_X1,
};

void *lum_thread(void *arg) {
    (void) arg;
    
    if (opt3001_init(&dev_opt3001) == 0) {
        puts("OPT3001 succesfully initialised!\r");
        
        while(1){
            uint32_t res = opt3001_measure(&dev_opt3001, &measure_opt3001);

            if(res == 0){
                printf("lum = %lu lux\r\n", measure_opt3001.luminocity);
            }
            lptimer_sleep(1000);
        }
    }

    return NULL;
}

int main(void) {
    /* Start another thread for luminocity measurements */
    thread_create(lum_stack, sizeof(lum_stack),
              THREAD_PRIORITY_MAIN+1,
              THREAD_CREATE_STACKTEST,
              lum_thread,
              NULL,
              "Luminoсity");

    if (bmx280_init(&dev_bmx280, &bme280_params) == BMX280_OK) {
        puts("BME280 succesfully initialised!\r");
        
        while(1){
            int t = bmx280_read_temperature(&dev_bmx280); /* degrees C * 100 */
            int h = bme280_read_humidity(&dev_bmx280); /* percents * 100 */
            int p = bmx280_read_pressure(&dev_bmx280)/100; /* Pa -> mbar */

            printf("T = %i.%i C, H = %i.%i %%, P = %i mbar\r\n", t/100, t%100, h/100, h%100, p);
            
            lptimer_sleep(10000);
        }
    }

    return 0;
}
