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
#ifndef AZURE_PNP_H
#define	AZURE_PNP_H


#define AZURE_DEVICE_ID         "rnwf02_device_01"//"<COMMON_NAME>"
#define AZURE_MODEL_ID          "dtmi:com:Microchip:AVR128DB48_CNANO;1"
#define AZURE_SCOPE_ID          "0ne000FFA42"//"0ne00AAE0D0"
#define AZURE_DPS_ENABLE        1

#define AZURE_PUB_TELEMETRY     "devices/"AZURE_DEVICE_ID"/messages/events/"
#define AZURE_PUB_PROPERTY      "$iothub/twin/PATCH/properties/reported/?$rid=1"
#define AZURE_PUB_TWIN_GET      "$iothub/twin/GET/?$rid=getTwin"
#define AZURE_PUB_CMD_RESP      "$iothub/methods/res/200/?$%s" 

#define AZURE_SUB_METHODS_POST  "$iothub/methods/POST/#"
#define AZURE_SUB_TWIN_PATCH    "$iothub/twin/PATCH/properties/desired/#"
#define AZURE_SUB_TWIN_RES      "$iothub/twin/res/#"


#define AZURE_MSG_LED0          "\\\"LED0\\\":%d"
#define AZURE_MSG_IPADDRESS     "\\\"ipAddress\\\":\\\"%s\\\""
#define AZURE_MSG_REP_RATE      "\\\"reportRate\\\":%d"


/* Properties and Telemetry reporting format */
#define AZURE_FMT_BUTTON_TEL    "{\\\"buttonEvent\\\": {\\\"button_name\\\":\\\"SW0\\\", \\\"press_count\\\":%d}"
#define AZURE_FMT_COUNTER_TEL   "{\\\"counter\\\": \\\"%d\\\"}"
#define AZURE_FMT_RATE_PROP     "{\\\"reportRate\\\":{\\\"ac\\\":200,\\\"av\\\":%s,\\\"ad\\\":\\\"success\\\",\\\"value\\\":%s}}"
#define AZURE_FMT_LED0_PROP     "{\\\"LED0\\\":{\\\"ac\\\":200,\\\"av\\\":%s,\\\"ad\\\":\\\"success\\\",\\\"value\\\":%s}}"
#define AZURE_FMT_DELAY_RSP     "{\\\"status\\\":\\\"Success\\\",\\\"delay\\\":%d}"
#define AZURE_FMT_ECHO_RSP      "{\\\"echoString\\\":\\\"%s\\\"}"

/* Parsing Tags for the Azure messages */
#define AZURE_VERSION_TAG       "\"$version\\\":"
#define AZURE_LED0_TAG          "\"LED0\\\":"
#define AZURE_RATE_TAG          "\"reportRate\\\":"
#define AZURE_DEALY_TAG         "\"delay\\\":\\\"PT"
#define AZURE_ECHO_TAG          "\"echoString\\\":\\\""

void APP_AZURE_Task(void);

#endif	/* AZURE_PNP_H */

