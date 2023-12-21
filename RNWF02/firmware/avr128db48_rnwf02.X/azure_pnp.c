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

#ifdef AZURE_CLOUD

#include "azure_pnp.h"

static int16_t g_RebootDelay = -1;

static uint16_t g_ReportRate = APP_CLOUD_REPORT_INTERVAL;

/**Azure IoT HUB subscribe list */
static const char *subscribe_list[] = {AZURE_SUB_TWIN_RES, AZURE_SUB_METHODS_POST, AZURE_SUB_TWIN_PATCH,  NULL};

static uint8_t subCnt;



void APP_AZURE_BUTTON_Telemetry(uint32_t press_count)
{            
    snprintf(app_buf, sizeof(app_buf), AZURE_FMT_BUTTON_TEL, press_count);
    printf("[Telemetry] SW0 button press count = %d\r\n", press_count);
    APP_MQTT_Publish(AZURE_PUB_TELEMETRY, app_buf);
}


void APP_AZURE_COUNTER_Telemetry(uint32_t counter)
{            
    snprintf(app_buf, sizeof(app_buf), AZURE_FMT_COUNTER_TEL, counter);
    printf("[Telemetry] counter = %d\r\n", counter);
    APP_MQTT_Publish(AZURE_PUB_TELEMETRY, app_buf);
}

void APP_LED_STATE_Handler(APP_LED_STATE_t ledState)
{
    printf("LED State = %d\r\n", ledState);
    switch(ledState)
    {
        case APP_LED_BLINK:
            TCA1_Interface.Start();
            break;
        case APP_LED_OFF:
            TCA1_Interface.Stop();
            LED_SetHigh();
            break;
        case APP_LED_ON:
            TCA1_Interface.Stop();
            LED_SetLow();
            break;
    }   
}

void APP_AZURE_SUBACK_Handler()
{    
    if(subscribe_list[subCnt] != NULL)
    {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(RNWF_MQTT_SUBSCRIBE_QOS0, app_buf);            
    }
    else
    {        
        // get device twin
        APP_MQTT_Publish(AZURE_PUB_TWIN_GET, "");
    }
}

void APP_AZURE_Task(void)
{       
    static uint32_t press_count = 0;
    static uint32_t counter = 0;
    
    if(!(g_SysTickCount % APP_SYS_TICK_COUNT_1SEC))
    {            
        if(!(g_SysTickCount % g_ReportRate))
        {
            APP_AZURE_COUNTER_Telemetry(counter++);
        }
                                           
        if(g_RebootDelay > 0)
        {            
            printf("Rebooting in %d seconds..\r\n", g_RebootDelay);
            g_RebootDelay--;
            if(g_RebootDelay == 0)
                APP_RESET_Device();            
        }                    
    }  
    
    if(g_ButtonPress)
    {        
        APP_AZURE_BUTTON_Telemetry(++press_count);
        g_ButtonPress = 0;
    }
         
    
    if(!subCnt && subscribe_list[subCnt] != NULL)
    {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(RNWF_MQTT_SUBSCRIBE_QOS0, app_buf);            
    }        
                 
}


void APP_AZURE_SUB_Handler(char *p_str)
{
    char *end_ptr = NULL;
    const char *ver_ptr = NULL, *led_ptr = NULL, *rate_ptr = NULL;
    if(strstr(p_str, "twin/res/200"))
    {
        sprintf(app_buf, "{"AZURE_MSG_IPADDRESS"}", APP_GET_IP_Address());
        APP_MQTT_Publish(AZURE_PUB_PROPERTY, app_buf);

    }                        

    if(strstr(p_str, "POST"))
    {       
        char *reboot_ptr = (char *)strstr(p_str, AZURE_DEALY_TAG);
        char *echo_ptr = (char *)strstr(p_str, AZURE_ECHO_TAG);   
        char *rid_ptr = (char *)strstr(p_str, "rid="); 
        if(rid_ptr != NULL)
        {
            end_ptr = (char *)strstr(rid_ptr, "\" \"");
            *end_ptr = '\0';
            //+1 for null character
            uint16_t pubLen = sprintf(app_buf, AZURE_PUB_CMD_RESP, rid_ptr) + 1;             
            if(reboot_ptr != NULL)
            {                 
                end_ptr = (char *)strstr(reboot_ptr, "S\"}");
                *end_ptr = '\0';
                g_RebootDelay = atoi((char *)reboot_ptr+strlen(AZURE_DEALY_TAG));
                printf("Reboot delay = %d mSec\r\n", g_RebootDelay);
                sprintf(app_buf+pubLen, AZURE_FMT_DELAY_RSP, g_RebootDelay);                
                APP_MQTT_Publish(app_buf, app_buf+pubLen);
            }
            if(echo_ptr != NULL)
            {                 
                echo_ptr += strlen(AZURE_ECHO_TAG);
                end_ptr = (char *)strstr(echo_ptr, "\\\"}");
                *end_ptr = '\0';            
                printf("Echo = %s\r\n", echo_ptr);
                sprintf(app_buf+pubLen, AZURE_FMT_ECHO_RSP, echo_ptr);                
                APP_MQTT_Publish(app_buf, app_buf+pubLen);
            }
        }                
    }  
    
    if(strstr(p_str, "desired"))
    {                
        ver_ptr = strstr(p_str, AZURE_VERSION_TAG);                
        led_ptr = strstr(p_str, AZURE_LED0_TAG);  
        rate_ptr = strstr(p_str, AZURE_RATE_TAG);
        
        if(ver_ptr != NULL)
        {
            ver_ptr += +strlen(AZURE_VERSION_TAG); 
            end_ptr = (char *)strstr(ver_ptr, "}");
            *end_ptr = '\0';
            if(led_ptr != NULL)
            {
                led_ptr += strlen(AZURE_LED0_TAG);
                end_ptr = (char *)strstr(led_ptr, " \\");
                *end_ptr = '\0';
                sprintf(app_buf, AZURE_FMT_LED0_PROP, ver_ptr, led_ptr);                
                APP_LED_STATE_Handler(atoi(led_ptr));                
                APP_MQTT_Publish(AZURE_PUB_PROPERTY, app_buf);
            }
            if(rate_ptr != NULL)
            {
                rate_ptr += strlen(AZURE_RATE_TAG);
                end_ptr = (char *)strstr(rate_ptr, " \\");
                *end_ptr = '\0';
                sprintf(app_buf, AZURE_FMT_RATE_PROP, ver_ptr, rate_ptr);
                g_ReportRate = atoi(rate_ptr) * APP_SYS_TICK_COUNT_1SEC;
                printf("Report Rate = %d msec\r\n", g_ReportRate);
                APP_MQTT_Publish(AZURE_PUB_PROPERTY, app_buf);
            }
        }                
    }    
}
#endif /* AZURE_CLOUD */
