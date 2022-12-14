/*
 * Copyright (C) 2016 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_periph_pm Power Management
 * @ingroup     drivers_periph
 * @brief       The kernels power management interface
 *
 * @attention This interface *must* be implemented for every platform in RIOT.
 *
 * @{
 *
 * @file
 * @brief       Power management interface
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 */

#ifndef PERIPH_PM_H
#define PERIPH_PM_H

#include "assert.h"
#include "periph_cpu.h"

#ifdef MODULE_PM_LAYERED
#include "pm_layered.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Available power modes
 */
enum pm_mode {
    PM_OFF,            /**< MCU is off */
    PM_POWERDOWN,      /**< MCU is powered down */
    PM_SLEEP,          /**< MCU in sleep mode */
    PM_IDLE,           /**< MCU is idle */
    PM_ON,             /**< MCU is active */
    PM_UNKNOWN = -1    /**< status unknown/unavailable */
};

/**
 * @brief   Reboot MCU
 */
void pm_reboot(void);

/**
 * @brief   Turn off MCU completely
 */
void pm_off(void);

/**
 * @brief   Initialization of power management (including clock setup)
 *
 * This function is invoked once during boot.
 */
void pm_init(void);

/**
 * @brief   Switches the MCU to a new power mode
 * @param[in]   target      Target power mode
 * @return                  The previous power mode
 */
//enum pm_mode pm_set(enum pm_mode target);

/**
 * @brief   Returns the current power mode
 * @return  Current power mode
 */
enum pm_mode pm_get(void);

/**
 * @brief   Switches the MCU to the lowest possible power mode
 *
 * This function will be called by the idle thread.
 */
void pm_set_lowest(void);

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_PM_H */
/** @} */
