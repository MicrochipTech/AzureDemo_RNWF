/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAIN_H
#define	MAIN_H

#include "mcc_generated_files/reset/rstctrl.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/timer/delay.h"

#include "mcc_generated_files/library/rnwf02/rnwf_interface.h"
#include "mcc_generated_files/library/rnwf02/rnwf_wifi_service.h"
#include "mcc_generated_files/library/rnwf02/rnwf_net_service.h"
#include "mcc_generated_files/library/rnwf02/rnwf_mqtt_service.h"
#include "mcc_generated_files/library/rnwf02/rnwf_system_service.h"



/* Wi-Fi Configuration */
//#define HOME_AP_SSID        "NETGEAR51"
//#define HOME_AP_PASSPHRASE  "melodicship232"
#define HOME_AP_SSID        "Verizon-SM-G965U-D1FB"
#define HOME_AP_PASSPHRASE  "dtpl684$"
#define HOME_AP_SECURITY    RNWF_WPA2

#define AZURE_CLOUD     1
//#define AWS_CLOUD     1

/* LED blink timer period for 200mSec delay */
#define APP_LED_TCA1_PER_TICK   0x3D08

/* System Tick timer tick for 1Sec */
#define APP_SYS_TICK_COUNT_1SEC 1000


/* APP Cloud Telemetry Rate in seconds */
#define APP_CLOUD_REPORT_INTERVAL  2 * APP_SYS_TICK_COUNT_1SEC

/* APP Switch long press delay in seconds */
#define APP_SW_LONG_PRESS_DELAY 3 * APP_SYS_TICK_COUNT_1SEC

/* Application generic purpose buffer size */
#define APP_BUFFER_SIZE_MAX     512

typedef enum {
    APP_SYS_INIT,
    APP_SYS_UP,
    APP_WIFI_INIT,
    APP_WIFI_DOWN,
    APP_WIFI_UP,
    APP_CLOUD_DOWN,
    APP_CLOUD_UP
} APP_STATE_t;

typedef enum {
    APP_LED_ON = 1,
    APP_LED_OFF,
    APP_LED_BLINK,
    APP_LED_MAX,
} APP_LED_STATE_t;

extern uint8_t app_buf[APP_BUFFER_SIZE_MAX];

extern uint32_t g_SysTickCount;

extern bool g_ButtonPress;


RNWF_RESULT_t APP_MQTT_Publish(const char *top, const char *msg);

void APP_RESET_Device(void);

uint8_t* APP_GET_IP_Address(void);

#endif	/* MAIN_H */

