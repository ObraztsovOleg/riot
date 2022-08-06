/*
 * Copyright (C) 2016-2018 Unwired Devices LLC <info@unwds.com>

 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * @defgroup
 * @ingroup
 * @brief
 * @{
 * @file		umdk-ibutton.h
 * @brief       umdk-ibutton driver module definitions
 * @author      Mikhail Perkov
 */
#ifndef UMDK_IBUTTON_H
#define UMDK_IBUTTON_H

#include "unwds-common.h"

#define UMDK_IBUTTON_STACK_SIZE 1024

#define	UMDK_IBUTTON_DEV UNWD_GPIO_4

#define UMDK_IBUTTON_READ_ROM 0x33
#define UMDK_IBUTTON_SKIP_ROM 0xCC
#define UMDK_IBUTTON_MATCH_ROM 0x55
#define UMDK_IBUTTON_SEARCH_ROM 0xF0

#define UMDK_IBUTTON_SIZE_ID 8

#define UMDK_IBUTTON_GRANTED_PERIOD_SEC 10

#define UMDK_IBUTTON_POLLING_PERIOD_MS 300

#define UMDK_IBUTTON_TIME_SWITCH_MS 1000

#define UMDK_IBUTTON_LED_GPIO  UNWD_GPIO_5

#define UNLIMIT 0xFFFF

#define CRC_RIGHT 1
#define CRC_WRONG 0

#define DEVICE_OK 1
#define DEVICE_ERROR 0

#define OK 1
#define ERROR 0

typedef enum {
	UMDK_IBUTTON_MSG_CMD = 0x00,
	UMDK_IBUTTON_MSG_DETECT = 0x01,
} umdk_ibutton_msg_t;

typedef enum {
	UMDK_IBUTTON_CMD_NONE = 0x00,
} umdk_ibutton_cmd_t;

void umdk_ibutton_init(uwnds_cb_t *event_callback);
bool umdk_ibutton_cmd(module_data_t *data, module_data_t *reply);

#endif /* UMDK_IBUTTON_H */
