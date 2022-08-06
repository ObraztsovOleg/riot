/*
 * Copyright 2015 Ludwig Knüpfer
 *           2015 Christian Mehlis
 *           2016-2017 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_dht
 * @{
 *
 * @file
 * @brief       Device driver implementation for the DHT 11 and 22
 *              temperature and humidity sensor
 *
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 * @author      Christian Mehlis <mehlis@inf.fu-berlin.de>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdint.h>
#include <string.h>

#include "log.h"
#include "assert.h"
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#include "dht.h"
#include "dht_params.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

#define PULSE_WIDTH_THRESHOLD       (40U)
#define DHT_SYNC         1
#define DHT_SYNC_UP      1

bool sync = false, get_value = false;


static void high_lev_handler(void * arg)
{

    msg_t msg;
    
    int pid = (int) arg;
    gpio_irq_disable(UNWD_GPIO_7);
    
    msg.type = DHT_SYNC;
    msg_send(&msg, pid);
    
    //puts("irq!\n");
}


static inline int wait_for(gpio_t pin, int arg){
    msg_t msg_req;
    gpio_init(pin, GPIO_IN_PU);
    
    if((arg && gpio_read(pin)) || (!arg && !gpio_read(pin))){
        //puts("Done waiting!/n");
    	return 1;
    }
    
    gpio_init_int(pin,
		  GPIO_IN_PU,
		  arg ? GPIO_RISING : GPIO_FALLING,
	          high_lev_handler,
                  (void*) (int) thread_getpid());
                  
    msg_receive(&msg_req);
    gpio_init(pin, GPIO_IN_PU);
    if(msg_req.type == DHT_SYNC){
	//puts("Done waiting in interrupt!/n");
    	return 1;
    }
    
    return 0;
}

static uint16_t read(gpio_t pin, int bits)
{
    uint16_t res = 0;

    for (int i = 0; i < bits; i++) {
        uint32_t start, end;
        res <<= 1;
        /* measure the length between the next rising and falling flanks (the
         * time the pin is high - smoke up :-) */
        wait_for(pin, 1);
        start = xtimer_now_usec();
        wait_for(pin, 0);
        end = xtimer_now_usec();
        /* if the high phase was more than 40us, we got a 1 */
        if ((end - start) > PULSE_WIDTH_THRESHOLD) {
            res |= 0x0001;
        }
    }
    return res;
}


int dht_init(dht_t *dev, const dht_params_t *params)
{
    DEBUG("dht_init\n");
    
                  

    /* check parameters and configuration */
    assert(dev && params &&
           ((params->type == DHT11) || (params->type == DHT22) || (params->type == DHT21)));

    *dev = *params;

    gpio_init(dev->pin, GPIO_OUT);
    gpio_set(dev->pin);

    xtimer_usleep(2000 * US_PER_MS);

    DEBUG("dht_init: success\n");
    return DHT_OK;
}

int dht_read(const dht_t *dev, int16_t *temp, int16_t *hum)
{
    uint8_t csum, sum;
    uint16_t raw_hum, raw_temp;
    

    assert(dev && temp && hum);

    /* send init signal to device */
    gpio_clear(dev->pin);
    xtimer_usleep(20 * US_PER_MS);
    gpio_set(dev->pin);
    xtimer_usleep(40);
    
    gpio_init(dev->pin, dev->in_mode);
    
    //while (!gpio_read(dev->pin)) {}
    wait_for(dev->pin, 1);
    wait_for(dev->pin, 0);
    
     

	    /*
	     * data is read in sequentially, highest bit first:
	     *  40 .. 24  23   ..   8  7  ..  0
	     * [humidity][temperature][checksum]
	     */

	    /* read the humidity, temperature, and checksum bits */
	    raw_hum = read(dev->pin, 16);
	    raw_temp = read(dev->pin, 16);
	    csum = (uint8_t)read(dev->pin, 8);

	    /* set pin high again - so we can trigger the next reading by pulling it low
	     * again */
	    gpio_init(dev->pin, GPIO_OUT);
	    gpio_set(dev->pin);

	    /* validate the checksum */
	    sum = (raw_temp >> 8) + (raw_temp & 0xff) + (raw_hum >> 8) + (raw_hum & 0xff);
	    if ((sum != csum) || (csum == 0)) {
		DEBUG("error: checksum invalid\n");
		return DHT_NOCSUM;
	    }

	    /* parse the RAW values */
	    //DEBUG("RAW values: temp: %7i hum: %7i\n\r", (int)raw_temp, (int)raw_hum);
	    switch (dev->type) {
		case DHT11:
		    *temp = (int16_t)((raw_temp >> 8) * 10);
		    *hum = (int16_t)((raw_hum >> 8) * 10);
		    break;
		case DHT22:
		    *hum = (int16_t)raw_hum;
		    /* if the high-bit is set, the value is negative */
		    if (raw_temp & 0x8000) {
		        *temp = (int16_t)((raw_temp & ~0x8000) * -1);
		    }
		    else {
		        *temp = (int16_t)raw_temp;
		    }
		    break;
		default:
		    return DHT_NODEV;      /* this should never be reached */
	    }
	    
	    sync = false;
	    get_value = false;

	    return DHT_OK;
}
