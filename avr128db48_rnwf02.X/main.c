 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
? [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>



#include "main.h"
#include "azure_pnp.h"
#include "aws_shadow.h"

/*
    Main application
*/

static APP_STATE_t g_AppState = APP_SYS_INIT;

/* System Tick Counter for 1mSec*/
uint32_t g_SysTickCount;

/* Keeps the device IP address */
static char g_DevIp[16];

/* Button Press Event */
bool    g_ButtonPress = false;

/* Application buffer */
uint8_t app_buf[APP_BUFFER_SIZE_MAX];


#ifdef AWS_CLOUD

const char *tls_cfg_1[] = {"AmazonRootCA1", "aws", "aws", NULL, "a1gqt8sttiign3-ats.iot.us-east-2.amazonaws.com"};
//const char *tls_cfg_1[] = {"AmazonRootCA1", "aws_dev_01_cert", "aws_dev_01_key", NULL, NULL};
                       
RNWF_MQTT_CFG_t mqtt_cfg = {
    .url = "a1gqt8sttiign3-ats.iot.us-east-2.amazonaws.com", //MCU8
    .username = "",
    .password = "",
    .clientid = AWS_DEVICE_ID,
    .port = 8883,
    .tls_conf = tls_cfg_1,
    .tls_idx = RNWF_NET_TLS_CONFIG_1,   
    .azure_dps = 0,
};

#define CLOUD_STATE_MACHINE()         APP_AWS_Task()

#define CLOUD_SUBACK_HANDLER()        APP_AWS_SUBACK_Handler()

#define CLOUD_SUBMSG_HANDLER(msg)     APP_AWS_SUB_Handler(msg)

#elif AZURE_CLOUD

/**TLS Configuration for the DPS */
const char *tls_cfg_dps[] = {"BaltimoreCyberTrustRoot", AZURE_DEVICE_ID, AZURE_DEVICE_ID, NULL, "global.azure-devices-provisioning.net"};
/**TLS Configuration for the IoT HUB */
const char *tls_cfg_hub[] = {"DigiCertGlobalRootG2", AZURE_DEVICE_ID, AZURE_DEVICE_ID, NULL, "global.azure-devices.net"};
                       
RNWF_MQTT_CFG_t mqtt_cfg = {
    .url = "global.azure-devices-provisioning.net",        
    .username = AZURE_ID_SCOPE"/registrations/"AZURE_DEVICE_ID"/api-version=2019-03-31",    
    .clientid = AZURE_DEVICE_ID,    
    .password = "",
    .port = 8883,
    .tls_conf = tls_cfg_dps,
    .tls_idx = RNWF_NET_TLS_CONFIG_1,  
    .azure_dps = AZURE_DPS_ENABLE
};

#define CLOUD_STATE_MACHINE()         APP_AZURE_Task()

#define CLOUD_SUBACK_HANDLER()        APP_AZURE_SUBACK_Handler()

#define CLOUD_SUBMSG_HANDLER(msg)        APP_AZURE_SUB_Handler(msg)
#else

RNWF_MQTT_CFG_t mqtt_cfg = {
    .url = "test.mosquitto.org",        
    .clientid = "rnwf02",    
    .username = "",    
    .password = "",
    .port = 1883,    
    .tls_idx = 0,  
    .azure_dps = 0
};

#define CLOUD_STATE_MACHINE         APP_AZURE_Task

#define CLOUD_SUBACK_HANDLER        APP_AZURE_SUBACK_Handler

#define CLOUD_SUBMSG_HANDLER        APP_AZURE_SUB_Handler

#endif

uint8_t *APP_GET_IP_Address(void)
{
    return g_DevIp;
}


void APP_RESET_Device(void)
{        
    printf("Device is rebooting...!\r\n");
    DELAY_milliseconds(500);
    RSTCTRL_reset();
}

void APP_SW_Handler(void)
{
    static uint32_t press_ticks;    
    if(!IO_PB2_GetValue())
    {
        press_ticks = g_SysTickCount;
    }
    else
    {
        if((g_SysTickCount - press_ticks) > APP_SW_LONG_PRESS_DELAY)
        {
            APP_RESET_Device();
        }
        else
        {
            g_ButtonPress = 1;            
        }
    }    
}

void APP_SYS_Tick(void)
{       
    g_SysTickCount++;     
}

void APP_LED_Handler(void)
{ 
    LED_Toggle();    
}

RNWF_RESULT_t APP_MQTT_Publish(const char *top, const char *msg)
{    
    RNWF_MQTT_FRAME_t mqtt_pub;    
    mqtt_pub.isNew = NEW_MSG;
    mqtt_pub.qos = MQTT_QOS0;
    mqtt_pub.isRetain = NO_RETAIN;
    mqtt_pub.topic = top;
    mqtt_pub.message = msg;        
    return RNWF_MQTT_SrvCtrl(RNWF_MQTT_PUBLISH, (void *)&mqtt_pub);              
}        

RNWF_RESULT_t APP_MQTT_Callback(RNWF_MQTT_EVENT_t event, uint8_t *p_str)
{
    
    switch(event)
    {
        case RNWF_MQTT_CONNECTED:
        {                        
            g_AppState = APP_CLOUD_UP;
            TCA1_Interface.Stop();
            LED_SetLow();                          
        }
        break;
        case RNWF_MQTT_SUBCRIBE_ACK:
        {
            CLOUD_SUBACK_HANDLER();
        }
        break;
        case RNWF_MQTT_SUBCRIBE_MSG:
        {            
//            printf("RNWF02 <- %s\r\n", p_str);
            CLOUD_SUBMSG_HANDLER(p_str);            
        }
        break;
        case RNWF_MQTT_DISCONNECTED:
        {            
            TCA1_Interface.Start();
            printf("MQTT - Reconnecting...\r\n");
            g_AppState = APP_CLOUD_DOWN;            
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, NULL);            
        }
        break; 
#ifdef AZURE_CLOUD        
        case RNWF_MQTT_DPS_STATUS:
        {                           
            printf("DPS Status = %d\r\n", *p_str);
            if(*p_str == 1)
            {
                printf("DPS Successful! Connecting to Azure IoT Hub\r\n");
                RNWF_NET_SOCK_SrvCtrl(mqtt_cfg.tls_idx, (void *)tls_cfg_hub);                                        
            }
            else
            {   
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONFIG, (void *)&mqtt_cfg);                                                           
            }
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, NULL);                
        }
        break;
#endif        
        default:
        break;
    }
    return RNWF_PASS;
}


void APP_WIFI_Callback(RNWF_WIFI_EVENT_t event, uint8_t *p_str)
{       
    switch(event)
    {
        case RNWF_SNTP_UP:
        {            
            if(g_AppState < APP_CLOUD_DOWN)
            {            
                printf("SNTP UP:%s\n", &p_str[2]);  
                TCA1_Interface.PeriodCountSet(APP_LED_TCA1_PER_TICK<<1);                
                printf("Connecting to the Cloud\r\n");
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_SET_CALLBACK, APP_MQTT_Callback);
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONFIG, (void *)&mqtt_cfg);
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, NULL);
                g_AppState = APP_CLOUD_DOWN;
            }
        }
        break;
        case RNWF_CONNECTED:
        {
            printf("Wi-Fi Connected\n");  
            
        }    
        break;
        case RNWF_DISCONNECTED:  
            g_AppState = APP_WIFI_DOWN;
            TCA1_Interface.PeriodCountSet(APP_LED_TCA1_PER_TICK);
            TCA1_Interface.Start();
            printf("Wi-Fi Disconnected\nReconnecting... \n");
            RNWF_WIFI_SrvCtrl(RNWF_STA_CONNECT, NULL);
            break;
        case RNWF_DHCP_DONE:
        {
            printf("DHCP IP:%s\n", &p_str[2]); 
            strncpy(g_DevIp, &p_str[3], strlen(&p_str[3])-1);
            g_AppState = APP_WIFI_UP;              
        }
        break;
        case RNWF_SCAN_INDICATION:
            break;
        case RNWF_SCAN_DONE:
            break;
        default:
            break;
    }
}


int main(void)
{         
    
    SYSTEM_Initialize();

    PB2_SetInterruptHandler(APP_SW_Handler);
    TCA0_Interface.TimeoutCallbackRegister(APP_SYS_Tick);     
    TCA1_Interface.TimeoutCallbackRegister(APP_LED_Handler); 
            
    printf("%s", "##################################\n");
    printf("%s", "  Welcome RNWF02 Basic Cloud Demo  \n");
    printf("%s", "##################################\n");
        
    RNWF_IF_Init(); 
    
    RNWF_SYSTEM_SrvCtrl(RWWF_SYSTEM_GET_WIFI_INFO, app_buf);    
    printf("Wi-Fi Info:- \r\n%s\n", app_buf);
    
    
    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_GET_CERT_LIST, app_buf);    
    printf("Certs on RNWF02:- \r\n%s\n", app_buf);
    
    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_GET_KEY_LIST, app_buf);    
    printf("Keys on RNWF02:- \r\n%s\n", app_buf);
                                                                     
    const char sntp_url[] =  "0.in.pool.ntp.org";    
    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SET_SNTP, sntp_url);             
                
    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SW_REV, app_buf);    
    printf("%s\n", app_buf);
    
    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_DEV_INFO, app_buf);    
    printf("%s\n", app_buf);  
                    
    /* RNWF Application Callback register */
    RNWF_WIFI_SrvCtrl(RNWF_WIFI_SET_CALLBACK, APP_WIFI_Callback);
                
    /* Wi-Fii Connectivity */
    RNWF_WIFI_PARAM_t wifi_sta_cfg = {RNWF_WIFI_MODE_STA, HOME_AP_SSID, HOME_AP_PASSPHRASE, HOME_AP_SECURITY, 1};        
    RNWF_WIFI_SrvCtrl(RNWF_SET_WIFI_PARAMS, &wifi_sta_cfg);
            
    while(1)
    {          
        switch(g_AppState)
        {             
            
            case APP_SYS_INIT:
            case APP_SYS_UP:
            case APP_WIFI_INIT:
            {
                
            }
            break;
    
            case APP_WIFI_UP:
            {
                
            }
            break;
            
            case APP_CLOUD_UP:
            {                
                CLOUD_STATE_MACHINE();                                                              
            }
            break;
            
            default:                   
            {
                
            }
            break;
        }
                                                            
        RNWF_EVENT_Handler();            
    }    
}
