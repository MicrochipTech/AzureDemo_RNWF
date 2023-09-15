/*
 * File:   azure_pnp.c
 * Author: c21415
 *
 * Created on July 26, 2023, 4:52 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#ifdef AWS_CLOUD

#include "aws_shadow.h"

static uint16_t g_ReportRate = APP_CLOUD_REPORT_INTERVAL;

/**Azure IoT HUB subscribe list */
static const char *subscribe_list[] = {AWS_SUB_SHADOW_UPDATE, NULL};

static uint8_t subCnt;

static uint8_t g_Temperature;
static uint8_t g_Light;
  


void APP_AWS_TEMP_LIGHT_Telemetry(uint8_t temperature, uint8_t light)
{            
    snprintf(app_buf, sizeof(app_buf), AWS_FMT_TEMP_LIGHT, temperature, light);
    printf("Telemetry ->> Temperature %d & Light = %d\r\n", temperature, light);
    APP_MQTT_Publish(AWS_PUB_TELEMETRY, app_buf);
}

void APP_AWS_SUBACK_Handler()
{    
    if(subscribe_list[subCnt] != NULL)
    {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(RNWF_MQTT_SUBSCRIBE_QOS0, app_buf);            
    }
    else
    {                
        sprintf(app_buf, AWS_FMT_SHADOW_LIGHT, !LED_GetValue());
        APP_MQTT_Publish(AWS_PUB_SHADOW_UPDATE, app_buf);
    }
}

void APP_AWS_Task(void)
{           
    if(!(g_SysTickCount % APP_SYS_TICK_COUNT_1SEC))
    {            
        if(!(g_SysTickCount % g_ReportRate))
        {
            APP_AWS_TEMP_LIGHT_Telemetry(g_Temperature++, g_Light++);            
        }
                                                                       
    }  
                 
    if(!subCnt && subscribe_list[subCnt] != NULL)
    {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(RNWF_MQTT_SUBSCRIBE_QOS0, app_buf);            
    }        
                 
}


void APP_AWS_SUB_Handler(char *p_str)
{
    if(strstr(p_str, "shadow/update/delta"))
    {        
        bool led_state;
        char *end_ptr, *toggle_ptr = (char *)strstr(p_str, AWS_SHADOW_LED_TAG);             
        if(toggle_ptr != NULL)
        {
            toggle_ptr += strlen(AWS_SHADOW_LED_TAG);
            end_ptr = (char *)strstr(toggle_ptr, "} ");
            *end_ptr = '\0';
            led_state = atoi(toggle_ptr);
            sprintf(app_buf, AWS_FMT_SHADOW_LIGHT, led_state);                         
            APP_MQTT_Publish(AWS_PUB_SHADOW_UPDATE, app_buf);
            if(led_state)
                LED_SetLow();
            else
                LED_SetHigh(); 
        }                                                  
    }                        
}
#endif /* AZURE_CLOUD */
