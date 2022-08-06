/*
 * Copyright (C) 2015-2018 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup         cpu_nrf5x_common
 * @{
 *
 * @file
 * @brief           nRF5x common definitions for handling peripherals
 *
 * @author          Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author          Manchenko Oleg <man4enkoos@gmail.com>
 */

#ifndef PERIPH_CPU_COMMON_H
#define PERIPH_CPU_COMMON_H

#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Power management configuration
 * @{
 */
#define PROVIDES_PM_LAYERED_OFF
#define PM_NUM_MODES    3
/** @} */

/**
 * @brief   Starting offset of CPU_ID
 */
#define CPUID_ADDR          (&NRF_FICR->DEVICEID[0])
/**
 * @brief   Length of the CPU_ID in octets
 */
#define CPUID_LEN           (8U)

/**
 * @name    Override macro for defining GPIO pins
 *
 * The port definition is used (and zeroed) to suppress compiler warnings
 */
#ifdef CPU_MODEL_NRF52840XXAA
#define GPIO_PIN(x,y)       ((x << 5) | y)
#else
#define GPIO_PIN(x,y)       ((x & 0) | y)
#endif

/**
 * @brief   Override GPIO_UNDEF value
 */
#define GPIO_UNDEF          (UINT_MAX)

/**
 * @brief   Generate GPIO mode bitfields
 *
 * We use 9 bit to encode the pin mode:
 * - bit   0: Pin direction
 * - bit   1: Connect or disconnect input buffer
 * - bit 2-3: Pull configuration
 * - bit 8-10: Drive configuration
 * - bit 16-17: Pin sensing mechanism
 */
#define GPIO_MODE(dir, input, pull, drive, sense)  ((dir   << GPIO_PIN_CNF_DIR_Pos)   | \
                                                    (input << GPIO_PIN_CNF_INPUT_Pos) | \
                                                    (pull  << GPIO_PIN_CNF_PULL_Pos)  | \
                                                    (drive << GPIO_PIN_CNF_DRIVE_Pos) | \
                                                    (sense << GPIO_PIN_CNF_SENSE_Pos) )

/**
 * @brief   No support for HW chip select...
 */
#define SPI_HWCS(x)         (SPI_CS_UNDEF)

/**
 * @brief   Declare needed shared SPI functions
 * @{
 */
#define PERIPH_SPI_NEEDS_INIT_CS
#define PERIPH_SPI_NEEDS_TRANSFER_BYTE
#define PERIPH_SPI_NEEDS_TRANSFER_REG
#define PERIPH_SPI_NEEDS_TRANSFER_REGS
/** @} */

#ifndef DOXYGEN
/**
 * @brief   Override GPIO modes
 *
 * We use 9 bit to encode the pin mode:
 * - bit   0: Pin direction
 * - bit   1: Connect or disconnect input buffer
 * - bit 2-3: Pull configuration
 * - bit 8-10: Drive configuration
 * - bit 16-17: Pin sensing mechanism
 * @{
 */
#define HAVE_GPIO_MODE_T

typedef enum {
    GPIO_IN    = GPIO_MODE(GPIO_PIN_CNF_DIR_Input, GPIO_PIN_CNF_INPUT_Connect, GPIO_PIN_CNF_PULL_Disabled, GPIO_PIN_CNF_DRIVE_S0S1, GPIO_PIN_CNF_SENSE_Disabled),     /**< Configure as input without pull resistor */
    GPIO_IN_PD = GPIO_MODE(GPIO_PIN_CNF_DIR_Input, GPIO_PIN_CNF_INPUT_Connect, GPIO_PIN_CNF_PULL_Pulldown, GPIO_PIN_CNF_DRIVE_S0S1, GPIO_PIN_CNF_SENSE_Disabled),     /**< Configure as input with pull-down resistor */
    GPIO_IN_PU = GPIO_MODE(GPIO_PIN_CNF_DIR_Input, GPIO_PIN_CNF_INPUT_Connect, GPIO_PIN_CNF_PULL_Pullup, GPIO_PIN_CNF_DRIVE_S0S1, GPIO_PIN_CNF_SENSE_Disabled),       /**< Configure as input with pull-up resistor */
    GPIO_OUT   = GPIO_MODE(GPIO_PIN_CNF_DIR_Output, GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_PULL_Disabled, GPIO_PIN_CNF_DRIVE_S0S1, GPIO_PIN_CNF_SENSE_Disabled), /**< Configure as output in push-pull mode */
    GPIO_OD    = GPIO_MODE(GPIO_PIN_CNF_DIR_Output, GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_PULL_Disabled, GPIO_PIN_CNF_DRIVE_S0D1, GPIO_PIN_CNF_SENSE_Disabled), /**< Configure as output in open-drain mode without pull resistor */
    GPIO_OD_PU = GPIO_MODE(GPIO_PIN_CNF_DIR_Output, GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_PULL_Pulldown, GPIO_PIN_CNF_DRIVE_S0D1, GPIO_PIN_CNF_SENSE_Disabled), /**< Configure as output in open-drain mode with pull resistor enabled */
    GPIO_AIN   = GPIO_MODE(GPIO_PIN_CNF_DIR_Input, GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_PULL_Disabled, GPIO_PIN_CNF_DRIVE_S0S1, GPIO_PIN_CNF_SENSE_Disabled)   /**< Configure as analog input */
} gpio_mode_t;
/** @} */

/**
 * @brief   Override GPIO active flank values
 * @{
 */
#define HAVE_GPIO_FLANK_T
typedef enum {
    GPIO_FALLING = 2,       /**< emit interrupt on falling flank */
    GPIO_RISING  = 1,       /**< emit interrupt on rising flank */
    GPIO_BOTH    = 3        /**< emit interrupt on both flanks */
} gpio_flank_t;
/** @} */
#endif /* ndef DOXYGEN */

/**
 * @brief   Timer configuration options
 */
typedef struct {
    NRF_TIMER_Type *dev;    /**< timer device */
    uint8_t channels;       /**< number of channels available */
    uint8_t bitmode;        /**< counter width */
    uint8_t irqn;           /**< IRQ number of the timer device */
} timer_conf_t;

/**
 * @brief   Override SPI mode values
 * @{
 */
#define HAVE_SPI_MODE_T
typedef enum {
    SPI_MODE_0 = 0,                                             /**< CPOL=0, CPHA=0 */
    SPI_MODE_1 = SPI_CONFIG_CPHA_Msk,                           /**< CPOL=0, CPHA=1 */
    SPI_MODE_2 = SPI_CONFIG_CPOL_Msk,                           /**< CPOL=1, CPHA=0 */
    SPI_MODE_3 = (SPI_CONFIG_CPOL_Msk | SPI_CONFIG_CPHA_Msk)    /**< CPOL=1, CPHA=1 */
} spi_mode_t;
/** @} */

/**
 * @brief   Override SPI clock values
 * @{
 */
#define HAVE_SPI_CLK_T
typedef enum {
    SPI_CLK_100KHZ = SPI_FREQUENCY_FREQUENCY_K125,  /**< 100KHz */
    SPI_CLK_400KHZ = SPI_FREQUENCY_FREQUENCY_K500,  /**< 400KHz */
    SPI_CLK_1MHZ   = SPI_FREQUENCY_FREQUENCY_M1,    /**< 1MHz */
    SPI_CLK_5MHZ   = SPI_FREQUENCY_FREQUENCY_M4,    /**< 5MHz */
    SPI_CLK_10MHZ  = SPI_FREQUENCY_FREQUENCY_M8     /**< 10MHz */
} spi_clk_t;
/** @} */

/**
 * @brief  SPI configuration values
 */
typedef struct {
    NRF_SPI_Type *dev;  /**< SPI device used */
    uint8_t sclk;       /**< CLK pin */
    uint8_t mosi;       /**< MOSI pin */
    uint8_t miso;       /**< MISO pin */
} spi_conf_t;

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CPU_COMMON_H */
/** @} */
