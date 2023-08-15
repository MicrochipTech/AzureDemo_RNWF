/*
 * File:   rnwf_system_service.c
 * Author: c21415
 *
 * Created on March 13, 2023, 10:44 AM
 */

/* This section lists the other files that are included in this file.
 */
#include <stdio.h>
#include <string.h>

#include "rnwf_interface.h"
#include "rnwf_system_service.h"

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

RNWF_RESULT_t RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SERVICE_t request, void *input) {
    RNWF_RESULT_t result = RNWF_FAIL;
    switch(request)
    {
        case RNWF_SYSTEM_RESET:
        {
            uint8_t init_msg[128];
            result = RNWF_CMD_SEND_OK_WAIT(NULL, init_msg, RNWF_RESET_TARGET);
        }
        break;
        case RNWF_SYSTEM_ECHO_OFF:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_ECHO_OFF);
        }
        break;case RNWF_SYSTEM_GET_MAN_ID:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+GMI:", input, RNWF_GET_MAN_ID);
        }
        break;
        case RNWF_SYSTEM_SW_REV:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+GMR:", input, RNWF_GET_REV_ID);
        }
        break;
        case RNWF_SYSTEM_DEV_INFO:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+DI:", input, RNWF_GET_DEV_INFO);
        }
        break;
        case RNWF_SYSTEM_GET_CERT_LIST:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+FS:2,1,", input, RNWF_GET_CERT_LIST);
        }
        break;
        case RNWF_SYSTEM_GET_KEY_LIST:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+FS:2,2,", input, RNWF_GET_KEY_LIST);
        }
        break;
        case RNWF_SYSTEM_GET_TIME:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_GET_SYS_TIME);
        }
        break;
        case RNWF_SYSTEM_SET_TIME_UNIX:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SYS_TIME_UNIX, *(uint32_t*)input);
        }
        break;
        case RNWF_SYSTEM_SET_TIME_NTP:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SYS_TIME_NTP, *(uint32_t*)input);
        }
        break;
        case RNWF_SYSTEM_SET_TIME_STRING:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SYS_TIME_STRING, (uint8_t*)input);
        }
        break;
        case RWWF_SYSTEM_GET_WIFI_INFO:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+NETIFC:", input, RNWF_GET_WIFI_IFNO);
        }
        break;
        case RNWF_SYSTEM_SET_SNTP:
        {            
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SNTP_STATIC, input);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SNTP_URL, input);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SNTP_ENBL, input);
        }
        break;
        case RNWF_SYSTEM_GET_MQTT_INFO:
        {            
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+MQTTC:", input, RNWF_GET_MQTT_IFNO);
        }
        break;
        default:
            break;
    }
    return result;
}
/* *****************************************************************************
 End of File
 */
