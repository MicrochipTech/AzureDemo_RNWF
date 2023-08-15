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
#ifndef AWS_SHADOW_H
#define	AWS_SHADOW_H

#define AWS_DEVICE_ID           "rnwf02_dev_01"
#define AWS_PUB_TELEMETRY       AWS_DEVICE_ID"/sensors"
#define AWS_PUB_SHADOW_UPDATE   "$aws/things/"AWS_DEVICE_ID"/shadow/update"
#define AWS_SUB_SHADOW_UPDATE   "$aws/things/"AWS_DEVICE_ID"/shadow/update/#"

/* Properties and Telemetry reporting format */
#define AWS_FMT_TEMP_LIGHT      "{\\\"Temperature (C)\\\": %d,\\\"Light (lux)\\\":%d}"
#define AWS_FMT_SHADOW_LIGHT    "{\\\"state\\\":{\\\"reported\\\":{\\\"toggle\\\": %d}}}"

#define AWS_SHADOW_LED_TAG      "\\\"toggle\\\":"

void APP_AWS_Task(void);


void APP_AZURE_TEMP_LIGHT_Telemetry(uint32_t temperature, uint32_t light);

#endif	/* AWS_SHADOW_H */

