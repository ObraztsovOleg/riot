/*
 * Copyright (C) 2016-2018 Unwired Devices
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     drivers_m24srxx
 * @{
 *
 * @file
 * @brief       Definition for the M24SRxx NFC memory
 * 
 * @author      Alexander Ugorelov <alex_u@unwds.com>
 *
 * @}
 */
 
#ifndef _M24SR_INTERNAL_H
#define _M24SR_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief M24SR status code
 * 
 * @{
 */
#define M24SR_ACTION_COMPLETED                              0x9000
#define UB_STATUS_OFFSET                                    4
#define LB_STATUS_OFFSET                                    3

#define M24SR_NUM_BYTE_INVALID                              0xFFFE
/**
 * @}
 */

/**
 * @brief M24SR file indetifier
 * 
 * @{
 */
#define M24SR_SYS_FILE_ID                                   0xE101   
#define M24SR_CC_FILE_ID                                    0xE103
#define M24SR_NDEF_FILE_ID                                  0x0001   
/**
 * @}
 */

/**
 * @brief M24SR NDEF properties
 * 
 * @{
 */
#define NDEF_FILE_LEN_POS                                   0
#define NDEF_FILE_LEN_NUM_BYTES                             2
/**
 * @}
 */


/**
 * @brief M24SR password management
 * 
 * @{
 */    
#define READ_PWD                                            0x0001
#define WRITE_PWD                                           0x0002
#define I2C_PWD                                             0x0003
/**
 * @}
 */


/**
 * @brief M24SR special command
 * 
 * @{
 */    
#define M24SR_OPEN_I2C_SESSION                              0x26
#define M24SR_KILL_RF_SESSION                               0x52
/**
 * @}
 */

/**
 * @brief M24SR APDU Command: 
 * 
 * @{
 */

/**
 * @brief Class List
 */
#define CLA_DEFAULT                                         0x00
#define CLA_STM                                             0xA2
 
/**
 * @brief  Data Area Management Commands
 */ 
#define INS_SELECT                                          0xA4
#define INS_GET_RESPONSE                                    0xC0
#define INS_UPDATE_BINARY                                   0xD6
#define INS_READ_BINARY                                     0xB0
#define INS_WRITE_BINARY                                    0xD0
#define INS_UPDATE_RECORD                                   0xDC
#define INS_READ_RECORD                                     0xB2

/**
 * @brief Safety Management Commands
 */
#define INS_VERIFY                                          0x20
#define INS_CHANGE_REF_DATA                                 0x24
#define INS_DISABLE_VERIFY_REQ                              0x26
#define INS_ENABLE_VERIFY_REQ                               0x28
/**
 * @}
 */


/**
 * @brief M24SR error code
 * 
 * @{
 */ 
#define SW_OK                                               0x9000
#define SW_FILE_OVERFLOW                                    0x6280
#define SW_END_OF_FILE                                      0x6282
#define SW_PASSWORD_REQUIRED                                0x6300
#define SW_PASSWORD_INCORRECT_2_RETRY                       0x63C2
#define SW_PASSWORD_INCORRECT_1_RETRY                       0x63C1
#define SW_PASSWORD_INCORRECT_0_RETRY                       0x63C0
#define SW_RF_SESSION_KILLED                                0x6500
#define SW_UNSUCCESSFUL_UPDATING                            0x6581
#define SW_WRONGH_LENGTH                                    0x6700
#define SW_COMMAND_INCORRECT                                0x6981
#define SW_SECURITY_STATUS                                  0x6982
#define SW_REFERENCE_DATA_NOT_USABLE                        0x6984
#define SW_INCORRECT_PARAMETER                              0x6A80
#define SW_FILE_NOT_FOUND                                   0x6A82
#define SW_FILE_OVERFLOW_LC                                 0x6A84
#define SW_INCORRECT_P1_P2                                  0x6A86
#define SW_INS_NOT_SUPPORTED                                0x6D00
#define SW_CLASS_NOT_SUPPORTED                              0x6E00
#define SW_FAULT                                            0x6F00
/**
 * @}
 */


/**
 * @brief M24SR Length answer packet
 * 
 * @{
 */ 
#define M24SR_STATUS_NUM_BYTE                               2
#define M24SR_CRC_NUM_BYTE                                  2
#define M24SR_STATUS_RESPONSE_NUM_BYTE                      5
#define M24SR_DESELECT_REQUEST_NUM_BYTE                     3
#define M24SR_DESELECT_RESPONSE_NUM_BYTE                    3
#define M24SR_WATING_TIME_EXT_RESPONSE_NUM_BYTE             4
/**
 * @}
 */

/**
 * @brief M24SR Length password
 * 
 * @{
 */    
#define M24SR_PASSWORD_NUM_BYTE                             0x10
/**
 * @}
 */

/**
 * @brief M24SR Command structure
 *
 * @{ 
 */

/**
 * @brief Command structure Mask
 */
#define M24SR_PCB_NEEDED                                    0x0001      /**< PCB byte present or not */
#define M24SR_CLA_NEEDED                                    0x0002      /**< CLA byte present or not */
#define M24SR_INS_NEEDED                                    0x0004      /**< Operation code present or not*/ 
#define M24SR_P1_NEEDED                                     0x0008      /**< Selection Mode  present or not*/
#define M24SR_P2_NEEDED                                     0x0010      /**< Selection Option present or not*/
#define M24SR_LC_NEEDED                                     0x0020      /**< Data field length byte present or not */
#define M24SR_DATA_NEEDED                                   0x0040      /**< Data present or not */
#define M24SR_LE_NEEDED                                     0x0080      /**< Expected length present or not */
#define M24SR_CRC_NEEDED                                    0x0100      /**< 2 CRC bytes present  or not */
#define M24SR_DEV_ID_NEEDED                                 0x08        /**< Device ID byte present or not */

/**
 * @brief Command structure
 */
#define M24SR_CMD_CATEGORY_0                            (M24SR_PCB_NEEDED  | \
                                                        M24SR_CLA_NEEDED  | \
                                                        M24SR_INS_NEEDED  | \
                                                        M24SR_P1_NEEDED   | \
                                                        M24SR_P2_NEEDED   | \
                                                        M24SR_LC_NEEDED   | \
                                                        M24SR_DATA_NEEDED | \
                                                        M24SR_LE_NEEDED   | \
                                                        M24SR_CRC_NEEDED)

#define M24SR_CMD_CATEGORY_1                            (M24SR_PCB_NEEDED  | \
                                                        M24SR_CLA_NEEDED  | \
                                                        M24SR_INS_NEEDED  | \
                                                        M24SR_P1_NEEDED   | \
                                                        M24SR_P2_NEEDED   | \
                                                        M24SR_LC_NEEDED   | \
                                                        M24SR_DATA_NEEDED | \
                                                        M24SR_CRC_NEEDED)

#define M24SR_CMD_CATEGORY_2                            (M24SR_PCB_NEEDED  | \
                                                        M24SR_CLA_NEEDED  | \
                                                        M24SR_INS_NEEDED  | \
                                                        M24SR_P1_NEEDED   | \
                                                        M24SR_P2_NEEDED   | \
                                                        M24SR_LE_NEEDED   | \
                                                        M24SR_CRC_NEEDED)

#define M24SR_CMD_CATEGORY_3                            (M24SR_PCB_NEEDED  | \
                                                        M24SR_CLA_NEEDED  | \
                                                        M24SR_INS_NEEDED  | \
                                                        M24SR_P1_NEEDED   | \
                                                        M24SR_P2_NEEDED   | \
                                                        M24SR_LC_NEEDED   | \
                                                        M24SR_CRC_NEEDED)

#define M24SR_CMD_CATEGORY_4                            (M24SR_PCB_NEEDED  | \
                                                        M24SR_CLA_NEEDED  | \
                                                        M24SR_INS_NEEDED  | \
                                                        M24SR_P1_NEEDED   | \
                                                        M24SR_P2_NEEDED   | \
                                                        M24SR_CRC_NEEDED)

/**
 * @}
 */

/**
 *  @brief M24SR Command Offset
 *  
 *  @{
 */
#define M24SR_OFFSET_PCB                                    0
#define M24SR_OFFSET_CLASS                                  1
#define M24SR_OFFSET_INS                                    2
#define M24SR_OFFSET_P1                                     3
#define M24SR_OFFSET_P2                                     4
/**
 * @}
 */

/**
 * @brief M24SR Mask of various types of blocks
 *
 * @{ 
 */
#define M24SR_MASK_BLOCK                                    0xC0
#define M24SR_MASK_IBLOCK                                   0x00
#define M24SR_MASK_RBLOCK                                   0x80
#define M24SR_MASK_SBLOCK                                   0xC0
/**
 * @}
 */



/**
 * @brief M24SR Properties
 *
 * @{ 
 */
#define M24SR_I2C_TIMEOUT               200     /**< I2C Time out (ms), this is the maximum time needed by M24SR to complete any command */
#define M24SR_I2C_POLLING               1       /**< In case M24SR will reply ACK failed allow to perform retry before returning error*/
#define M24SR_ADDR                      0xAC    /**< M24SR address */
/* If both of this two flags are disabled, then the I2C polling will be used */
#define I2C_GPO_INTERRUPT_ALLOWED       (1)     /**< allow tu use GPO interrupt as I2C synchronization */
//#define I2C_GPO_SYNCHRO_ALLOWED         (1)     /**< allow tu use GPO polling as I2C synchronization */
#define RFU                             (1)     /**< Reserved  for Future  Use*/
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _M24SR_INTERNAL_H */