#include <stdio.h>
#include <math.h>
#include "thread.h"
#include "msg.h"
#include "board.h"
#include "periph/uart.h"
#include "lptimer.h"
#include "periph/i2c.h"
#include "bmx280.h"
#include "opt3001.h"

#define RCV_QUEUE_SIZE (16) // must be a power of two

static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF];
static kernel_pid_t receiver_thread_pid;
static msg_t rcv_queue[RCV_QUEUE_SIZE];

static void *receiver_thread(void *arg);
static void uart_rx_callback(void *arg, uint8_t data);

uint8_t comm[13] = {0}; 

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

void bme280_get(uint8_t cycles)
{
  for (int i = 0; i < cycles; i++)
  {
    int t = bmx280_read_temperature(&dev_bmx280);
    int h = bme280_read_humidity(&dev_bmx280); 
    int p = bmx280_read_pressure(&dev_bmx280)/100; 

    printf("T = %i.%i C, H = %i.%i %%, P = %i mbar\r\n", t/100, t%100, h/100, h%100, p);
  }
}

void opt3001_get(uint8_t cycles)
{
  for (int i = 0; i < cycles; i++)
  {
    uint32_t res = opt3001_measure(&dev_opt3001, &measure_opt3001);

    if(res == 0) printf("lum = %lu lux\r\n", measure_opt3001.luminocity);
  }
}


int main(void)
{
  printf("uart read example\n\r");

  int res = uart_init(UART_STDIO_DEV, 115200, uart_rx_callback, NULL);
  if (res != UART_OK)
  {
    printf("uart initialize error\n\r");
    return -1;
  }

  if(opt3001_init(&dev_opt3001) == 0) puts("OPT3001 succesfully initialised!\r\n");
  if (bmx280_init(&dev_bmx280, &bme280_params) == BMX280_OK) puts("BME280 succesfully initialised!\r\n");

  receiver_thread_pid = thread_create(receiver_thread_stack,
                                      sizeof(receiver_thread_stack),
                                      (THREAD_PRIORITY_MAIN - 1),
                                      THREAD_CREATE_STACKTEST,
                                      receiver_thread,
                                      NULL,
                                      "receiver thread");

  return 0;
}

int crc_determ(void)
{
    int sum = 0;

    for(uint8_t i = 0; i < 11; i++)
    {
      sum += comm[i];
    }

    return sum;
}

static void *receiver_thread(void *arg)
{
  (void)arg;

  printf("receiver thread is starting\n\r");
  msg_init_queue(rcv_queue, RCV_QUEUE_SIZE);

  msg_t msg;

  while (1)
  {
    msg_receive(&msg);


    for(int i = 0; i < 11; i++)
    {
        printf("%02hX::", comm[i]);
    }
    printf("\n\r%d\n\r", crc_determ());
    
    if(comm[8] == 10)
    {
      opt3001_get(comm[10]);
      continue;
    } 

    if(comm[8] == 12)
    {
      bme280_get(comm[10]);
      continue;
    } 
  }

  return NULL;
}

static void uart_rx_callback(void *arg, uint8_t data)
{
  (void)arg;
  static int counter = 0;
  msg_t msg;

  comm[counter++] = data;

  
  if(counter == 13)
  {
    msg_send_int(&msg, receiver_thread_pid);
    counter = 0;
  }
}