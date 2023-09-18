/*******************************************************************************
 MPLAB Harmony Application Source File

 Company:
 Microchip Technology Inc.

 File Name:
 app_rio2.c

 Summary:
 This file contains the source code for the MPLAB Harmony application.

 Description:
 This file contains the source code for the MPLAB Harmony application.  It
 implements the logic of the application's state machine and it may call
 API routines of other MPLAB Harmony modules in the system, such as drivers,
 system services, and middleware.  However, it does not call any of the
 system interfaces (such as the "Initialize" and "Tasks" functions) of any of
 the modules in the system or make any assumptions about when those functions
 are called.  That is the responsibility of the configuration-specific system
 files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "mcc_generated_files/system/system.h"
#include "app_rio2_config.h"
#include "string.h"
#include "temperature.h"
#include <stdbool.h>
//#include "definitions.h"
//#include "config/default/peripheral/gpio/plib_gpio.h"                // SYS function prototypes

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

 Summary:
 Holds application data

 Description:
 This structure holds the application's data.

 Remarks:
 This structure should be initialized by the APP_RIO2_Initialize function.

 Application strings and buffers are be defined outside this structure.
 */

extern APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
/* TODO:  Add any necessary local functions.
 */
void APP_reset(void)
{
  reset();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
 Function:
 void APP_RIO2_Initialize ( void )

 Remarks:
 See prototype in app_rio2.h.
 */

void WriteTerminal(uint8_t* buffer, uint16_t length) {
    uint16_t i;
    for (i = 0; i < length; i++) {
        UART1_Write(buffer[i]);
    }


}

void WriteAnyCloud(uint8_t* buffer, uint16_t length) {
    uint16_t i;
    for (i = 0; i < length; i++) {
        UART2_Write(buffer[i]);
    }


}



char *dataBufferPtr;
char data_received = 0;
char test = 0;
bool readLineComplete = true;
char *linePtrBuffer;
uint16_t numberOfBytes;
char linearBuffer[1648];
float temperature;
uint16_t press_count = 0;

#define AnyCloud_IsRxReady UART2_IsRxReady
#define AnyCloud_Read UART2_Read

uint16_t GetNextLineUntil_CRLF(char *dataBuffer) {
    uint16_t count;

    if (!AnyCloud_IsRxReady())
        return 0;

    if (!linePtrBuffer) //
        linePtrBuffer = dataBuffer; //Initialize to buffer address

    do {
        uint8_t byte;

        if ((byte = AnyCloud_Read())) //Read one byte at a  time
        {

            *linePtrBuffer++ = byte;
            numberOfBytes++;
            if (numberOfBytes >= 2) //if I have read at least 2 bytes
            {
                //check if last 2 character are \n\r
                if ((*(linePtrBuffer - 1) == 0x0A)
                        && (*(linePtrBuffer - 2) == 0x0d)) {
                    //found CRLF
                    *linePtrBuffer = 0; // Terminate line with NULL
                    linePtrBuffer = NULL;
                    uint16_t bytes;
                    bytes = numberOfBytes;
                    numberOfBytes = 0;
                    return bytes;
                }
            }
        };
    } while (AnyCloud_IsRxReady());

    return 0;
}

bool gRDY, gOK, gATE, gGMM, gCFG, gTIME, gSOCKO, gSOCKCL, gWSTALU, gWSTALD, gDNSRESOLV,
gSOCKIND, gSOCKLST, gSOCKRXT, gSOCKRD, gSOCKRXU, gWSTAAIP, gWAPAIP,
gMQTTCONN, gMQTTPUB, gMQTTSUB, gMQTTSUBRX, gMQTTPUBACC, gMQTTPUBCOMP, gMQTTPUBERR,
gSOCKWR_Error, gSOCKID_Error, gWSCNDONE, gWSCNIND;

char *resultPtrRDY, *resultPtrOK, *resultPtrGMM, *resultPtrCFG,
*resultPtrTIME, *resultPtrSOCKO, *resultPtrSOCKCL, *resultPtrWSTALU,
*resultPtrWSTALD, *resultPtrDNSRESOLV, *resultPtrSOCKIND,
*resultPtrSOCKLST, *resultPtrSOCKRXT, *resultPtrSOCKRD,
*resultPtrSOCKRXU, *resultPtrWSTAAIP, *resultPtrWAPAIP,
*resultPtrMQTTCONN, *resultPtrMQTTPUB, *resultPtrMQTTSUB, *resultPtrMQTTSUBRX, *resultPtrMQTTPUBACC,
*resultPtrMQTTPUBCOMP, *resultPtrMQTTPUBERR, *resultPtrSOCKWR_Error,
*resultPtrWSCNDONE, *resultPtrWSCNIND;


//uint8_t announceResult[512];

/************************************************************/
/************************************************************/

/************************************************************/

typedef enum {
    CMD_RESET,

    CMD_ERROR,

    CMD_MODEL,

    CMD_MAC_ADDR_HOST_NAME,

    CMD_TIME,

    M2M_WIFI_CONNECTED,
    /*!< Wi-Fi state is connected.
     */
    M2M_WIFI_DISCONNECTED,
    /*!< Wi-Fi state is disconnected.
     */
    M2M_WIFI_REQ_DHCP_CLIENT,

    M2M_WIFI_REQ_DHCP_SERVER,

    M2M_WIFI_DNS_RESOLVE,

    M2M_WIFI_SCAN_RESULT,

    M2M_WIFI_SCAN_DONE,

    M2M_SOCKET_OPEN,

    M2M_SOCKET_RXTCP,

    M2M_SOCKET_RXUDP,

    M2M_SOCKET_READ,

    M2M_SOCKET_CONNECT,

    M2M_SOCKET_LIST,

    M2M_SOCKET_CLOSE,

    M2M_MQTT_CONNECTED,

    M2M_MQTT_PUBLISH,
            
    M2M_MQTT_SUBRX,

    M2M_MQTT_SUBSCRIBE,

    M2M_MQTT_PUBLISH_ACK_QOS1,

    M2M_MQTT_PUBLISH_COMPLETE_QOS2,

    M2M_MQTT_PUBLISH_ERROR,

    M2M_WIFI_UNDEF = 0xff
    /*!< Undefined Wi-Fi State.
     */
} event_type_enu;

uint8_t webServerIP[20];
static char remoteIP[20];

char scanResponse[100] = SCANRESPONSE;

typedef struct {
    char rssi[5]; //
    char mac[20]; //
    char ssid[40]; //
} SCAN_STRUCT; //  SCAN Struct descriptor
#define SCANSIZE 5
uint8_t scanIdx = 0;
SCAN_STRUCT scanArray[SCANSIZE];

static uint8_t MAC5[5], MAC4[5], MAC3[5], MAC2[5], MAC1[5], MAC0[5];

static bool ntpUpdated = false;
void atCmdRespHandler(uint8_t u8MsgType, void **pvMsg) {

    switch (u8MsgType) {
        case CMD_RESET:
        {
            printf_CB(("RESETING RIO2\r\n"));
            //if I received this RIO2 Banner,  this means the device had reset,
            //just re-initialize the state machine?
            app_rio2Data.state = APP_RIO2_STATE_WAIT_RST;
            break;
        }
        case CMD_TIME:
        {
            //+TIME:<TIME_FORMAT>,<TIME>
            // 1 UNIX timestamp
            // 2 NTP time
            // 3 RFC3338 / ISO - 8601 string format
            char *str;
            
                ntpUpdated = true;

            break;
        }

        case CMD_ERROR:
        {
            printf_CB(("ERROR  => %s\r\n", *pvMsg));
            if (strstr(*pvMsg, "2,")) //Invalid command
            {
                gRDY = false;
                //app_rio2Data.state = APP_RIO2_STATE_INIT;

            }
            if (strstr(*pvMsg, "17,")) //ERROR:17,"Socket Send Failed"
            {
                gSOCKWR_Error = true;
            }
            if (strstr(*pvMsg, "10,")) //ERROR:10,"Socket ID Not Found"
            {
                gSOCKID_Error = true;
            }

            break;
        }
        case CMD_MODEL:
        {
            printf_CB(("MODEL  is %s \r\n\r\n", *pvMsg));

            break;
        }
        case CMD_MAC_ADDR_HOST_NAME:
        {
            //AT+CFG
            //+CFG:1,"68:27:19:C3:48:2E"
            //+CFG:2,"PIC32MZW1-4c-14"
            if (strstr(*pvMsg, "1,")) //MAC Address
            {
                char *strStart;

                strStart = strstr(*pvMsg, "\"");
                if (strStart) {
                    printf_CB(("CFG   MACADDRESS %s ", strStart));
#if (1)
                    sscanf(strStart, "\"%x:%x:%x:%x:%x:%x\"\r\n", (unsigned int *) MAC5, (unsigned int *) MAC4, (unsigned int *) MAC3, (unsigned int *) MAC2, (unsigned int *) MAC1, (unsigned int *) MAC0);
#else
                    char *strEnd;
                    strEnd = strstr(++strStart, ":");
                    if (strEnd) {
                        *strEnd++ = 0;
                        printf_CB("MAC5=%s\r\n", strStart);
                        sprintf(MAC5, "%s", strStart);
                        strStart = strEnd;
                        strEnd = strstr(strStart, ":");
                        if (strEnd) {
                            *strEnd++ = 0;
                            printf_CB("MAC4=%s\r\n", strStart);
                            sprintf(MAC4, "%s", strStart);
                            strStart = strEnd;
                            strEnd = strstr(strStart, ":");
                            if (strEnd) {
                                *strEnd++ = 0;
                                printf_CB("MAC3=%s\r\n", strStart);
                                sprintf(MAC3, "%s", strStart);
                                strStart = strEnd;
                                strEnd = strstr(strStart, ":");
                                if (strEnd) {
                                    *strEnd++ = 0;
                                    printf_CB("MAC2=%s\r\n", strStart);
                                    sprintf(MAC2, "%s", strStart);
                                    strStart = strEnd;
                                    strEnd = strstr(strStart, ":");
                                    if (strEnd) {
                                        *strEnd++ = 0;
                                        printf_CB("MAC1=%s\r\n", strStart);
                                        sprintf(MAC1, "%s", strStart);
                                        strStart = strEnd;
                                        strEnd = strstr(strStart, "\"");
                                        if (strEnd) {
                                            *strEnd++ = 0;
                                            printf_CB("MAC0=%s\r\n", strStart);
                                            sprintf(MAC0, "%s", strStart);
                                            strStart = strEnd;
                                            strEnd = strstr(strStart, ":");

                                        }

                                    }

                                }

                            }

                        }

                    }
#endif

                }

            }
            if (strstr(*pvMsg, "2,")) //Host Name
            {
                printf_CB(("CFG   HOSTNAME %s ", strstr(*pvMsg, "\"")));
            }

            break;
        }
    }

}

void wifi_cb(uint8_t u8MsgType, void **pvMsg) {

    //printf("%s", __FUNCTION__);

    //strcpy(wifiResult, pvMsg);
    switch (u8MsgType) {
        case M2M_WIFI_CONNECTED:
        {
            printf_CB(("WSTALU  CONNECTED to AP %s\r\n", *pvMsg));
            break;
        }
        case M2M_WIFI_DISCONNECTED:
        {
            printf_CB(("WSTALD  DISCONNECTED\r\n"));
            gRDY = false;
            app_rio2Data.state = APP_RIO2_STATE_INIT;
            break;
        }
        case M2M_WIFI_REQ_DHCP_CLIENT:
        {
            printf_CB(("WSTAAIP  MY IP IS  %s\r\n", *pvMsg));
            break;
        }
        case M2M_WIFI_REQ_DHCP_SERVER:
        {
            printf_CB(("WAPAIP  REMOTE IP IS  %s\r\n", *pvMsg));
            break;
        }
        case M2M_WIFI_DNS_RESOLVE:
        {
            //+DNSRESOLV:0,"www.example.com","192.168.0.1"
            char *ptr;
            if ((ptr = strstr(*pvMsg, ","))) {
                if ((ptr = strstr(ptr + 1, ","))) {

                    strcpy(remoteIP, ptr + 2);
                    if ((ptr = strstr(remoteIP, "\""))) //Search closing "
                    {
                        *ptr = 0;

                    }

                };

            };
            printf_CB(("DNSRESOLV  Domain IP is %s\r\n", remoteIP));
            break;
        }
#define SCAN_CALLBACK
#ifdef SCAN_CALLBACK

        case M2M_WIFI_SCAN_RESULT:
        {
            char *strStart, *strEnd;
            gWSCNIND = false;
            if (scanIdx < SCANSIZE) {

                //+WSCNIND:-36,0,8,"1C:7E:E5:33:54:EE","TestingHotSpot"\r\n
                strStart = resultPtrWSCNIND;
                strEnd = strstr(resultPtrWSCNIND, ",");
                *strEnd++ = 0;
                strcpy(scanArray[scanIdx].rssi, strStart);

                strStart = strEnd;
                strEnd = strstr(strStart, ",");
                *strEnd++ = 0;

                strStart = strEnd;
                strEnd = strstr(strStart, ",");
                *strEnd++ = 0; // replace "," by \0
                *strEnd++ = 0; // replace double quote by \0

                strStart = strEnd;
                strEnd = strstr(strStart, ",");
                *(strEnd - 1) = 0; // replace end double quote by \0  of MAC
                *strEnd++ = 0; // replace "," by \0
                *strEnd++ = 0; // replace start double quote by \0  of SSID
                strcpy(scanArray[scanIdx].mac, strStart);

                strStart = strEnd;
                strEnd = strstr(strStart, "\r");
                *(strEnd - 1) = 0; // replace end double quote by \0  of SSID
                *strEnd++ = 0; //Replace \r by \0 of SSID
                strcpy(scanArray[scanIdx].ssid, strStart);

                scanIdx++;

            }
            break;
            case M2M_WIFI_SCAN_DONE:
            {
                //+WSCNDONE
                //This will create an JSON Object like this
                //{"results":[
                //    {"name":"LucHelix2_4G","mac":"94:6A:77:3A:52:1B","rssi":"-32"},
                //    {"name":"","mac":"94:6A:77:3A:52:1C","rssi":"-32"},
                //    {"name":"","mac":"94:6A:77:3A:52:1E","rssi":"-32"},
                //    {"name":"BELL799","mac":"90:72:82:F8:95:F6","rssi":"-81"},
                //    {"name":"LucNetwork_HiSpeed_24","mac":"18:D6:C7:D8:7A:87","rssi":"-18"},
                //    {"name":"TestingHotSpot","mac":"1C:7E:E5:33:54:EE","rssi":"-28"},
                //    {"name":"DIRECT-2F4C846B","mac":"DE:CD:2F:4C:04:6B","rssi":"-47"},
                //    {"name":"LucNetwork","mac":"00:26:5A:F2:4C:1C","rssi":"-65"}
                //    ]}
                gWSCNDONE = false;
                uint8_t i;
                char tempBuffer[10], lineBbuffer[10]; //tempBuffer[512], lineBbuffer[100];

                memset(tempBuffer, 0, sizeof (tempBuffer));

                for (i = 0; i < scanIdx; i++) {
                    //printf("SSID => %s ,  MAC => %s,  RSSI => %s\r\n", scanArray[i].ssid, scanArray[i].mac, scanArray[i].rssi);
                    if (i == 0) {
                        sprintf(tempBuffer, SCANSINGLEJSON, scanArray[i].ssid, scanArray[i].mac, scanArray[i].rssi);
                    } else {

                        strcat(tempBuffer, ",");
                        sprintf(lineBbuffer, SCANSINGLEJSON, scanArray[i].ssid, scanArray[i].mac, scanArray[i].rssi);
                        strcat(tempBuffer, lineBbuffer);
                    }
                }

                sprintf(scanResponse, SCANRESPONSE, tempBuffer);
                printf_CB(("WSCNDONE \r\n%s", scanResponse));

                scanIdx = 0;
                memset(scanArray, 0, sizeof (scanArray));

                break;
            }

        }

#endif
    }

}

void socket_cb(uint8_t u8MsgType, void **pvMsg) {
    //printf("%s", __FUNCTION__);
    //strcpy(sktResult, pvMsg);

    switch (u8MsgType) {

        case M2M_SOCKET_OPEN:
        {
            printf_CB(("SOCKO   %s\r\n", *pvMsg));
            break;
        }
        case M2M_SOCKET_RXTCP:
        {
            char *str, buffer[50];
            strcpy(buffer, *pvMsg);
            //+SOCKRXT:<SOCK_ID>,<LENGTH>
            if ((str = strstr(buffer, ","))) {
                *str++ = 0;
                printf_CB(("SOCKRX   SKT => %s BYTES => %s\r\n", buffer, str));
            }

            break;
        }
        case M2M_SOCKET_RXUDP:
        {
            char *str, buffer[50];
            strcpy(buffer, *pvMsg);
            //strcpy(announceResult, *pvMsg);
            //+SOCKRXU:<SOCK_ID>,<RMT_ADDR>,<RMT_PORT>,<LENGTH>
            if ((str = strstr(buffer, ","))) {
                *str++ = 0;
                printf_CB(("SOCKRXU  SKT=> %s INFO => %s\r\n", buffer, str));
            }

            break;
        }

        case M2M_SOCKET_READ:
        {
            printf_CB(("SOCKRD   %s\r\n", *pvMsg));
            break;
        }
        case M2M_SOCKET_CLOSE:
        {
            printf_CB(("SOCKCL   %s\r\n", *pvMsg));
            break;
        }
        case M2M_SOCKET_CONNECT:
        {
            printf_CB(("SOCKIND   CONNECT => %s\r\n", *pvMsg));
            break;
        }
        case M2M_SOCKET_LIST:
        {
            printf_CB(("SOCKLST   %s\r\n", *pvMsg));

            break;
        }
    }

}
static bool mqttConnected;

void mqtt_cb(uint8_t u8MsgType, void **pvMsg) {
    //uint8_t *str, buffer[20];
    //strcpy(mqttResult, pvMsg);

    switch (u8MsgType) {

        case M2M_MQTT_CONNECTED:
        {
            //+MQTTCONN:1
            if (*resultPtrMQTTCONN == '1') {
                printf_CB(("MQTTCONN  CONNECTED => %s\r\n", resultPtrMQTTCONN));
                mqttConnected = true;
            }
            if (*resultPtrMQTTCONN == '0') {
                printf_CB(("MQTTCONN  DIS-CONNECTED => %s\r\n",
                        resultPtrMQTTCONN));
                mqttConnected = false;
            }
            break;
        }
        case M2M_MQTT_PUBLISH:
        {
            //+MQTTPUB:15,"Rio2Topic/name/",11,"Hello World"
            printf_CB(("\r\nMQTTPUB  PUBLISH  => %s\r\n", resultPtrMQTTPUB));
            break;
        }
        case M2M_MQTT_SUBRX:
        {
           //+MQTTSUBRX:15,"Rio2Topic/name/",11,"Hello World"
            printf_CB(("\r\nMQTTPUB  PUBLISH  => %s\r\n", resultPtrMQTTSUBRX));
            break;
        }
        case M2M_MQTT_PUBLISH_ACK_QOS1:
        {
            //+MQTTPUBACC
            printf_CB(("MQTTPUBACC   ACK\r\n"));
            break;
        }
        case M2M_MQTT_PUBLISH_COMPLETE_QOS2:
        {
            //+MQTTPUBCOMP
            printf_CB(("MQTTPUBCOMP  PUBLISH  COMPLETE\r\n"));
            break;
        }
        case M2M_MQTT_PUBLISH_ERROR:
        {
            //+MQTTPUBERR
            printf_CB(("MQTTPUBERR  PUBLISH  ERROR\r\n"));
            break;
        }
        
    }

}

typedef void (*callback)(uint8_t u8MsgType, void **pvMsg);

typedef struct {
    const char *atCmdResp; //  AT command RN-RIO2 response string to search for
    bool *flag; //  Associated flag
    callback func; //  CallBack function
    const uint8_t event; //  Event associated with Callback
    char **param; //  AT command parameter

} ATCMD_RESPONSE; // a simple command descriptor

static uint8_t atCmdTbleIndex;
static ATCMD_RESPONSE ATCmdResponseTbl[] = {

    { "AT Command Application", &gRDY, atCmdRespHandler, CMD_RESET, false}, // Start Banner of RN-RIO2
    { "OK\r\n", &gOK, NULL, (event_type_enu) NULL, false}, //  OK
    { "ATE0\r\n", &gATE, NULL, (event_type_enu) NULL, false}, //  ATE0
    { "ATE1\r\n", &gATE, NULL, (event_type_enu) NULL, false}, //  ATE0
    { "ERROR:", &gRDY, atCmdRespHandler, CMD_ERROR, &resultPtrRDY}, //  ERROR
    { "+GMM=", &gGMM, atCmdRespHandler, CMD_MODEL, &resultPtrGMM}, //  +GMM:<MODEL_ID>
    { "+CFG:", &gCFG, atCmdRespHandler, CMD_MAC_ADDR_HOST_NAME, &resultPtrCFG}, //  +CFG:<param_id>,<param_val>   1	<MAC_ADDR>	String	The MAC address of the device (Read Only)  2	<DEVICE_NAME>	String	The device name
    { "+TIME:", &gTIME, atCmdRespHandler, CMD_TIME, &resultPtrTIME}, //  +TIME:<TIME_FORMAT>,<TIME>  //<1 - UNIX timestamp>  <2 - NTP time>  <3 - RFC3338 / ISO-8601 string format>

    { "+WSTALU:", &gWSTALU, wifi_cb, M2M_WIFI_CONNECTED, &resultPtrWSTALU}, //+WSTALU: <ASSOC_ID>,<BSSID>,<CHANNEL>
    { "+WSTALD:", &gWSTALD, wifi_cb, M2M_WIFI_DISCONNECTED, &resultPtrWSTALD}, //+WSTALD: <ASSOC_ID>
    { "+WSTAAIP:", &gWSTAAIP, wifi_cb, M2M_WIFI_REQ_DHCP_CLIENT, &resultPtrWSTAAIP}, //+WSTAAIP:1,"10.0.0.196"
    { "+WAPAIP:", &gWAPAIP, wifi_cb, M2M_WIFI_REQ_DHCP_SERVER, &resultPtrWAPAIP}, //+WAPAIP: <ASSOC_ID>,<IP_ADDRESS>
    { "+DNSRESOLV:", &gDNSRESOLV, wifi_cb, M2M_WIFI_DNS_RESOLVE, &resultPtrDNSRESOLV}, //+DNSRESOLV:0,"api.openweathermap.org","192.241.245.161"
    { "+WSCNIND:", &gWSCNIND, wifi_cb, M2M_WIFI_SCAN_RESULT, &resultPtrWSCNIND}, //+WSCNIND:<RSSI>,<SEC_TYPE>,<CHANNEL>,<BSSID>,<SSID>
    { "+WSCNDONE", &gWSCNDONE, wifi_cb, M2M_WIFI_SCAN_DONE, false}, //+WSCNDONE

    { "+SOCKO:", &gSOCKO, socket_cb, M2M_SOCKET_OPEN, &resultPtrSOCKO}, //+SOCKO:<SOCK_ID>
    { "+SOCKRXT:", &gSOCKRXT, socket_cb, M2M_SOCKET_RXTCP, &resultPtrSOCKRXT}, //+SOCKRXT:<SOCK_ID>,<LENGTH>  TCP
    { "+SOCKRXU:", &gSOCKRXU, socket_cb, M2M_SOCKET_RXUDP, &resultPtrSOCKRXU}, //+SOCKRXU:<SOCK_ID>,<RMT_ADDR>,<RMT_PORT>,<LENGTH>  UDP
    { "+SOCKRD:", &gSOCKRD, socket_cb, M2M_SOCKET_READ, &resultPtrSOCKRD}, //+SOCKRD:<SOCK_ID>,<LENGTH>,<DATA>
    { "+SOCKCL:", &gSOCKCL, socket_cb, M2M_SOCKET_CLOSE, &resultPtrSOCKCL}, //+SOCKCL:<SOCK_ID>
    { "+SOCKIND:", &gSOCKIND, socket_cb, M2M_SOCKET_CONNECT, &resultPtrSOCKIND}, //+SOCKIND:<SOCK_ID>,<LCL_ADDR>,<LCL_PORT>,<RMT_ADDR>,<RMT_PORT>
    { "+SOCKLST:", &gSOCKLST, socket_cb, M2M_SOCKET_LIST, &resultPtrSOCKLST}, //+SOCKLST:<SOCK_ID>,<PROTOCOL>,<RMT_ADDR>,<RMT_PORT>,<LCL_PORT>

    { "+MQTTCONN:", &gMQTTCONN, mqtt_cb, M2M_MQTT_CONNECTED, &resultPtrMQTTCONN}, //+MQTTCONN:0 or +MQTTCONN:1
    { "+MQTTPUB:", &gMQTTPUB, mqtt_cb, M2M_MQTT_PUBLISH, &resultPtrMQTTPUB}, //+MQTTPUB:<TOPIC_NAME_LENGHT>,"<TOPIC_NAME>",<TOPIC_PAYLOAD_LENGHT>,"<TOPIC_PAYLOAD>"
    { "+MQTTSUB:", &gMQTTSUB, mqtt_cb, M2M_MQTT_SUBSCRIBE, &resultPtrMQTTSUB}, //+MQTTPUB:<TOPIC_NAME_LENGHT>,"<TOPIC_NAME>",<TOPIC_PAYLOAD_LENGHT>,"<TOPIC_PAYLOAD>"
 /*RIO0*/   { "+MQTTSUBRX:", &gMQTTSUBRX, mqtt_cb, M2M_MQTT_SUBRX, &resultPtrMQTTSUBRX}, //+MQTTPUB:<TOPIC_NAME_LENGHT>,"<TOPIC_NAME>",<TOPIC_PAYLOAD_LENGHT>,"<TOPIC_PAYLOAD>"
    { "+MQTTPUBACC", &gMQTTPUBACC, mqtt_cb, M2M_MQTT_PUBLISH_ACK_QOS1, false}, //+MQTTPUBACC  QoS(1)
    { "+MQTTPUBCOMP", &gMQTTPUBCOMP, mqtt_cb, M2M_MQTT_PUBLISH_COMPLETE_QOS2, false}, //+MQTTPUBCOMP  QoS(2)
    { "+MQTTPUBERR", &gMQTTPUBERR, mqtt_cb, M2M_MQTT_PUBLISH_ERROR, false}, //+MQTTPUBERR:

};
#define ATCMD_RESPONSE_SIZE sizeof (ATCmdResponseTbl) / sizeof (*ATCmdResponseTbl)

/************************************************************/
/************************************************************/

/************************************************************/
void parseRIO2RxMessage(char *buffer) {
    char *str;
    uint8_t i;

    for (i = 0; i < ATCMD_RESPONSE_SIZE; i++) {
        if ((str = strstr((char *) buffer, (char *) ATCmdResponseTbl[i].atCmdResp))) {
            //printf("FOUND %s", ATCmdResponseTbl[i].atCmdResp);
            atCmdTbleIndex = i;
            *ATCmdResponseTbl[i].flag = true;

            if (ATCmdResponseTbl[i].func) //Available CallBack?
            {
                if (ATCmdResponseTbl[i].param)
                    *ATCmdResponseTbl[i].param = str
                        + strlen(ATCmdResponseTbl[i].atCmdResp); //ATCmdResponseTbl[i].param will point to after atCmdResp string
                //Example +WSTALU:1,"00:01:02:03:04:05",6
                //In example above,   .param points right after ':' to 1,"00:01:02:03:04:05",6

                ATCmdResponseTbl[i].func(ATCmdResponseTbl[i].event, (void*) ATCmdResponseTbl[i].param);
                break; //Found the command,  no need to finish parsing the rest!
            }
        }
    }

}

static bool writeStatus = false;

void APP_WriteCallback(uintptr_t context) {
    writeStatus = true;
}

//void APP_ReadCallback(USART_ERROR status, uintptr_t context) {
//    //USART_ERROR status;
//    //status = SERCOM2_USART_ErrorGet();
//    if (status == SERCOM_USART_EVENT_READ_BUFFER_FULL)
//        printf("Buffer Full\r\n");
//}

#define SECOND 1000



uint32_t getTick(void);

void APP_RIO2_Initialize(void) {
    /* Place the App state machine in its initial state. */
    app_rio2Data.state = APP_RIO2_STATE_INIT;
    //    SERCOM2_USART_WriteCallbackRegister((SERCOM_USART_RING_BUFFER_CALLBACK) APP_WriteCallback, 0);
    //    SERCOM2_USART_ReadCallbackRegister((SERCOM_USART_RING_BUFFER_CALLBACK) APP_ReadCallback, 0);


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    TEMPERATURE_init();
}

/******************************************************************************
 Function:
 void APP_RIO2_Tasks ( void )

 Remarks:
 See prototype in app_rio2.h.
 */

/*  Everything received from Terminal echo back to RIO2 AT CMD
 */

//Circular Buffer UART1 & UART2 Interrupt driven!

#define Terminal_IsRxReady UART1_IsRxReady
#define Terminal_Read UART1_Read

static uint24_t buttonTick;
static bool buttonPress, once;

void echoTerminal2RIO(void) {
    uint8_t byte;
    while (Terminal_IsRxReady()) {
        byte = Terminal_Read();
        if (byte == 0x13) //CTRL-S
        {
            printf("\r\nSM = %d\r\n", app_rio2Data.state);
            return;
        }
        if (byte == 0x3) //CTRL-C
        {
            once = false;
            printf_BLACK(("\r\nRestarting SM\r\n"));

            app_rio2Data.state = APP_RIO2_STATE_INIT;
            return;
        }
        WriteTerminal((uint8_t*) & byte, 1); //Echo back
        //Send to RIO2 as well
        WriteAnyCloud((uint8_t*) & byte, 1);
    }
}

uint8_t gValidIP;

typedef struct {
    const uint8_t item; //
    const char *cmdDescr; //  AT command
    const char *param; //  AT command parameter

} INIT_STRUCT; // a simple command descriptor

/**************************************
 *
 * STATION CONFIGURATION TABLE
 *        stationCmdTbl[]
 *
 **************************************/
char stationSSID[50] = WIFI_SSID;
char stationPWD[50] = WIFI_PSWD;
char stationMode[2] = SEC_TYPE;
#ifdef  TCPSERVER
char serverPORT[5] = SERVER_PORT;
#endif
static uint8_t stationCmdTblIndex;
#define CONFIG_AP_INIT_SIZE sizeof (stationCmdTbl) / sizeof (*stationCmdTbl)
static INIT_STRUCT stationCmdTbl[] = {
    { -3, "AT+SNTPC=2,1\r\n", NULL}, //Ensure NTP is enable
    { -2, "AT+SNTPC=3,\"%s\"\r\n", "0.in.pool.ntp.org"}, //set NTP server
    { -1, "AT+SNTPC=1,1\r\n", NULL}, //Enable static (don't use NTP address provided during DHCP)

    { 0, "AT+WSTAC=1,\"%s\"\r\n", stationSSID},
    { 1, "AT+WSTAC=2,%s\r\n", stationMode},
    { 2, "AT+WSTAC=3,\"%s\"\r\n", stationPWD},
    {6, "AT+WSTA=1\r\n", NULL}, //Connect
};

/**************************************
 *
 *          SOFTATP CONFIGURATION TABLE
 *                  softAPCmdTbl[]
 *
 **************************************/

const char softAP_IP[20] = "\"10.10.0.1\"";
const char assign_Remote_IP[] = "\"10.10.0.50\"";

static uint8_t softAPCmdTblIndex;
#define CONFIG_SOFTAP_INIT_SIZE sizeof (softAPCmdTbl) / sizeof (*softAPCmdTbl)
static INIT_STRUCT softAPCmdTbl[] = {
    { 0, "AT+WAPC=1,\"%s\"\r\n", "WFI32_WPA2" /*softAPSSID*/}, //BSSID
    {1, "AT+WAPC=2,%s\r\n", "3"/*softAPMode*/}, //Mode 3 is WPA/WPA2
    { 2, "AT+WAPC=3,\"%s\"\r\n", "Microchip" /*softAPPWD*/}, //PWD
    { 3, "AT+WAPC=4,%s\r\n", "11" /*softAPChannel*/}, //AP Channel "1" to "14"
    { 4, "AT+WAPC=5,%s\r\n", "0" /*softAPVisible*/}, //Visible "0" Hidden "1"
    { 5, "AT+WAPC=6,%s\r\n", softAP_IP /*softAPGatewayIP*/}, //Gateway IP
    { 6, "AT+WAPC=7,\"%s\"\r\n", "255.255.255.0" /*softAPGatewayMask*/}, //MASK
    { 7, "AT+WAPC=8,%s\r\n", softAP_IP /*softAPRouterIP*/}, //Default Router IP
    { 8, "AT+WAPC=9,%s\r\n", softAP_IP /*softAPDNSIP*/}, //DNS Server
    { 9, "AT+WAPC=10,%s\r\n", "1" /*softAPDHCPs*/}, //DHCPs enable "1" disable "0"
    { 10, "AT+WAPC=11,%s\r\n", assign_Remote_IP /*softAPDHCPsIP*/} //DHCPs IP Pool Start IP Addr  ex:  Gateway is 192.168.10.1,  Pool could be 192.168.10.100...to 255

};


/**************************************
 *
 * MQTT CONFIGURATION TABLE
 *        mqttCmdTbl[]
 *
 **************************************/
char thingID[30] = MY_THING_ID;
char broker[75] = BROKER;
char brokerCert[50] = "AmazonRootCA1";//"AmazonRootCA1";//DigiCertGlobalRootG2";//BaltimoreCyberTrustRoot";
char port[10] = PORT;
char TLS[2] = TLSENABLE;
#ifdef USE_AZURE
char brokerUserName[150] = BROKER_USER_NAME;
#endif
char brokerUserPWD[50] = BROKER_USER_PWD;
char clientID[50] = CLIENTID;
char keepAlive[50] = KEEPALIVE;

static uint8_t mqttCmdTblIndex;
#define CONFIG_MQTT_INIT_SIZE sizeof (mqttCmdTbl) / sizeof (*mqttCmdTbl)
static INIT_STRUCT mqttCmdTbl[] = {
    { -2, "AT+TIME=3\r\n", NULL},
    { -1, "ATE0\r\n", NULL},

    { -1, "AT+TLSC=1,1,\"%s\"\r\n", brokerCert},
#ifdef USE_AZURE
    { -1, "AT+TLSC=1,2,\""RNWF_DEVICE_CERT"\"\r\n", NULL},
    { -1, "AT+TLSC=1,3,\""RNWF_DEVICE_KEY"\"\r\n", NULL},
    { -1, "AT+TLSC=1,5,\"*.azure-devices-provisioning.net\"\r\n", NULL},
#endif
#ifdef USE_AWS
    { -1, "AT+TLSC=1,2,\"RIO_0_AWS_CRT\"\r\n", NULL},
    { -1, "AT+TLSC=1,3,\"RIO_0_AWS_KEY\"\r\n", NULL},
    { -1, "AT+TLSC=1,5,\"%s\"\r\n", broker},
#endif
    { 0, "AT+MQTTC=1,\"%s\"\r\n", broker},
    { 1, "AT+MQTTC=2,%s\r\n", port},
    { 2, "AT+MQTTC=3,\"%s\"\r\n", clientID},
#ifdef USE_AZURE
    { 3, "AT+MQTTC=4,\"%s\"\r\n", brokerUserName},
#endif
    //    { 4, "AT+MQTTC=5,\"%s\"\r\n", brokerUserPWD},
    { 5, "AT+MQTTC=6,%s\r\n", keepAlive},
    { 6, "AT+MQTTC=7,%s\r\n", TLS}, //Use TLS Idx "1" for secure connection
    { 6, "AT+MQTTC=8,3\r\n", NULL}, //Use MQTT Version 3
    { 7, "AT+MQTTCONN=1\r\n", NULL},

};

#define SW0 (RA3)
#define USER_LED (RF3)

char largeBuffer[500];

void APP_RIO2_Tasks(void) {

    char buffer1[50];
    static bool cloudBrokerConnected = false;
        


    static uint16_t count, byteRcvd;

    static char *jsonPayloadResponse;


    static uint32_t myDelay, myPUBDelay = -1;


    static char mqttRCVMessage[100] = "Default POR Msg";
    //static bool connected2IoTCentral;

    static char valueIP4[5], valueIP3[5], valueIP2[5], valueIP1[5];
    static uint8_t telemetryInterval = 10;

    echoTerminal2RIO();
    /***********   PIC18    ***************/
    static bool SwLastState, SwPressFlag = false;
    static uint16_t temperatureValue;
    static uint32_t gDelay;

    // Detect change in state of the SW0 button
    if (SW0 != SwLastState) {
        SwLastState = SW0;
        myPUBDelay = getTick();
        if (SW0 == 0) {
            // We just caught the falling edge
            press_count++;
            SwPressFlag = true;
        }
    }
    static uint8_t ledState;



    /***********   PIC18   ***************/

#if (0)
    static uint32_t gDelay;
    if (!(getTick() > gDelay + (SECOND / 10)))
        return;
    gDelay = getTick();
#endif
    //Get next line form UART Circular buffer and place inline to current "linearBuffer[]"
    //Make malloc size the SIZE of the largest packet you may receive!!!!!
    static char * tempBuffer;
    static uint16_t linearBufferIndex;
    if (tempBuffer == NULL)
        tempBuffer = malloc(850); //Make malloc size the SIZE of the largest packet you may receive!!!!!
                                  //If using AT+READCERT=1,  Certificate is > 800 bytes

    // Get next line form UART Circular buffer and place  to current "tempBuffer"
    // Note tempBuffer will increment on every byte receive from AnyCloud,  not just on CRLF
    // and only returns with byteRcvd different from 0 after CFLF
    if ((byteRcvd = GetNextLineUntil_CRLF(tempBuffer))) {
        //Ensure at LEAST byteRcvd are available at end of linearBuffer for next line with extra 10 bytes for safety ?
        if (byteRcvd >= (sizeof (linearBuffer) - linearBufferIndex - 10)) {

            printf_TST(("\r\n\r\nRESETING\r\nBufferSize %d\r\nbyteRcvd %d\r\nIndex %d\r\n\r\n", sizeof (linearBuffer), byteRcvd, linearBufferIndex));
            linearBufferIndex = 0;
        }
        // Copy tempBuffer to linearBuffer actual free position linearBuffer[linearBufferIndex]
        memcpy(&linearBuffer[linearBufferIndex], tempBuffer, byteRcvd);
        // free tempBuffer,  so that it is re-initialize to point at the beginning of a new malloc!! 
        //free(tempBuffer);
        //tempBuffer = NULL;
        linearBuffer[linearBufferIndex + byteRcvd] = 0; // Terminate "LINE\r\n" with NULL so it becomes "LINE\r\n\0"
        ++byteRcvd;

        printf_FRIO(("%s", &linearBuffer[linearBufferIndex])); // print received line

        parseRIO2RxMessage(&linearBuffer[linearBufferIndex]); //Parse current "LINE\r\\n"
        linearBufferIndex += byteRcvd; //Index of linearBuffer[]. Needs to be place after current "LINE\r\n\0"


    }
#ifdef PASSTHROUGH
    return;
#endif
    if ((app_rio2Data.state == APP_RIO2_STATE_SOCKET_IDLE) && (1)) {
        if (gSOCKIND)
            app_rio2Data.state = APP_RIO2_STATE_TCP_SOCKET_CONNECTED;

        if (gSOCKRXT)
            app_rio2Data.state = APP_RIO2_STATE_TCP_SOCKET_RCV_DATA;

        if (gSOCKRXU)
            app_rio2Data.state = APP_RIO2_STATE_UDP_SOCKET_RCV_DATA;

        if (gMQTTCONN) {
            if (strstr(resultPtrMQTTCONN, "1")) {
#ifdef USE_AZURE
                if (strcmp(broker, BROKER) == 0) //(!connected2IoTCentral)
                    app_rio2Data.state = APP_MQTT_STATE_CONNECTED;
#else
                app_rio2Data.state = APP_MQTT_STATE_CONNECTED;
#endif
            } else if (strstr(resultPtrMQTTCONN, "0")) {

                app_rio2Data.state = APP_MQTT_STATE_DISCONNECTED;
            }
        }

        if (gMQTTSUBRX && cloudBrokerConnected) {
            app_rio2Data.state = APP_MQTT_STATE_RCV_DATA;

        }

    }

#ifdef MQTTCLIENT
    if (((app_rio2Data.state == APP_RIO2_STATE_SOCKET_IDLE)
            && (getTick() > myPUBDelay)) && mqttConnected) {

        app_rio2Data.state = APP_MQTT_STATE_PUBLISH; //APP_RIO2_STATE_AZURE_PUBLISH;
    }

#endif

    /* Check the application's current state. */
    switch (app_rio2Data.state) {
            char buffer[200];
            static uint24_t pubCount;

        case APP_RIO2_STATE_SOCKET_IDLE:
        {

            break;
        }
            /* Application's initial state. */
        case APP_RIO2_STATE_INIT:
        {
            bool appInitialized = true;
            WriteAnyCloud((uint8_t*) "AT+RST\r\n", strlen("AT+RST\r\n"));
            
            gRDY = false;
            gOK = false;

            if (appInitialized) {

                app_rio2Data.state = APP_RIO2_STATE_WAIT_RST;
                myDelay = getTick();
                stationCmdTblIndex = 0;
                softAPCmdTblIndex = 0;
                mqttCmdTblIndex = 0;
                pubCount = 0;
                ntpUpdated = false;
#ifdef USE_AZURE
                //reinit MQTT Config for Azure
                strcpy(broker, BROKER);
                strcpy(brokerUserName, BROKER_USER_NAME);
                cloudBrokerConnected = false;
                
                strcpy (brokerCert,"BaltimoreCyberTrustRoot");// "BaltimoreCyberTrustRoot"; "DigiCertGlobalRootG2"
                //connected2IoTCentral = false;
#endif
            }
            break;
        }
        case APP_RIO2_STATE_WAIT_RST:
        {
            if (getTick() > myDelay + (SECOND * 5)) {
                app_rio2Data.state = APP_RIO2_STATE_INIT;
            }

            if (gOK) { //(gRDY) {
                static bool once1;
                if (SW0 == 0) {
                    if (!buttonPress) {
                        buttonPress = true;
                        buttonTick = getTick();
                    }
                    if ((getTick() > buttonTick + (SECOND * 2)) && (!once)) {
                        once = true;
                        once1 = false;
                        printf_BLACK (("Thing Name %s\r\n",MY_THING_ID));
                        WriteAnyCloud((uint8_t*) "AT+READCERT=1\r\n", strlen("AT+READCERT=1\r\n"));
                        printf_2RIO(((uint8_t*) "AT+READCERT=1\r\n", strlen("AT+READCERT=1\r\n")));
                        
                    }
                    app_rio2Data.state = APP_RIO2_STATE_WAIT_RST;
                    break;

                } else {
                    buttonPress = false;
                    if (!once1)
                    {
                        once1 = true;
                        app_rio2Data.state = APP_RIO2_STATE_INIT;
                        break;
                    }
                }

                WriteAnyCloud((uint8_t*) "ATE0\r\n", strlen("ATE0\r\n"));
                printf_2RIO(((uint8_t*) "ATE0\r\n", strlen("ATE0\r\n")));

                app_rio2Data.state = APP_RIO2_STATE_ECHO_OFF;
                gOK = false;
                gATE = false;
                myDelay = getTick();

            }

            break;
        }

        case APP_RIO2_STATE_ECHO_OFF:
        {
            if (getTick() > myDelay + (SECOND * 5)) {
                app_rio2Data.state = APP_RIO2_STATE_WAIT_RST;
                break;
            }
            if (gATE && gOK) {
                gATE = false;
                gOK = false;
                WriteAnyCloud((uint8_t*) "AT+CFG\r\n", strlen("AT+CFG\r\n"));
                printf_2RIO(((uint8_t*) "AT+CFG\r\n", strlen("AT+CFG\r\n")));
                app_rio2Data.state = APP_RIO2_STATE_GET_VERSION;

            }

        }
            break;
        case APP_RIO2_STATE_GET_VERSION:
        {
            if (gOK && gCFG) {
                printf_DBG(("Module ID is %s\r\n", (strstr(resultPtrCFG, ",") + 1)));
                app_rio2Data.state = APP_RIO2_STATE_CONFIG_AP; //APP_RIO2_STATE_SET_SSID;

                gOK = true;
            }

            break;
        }
        case APP_RIO2_STATE_CONFIG_AP:
        {

            if (gOK) {

                //if (!(getTick() > myDelay + (SECOND / 2)))
                //    return;
                myDelay = getTick();
                gOK = false;
                if (stationCmdTblIndex < CONFIG_AP_INIT_SIZE) {
                    //char buffer[100];
                    sprintf(buffer, stationCmdTbl[stationCmdTblIndex].cmdDescr,
                            stationCmdTbl[stationCmdTblIndex].param);
                    printf_2RIO(("%s", buffer));
                    WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                    stationCmdTblIndex++;
                    gOK = false;

                } else {
                    stationCmdTblIndex = 0;
                    app_rio2Data.state = APP_RIO2_STATE_WAIT_IP; //APP_RIO2_STATE_CONNECT_STA;
                    myDelay = getTick();
                    gOK = true; //Force gOK
                    gWSTAAIP = gWAPAIP = false;
                }

            }
            break;
        }

        case APP_RIO2_STATE_CONNECT_STA:
        {
            if ((getTick() > myDelay + (SECOND))) //Wait 2 SECOND before connecting,  otherwise fail!!!
            {

                WriteAnyCloud((uint8_t*) "AT+WSTA=1\r\n", strlen("AT+WSTA=1\r\n"));
                printf_2RIO(((uint8_t*) "AT+WSTA=1\r\n", strlen("AT+WSTA=1\r\n")));
                printf("\r\nConnecting to InfraStructure\r\n");

                gOK = false;
                gWSTAAIP = gWAPAIP = gOK = false;

                app_rio2Data.state = APP_RIO2_STATE_WAIT_IP;

            }
            break;
        }

        case APP_RIO2_STATE_WAIT_IP:
        {
            char *resultPtr, localIPArray[sizeof (softAP_IP)];
            if (gOK && (gWSTAAIP || gWAPAIP)) {
                if (gWSTAAIP)
                    resultPtr = resultPtrWSTAAIP;
                if (gWAPAIP) //SOFTAP,  IP provided to remote device (DHCP Server)
                {
                    //Do not mess up softAP_IP,  so copy it into a temp array!!!
                    strcpy(localIPArray, softAP_IP);
                    resultPtr = localIPArray;
                }
                gWSTAAIP = gWAPAIP = gOK = false;

                gValidIP = true;

                char *ptrS, *ptrE;
                //+WSTAAIP:1,"10.0.0.200"\r\n

                if ((ptrS = strstr((char *) resultPtr, "\""))) { //Search first "
                    *ptrS++ = 0;
                    if ((ptrE = strstr(ptrS, "."))) { //Find first dot "."
                        *ptrE++ = 0; //Terminate with NULL
                        strcpy(valueIP4, ptrS); //Copy to IP4
                        ptrS = ptrE;
                        if ((ptrE = strstr(ptrS, "."))) { //Find second dot "."
                            *ptrE++ = 0; //Terminate with NULL
                            strcpy(valueIP3, ptrS); //Copy to IP3
                            ptrS = ptrE;
                            if ((ptrE = strstr(ptrS, "."))) { //Find third dot "."
                                *ptrE++ = 0; //Terminate with NULL
                                strcpy(valueIP2, ptrS); //Copy to IP2
                                ptrS = ptrE;
                                if ((ptrE = strstr(ptrS, "\""))) {//Find third dot "."
                                    *ptrE++ = 0; //Terminate with NULL
                                    strcpy(valueIP1, ptrS); //Copy to IP1
                                }
                            }
                        }
                    }
                }
                printf_DBG(("AnyCloud IP is => %s.%s.%s.%s\r\n", valueIP4, valueIP3, valueIP2, valueIP1));
                //printf_2RIO(((uint8_t*) "AT+TIME=3\r\n", strlen("AT+TIME=3\r\n")));
                
                
#ifdef MQTTCLIENT
                app_rio2Data.state = APP_MQTT_STATE_INIT_MQTT;
                gOK = true; //Force START INIT MQTT
#else
                // TODO LAB2 STEP2 B.3 Change next SM for APP_RIO2_STATE_UDP_SOCKET_CREATE
                app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
#ifdef SOLUTION
                app_rio2Data.state = APP_RIO2_STATE_UDP_SOCKET_CREATE;
#endif
#endif

            }
            break;
        }
            // TODO LAB2 STEP2 B.4 Create UDP Socket        
        case APP_RIO2_STATE_UDP_SOCKET_CREATE:
        {
            //char buffer [100];

            //Build AT+CMD to create  a UDP socket
            //AT+SOCKO=<PROTOCOL>  1 == UDP, 2 == TCP
            // TODO     LAB2 STEP2 B.4.a Create SOCKCMD   
#define  SOCKCMD "REPLACE WITH COMMAND"
#ifdef SOLUTION
#define  SOCKCMD "AT+SOCKO=1"
#endif
            sprintf(buffer, SOCKCMD"\r\n");
            // Send to AnyCLoud
            WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
            //Echo back to Terminal
            printf_2RIO((buffer));

            app_rio2Data.state = APP_RIO2_STATE_UDP_SOCKET_GET;
            break;
        }
            // TODO LAB2 STEP2 B.5 Get UDP Socket 
            static char udpSocket[10];
        case APP_RIO2_STATE_UDP_SOCKET_GET:
        {
            // After previous command was sent to create a UDP Socket,
            // AnyCLoud will respond with following
            // +SOCKO:3755\r\n\0  NULL terminated
            // AND
            // OK\r\n\0
            //
            // resultPtrSOCKO points to 3755\r\n\0

            char *ptr;
            // TODO     LAB2 STEP2 B.5.a Wait for TWO global Flags to be set
            // From AnyCLoud response parser in ATCmdResponseTbl[]
            // { "+SOCKO:", &gSOCKO, socket_cb, M2M_SOCKET_OPEN, &resultPtrSOCKO}, //+SOCKO:<SOCK_ID>
            // { "OK\r\n", &gOK, NULL, (event_type_enu) NULL, false}, //  OK

#define FLAG1 dummyFlag/*replace with global flag set when OK is received from AnyCLoud*/
#define FLAG2 dummyFlag/*replace with global flag set when +SOCKO:12345 is received from AnyCLoud*/
#ifdef SOLUTION
#define FLAG1 gSOCKO
#define FLAG2 gOK
#endif
            if (!FLAG1 && !FLAG2) //Replace FLAG1 and FLAG2 with two global flags set when event occurs
                break;
            FLAG1 = FLAG2 = false; // CLear global Flags
            //Need to remove the \r\n from +SOCKO:3755\r\n\0
            //Remove trailing \r\n, replace with NULL
            *strstr((const char *) resultPtrSOCKO, "\r\n") = '\0';
            // TODO     LAB2 STEP2 B.5.b Copy resultPtrSOCKO into udpSocket[]
            strcpy(dummyArray, dummyArray);
#ifdef SOLUTION           
            strcpy(udpSocket, resultPtrSOCKO);
#endif 
            app_rio2Data.state = APP_RIO2_STATE_UDP_SOCKET_BIND;
            //  no need to break we can jump to next SM right away if we want!!!
            //break;
        }
            // TODO LAB2 STEP2 B.6 BIND UDP Socket 
        case APP_RIO2_STATE_UDP_SOCKET_BIND:
        {
            //char buffer [100];

            // TODO LAB2 STEP2 B.6.a Create BINDCMD, include %s for  <SOCK_ID> stored in udpSocket[]
            //Build AT+CMD to BIND the UDP socket with PORT 30303
            //AT+SOCKBL=<SOCK_ID>,<LCL_PORT>
#define  BINDCMD "REPLACE WITH COMMAND"
#ifdef SOLUTION
#define  BINDCMD "AT+SOCKBL=%s,30303"
#endif
            sprintf(buffer, BINDCMD"\r\n", udpSocket); // the BINDCMD must include %s for <SOCK_ID> in udpSocket
            // Send to AnyCLoud
            WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
            //Echo back to Terminal
            printf_2RIO((buffer));

            app_rio2Data.state = APP_RIO2_STATE_UDP_SOCKET_SEND_DATA;
            break;
        }

            // TODO LAB2 STEP2 B.7  BROADCAST on UDP Socket    
        case APP_RIO2_STATE_UDP_SOCKET_SEND_DATA:
        {
            //char buffer [100];
            //After previous command was sent,
            //Wait to receiving OK\r\n\0  from AnyCLoud NULL terminated

            if (!gOK)
                break;
            // TODO LAB2 STEP2 B.7.a Create WRITECMD 
            //Build AT+CMD to WRITE to the UDP socket on PORT 30303, include %s for  <SOCK_ID> stored in udpSocket[]
            //AT+SOCKWRTO=<SOCK_ID>,<RMT_ADDR>,<RMT_PORT>,<LENGTH>[,<DATA>]
#define  WRITECMD "REPLACE WITH COMMAND"
#ifdef SOLUTION
#define  WRITECMD "AT+SOCKWRTO=%s,\"255.255.255.255\",30303,5,\"Hello\""
#endif
            sprintf(buffer, WRITECMD"\r\n", udpSocket); // the WRITECMD must include %s for <SOCK_ID> in udpSocket
            WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
            printf_2RIO((buffer));

            app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
            break;
        }
            // TODO LAB2 STEP2 B.8 On UDP Socket received,  issue a READ CMD!       
        case APP_RIO2_STATE_UDP_SOCKET_RCV_DATA:
        {
            //char buffer [100];
            //Receiving this from AnyCLoud NULL terminated
            //+SOCKRXU:3855,"192.168.1.107",30303,23\r\n\0

            if (!gSOCKRXU) {
                printf_DBG(("SM called but flag not set\r\n ERROR\r\n"));
                break;
            }
            gSOCKRXU = false;
            // TODO LAB2 STEP2 B.8.a Create READCMD,   include %s for  <SOCK_ID> stored in udpSocket[]          
            //AT+SOCKRD=<SOCK_ID>,<OUTPUT_MODE>,<LENGTH>
#define  READCMD "REPLACE WITH COMMAND"
#ifdef SOLUTION
#define  READCMD "AT+SOCKRD=%s,1,999"
#endif
            sprintf(buffer, READCMD"\r\n", udpSocket); // the READCMD must include %s for <SOCK_ID> in udpSocket
            // Send to AnyCLoud
            WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
            //Echo back to Terminal
            printf_2RIO((buffer));

            printf_DBG(("\r\n\r\nReceived DATA :\r\n\r\n"));

            app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
            break;
        }



#ifdef MQTTCLIENT
        case APP_MQTT_STATE_INIT_MQTT:
        {
            if (gOK && ntpUpdated) {

                gOK = gMQTTCONN = false;

                mqttConnected = false;

                if (mqttCmdTblIndex < CONFIG_MQTT_INIT_SIZE) {
                    //char buffer[100];
                    sprintf(buffer, mqttCmdTbl[mqttCmdTblIndex].cmdDescr,
                            mqttCmdTbl[mqttCmdTblIndex].param);
                    WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                    printf_2RIO(("%s", buffer));

                    mqttCmdTblIndex++;

                } else {
                    mqttCmdTblIndex = 0;
                    app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
                }

            }
            break;
        }

        case APP_MQTT_STATE_CONNECTED:
        {
            if (mqttConnected) {
                gMQTTCONN = false;
                //connected2IoTCentral = true;
                printf_DBG(("MQTT Connected\r\n"));
                app_rio2Data.state = APP_MQTT_STATE_SUBSCRIBE;
            }

        }
        case APP_MQTT_STATE_SUBSCRIBE:
        {
            //char buffer[100];

#ifdef USE_AZURE
            sprintf(buffer, "AT+MQTTSUB=\""SUB_DPS_REGISTRATION_TOPIC"\",0\r\n");
#else
            cloudBrokerConnected = true;
            sprintf(buffer, "AT+MQTTSUB=\""SUB_TOPIC"\",0\r\n", MY_THING_ID);
#endif
            WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
            printf_2RIO(("%s", buffer));
            gMQTTSUB = false;
            app_rio2Data.state = APP_MQTT_STATE_SUBSCRIBE_COMPLETE;

            break;
        }
        case APP_MQTT_STATE_SUBSCRIBE_COMPLETE:
        {
            if (gMQTTSUB) {
                gMQTTSUB = false;
                //PATCH since after +MQTTCONN:1,  AnyCLoud starts echoing back again!
                //Issue another ATE0 :)
                WriteAnyCloud((uint8_t*) "ATE0\r\n", strlen("ATE0\r\n"));
#ifdef USE_AZURE
                app_rio2Data.state = APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_PUT;
#else
                app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
                myPUBDelay = getTick() + (1 * SECOND);
#endif
            }
            break;
        }
#ifdef USE_AZURE

        case APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_PUT:
        {
            //char buffer[175];

            gMQTTSUBRX = false;
            sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC_DPS_PUT"\",\""PUB_REPORTED_PAYLOAD"\"\r\n");
            WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
            printf_2RIO(("%s", buffer));
            app_rio2Data.state = APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_GET;
            myDelay = getTick();

            break;
        }
        case APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_GET:
        {
            //char buffer[150];
            char *operationID;
            //Wait 3 seconds before requesting Assigned HUB
            if ((getTick() - (SECOND * 3)) < myDelay)
                break;
            myDelay = getTick();
            if (gMQTTSUBRX) {
                gMQTTSUBRX = false;
                if ((operationID = strstr(resultPtrMQTTSUBRX, "\\\"operationId\\\":\\\""))) {
                    operationID += strlen("\\\"operationId\\\":\\\"");
                    *(strstr(operationID, "\\")) = 0;

                    sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC_DPS_GET"\",\"""\"\r\n", operationID);
                    WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                    printf_2RIO(("%s", buffer));
                    // if Azure +MQTTPUB response does not contain "retry-after=",  
                    // move to next state, otherwise retry in 1 SECOND
                    if (!strstr(resultPtrMQTTSUBRX, "retry-after="))
                        app_rio2Data.state = APP_RIO2_STATE_AZURE_GET_ASSIGN_HUB_AND_DISCONNECT;
                    
                        
                }
                //strcpy(brokerUserName,BROKER_USER_NAME_HUB);
            }
            break;
        }
            static char *azureAssignedHub;
        case APP_RIO2_STATE_AZURE_GET_ASSIGN_HUB_AND_DISCONNECT:
        {
            //char buffer[50];
            if (gMQTTSUBRX) {
                gMQTTSUBRX = false;
                if ((azureAssignedHub = strstr(resultPtrMQTTSUBRX, "\\\"assignedHub\\\":\\\""))) {

                    azureAssignedHub += strlen("\\\"assignedHub\\\":\\\"");
                    *(strstr(azureAssignedHub, "\\")) = 0;  //Find the closing \"
                    sprintf(buffer, "AT+MQTTDISCONN\r\n");
                    WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                    printf_2RIO(("%s", buffer));

                    strcpy (brokerCert,"DigiCertGlobalRootG2");
                    app_rio2Data.state = APP_RIO2_STATE_AZURE_WAIT_DISCONNECT;
                    gOK = false;
                    mqttCmdTblIndex = 0;
                    sprintf(broker, azureAssignedHub);
                    sprintf(brokerUserName, "%s/%s/?api-version=2021-04-12", azureAssignedHub, clientID);
                    gMQTTCONN = false;
                }
            }

            break;
        }
        case APP_RIO2_STATE_AZURE_WAIT_DISCONNECT:
        {
            if (gMQTTCONN && !mqttConnected) {
                gMQTTCONN = false;
                app_rio2Data.state = APP_RIO2_STATE_AZURE_RECONNECT;

            }
            break;
        }
        case APP_RIO2_STATE_AZURE_RECONNECT:
        {
            if (gOK) {
                gOK = false;


                if (mqttCmdTblIndex < CONFIG_MQTT_INIT_SIZE) {
                    //char buffer[150];
                    sprintf(buffer, mqttCmdTbl[mqttCmdTblIndex].cmdDescr,
                            mqttCmdTbl[mqttCmdTblIndex].param);
                    WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                    printf_2RIO(("%s", buffer));

                    mqttCmdTblIndex++;

                } else {
                    mqttCmdTblIndex = 0;
                    app_rio2Data.state = APP_RIO2_STATE_AZURE_SUBSCRIBE_1; //APP_RIO2_STATE_AZURE_PUBLISH;//APP_RIO2_STATE_AZURE_SUBSCRIBE_1;
                    gOK = false;
                    gMQTTCONN = false;
                }

            }

            break;
        }

        case APP_RIO2_STATE_AZURE_SUBSCRIBE_1:
        {
            //char buffer[125];
            if (gMQTTCONN && mqttConnected) {
                cloudBrokerConnected = true;
                gMQTTCONN = false;
                //sprintf(buffer, "AT+MQTTSUB=\"$iothub/twin/PATCH/properties/desired/#\",0\r\n");
                sprintf(buffer, "AT+MQTTSUB=\"$iothub/methods/POST/#\",0\r\n");
                WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                printf_2RIO(("%s", buffer));
                gMQTTSUB = false;
                app_rio2Data.state = APP_RIO2_STATE_AZURE_SUBSCRIBE_2; //APP_RIO2_STATE_AZURE_PUBLISH; //APP_RIO2_STATE_AZURE_SUBSCRIBE_2;

            }

            break;
        }
        case APP_RIO2_STATE_AZURE_SUBSCRIBE_2:
        {
            //char buffer[125];
            {
                if (!gMQTTSUB)
                    break;
                gMQTTSUB = false;
                //sprintf(buffer, "AT+MQTTSUB=\"$iothub/twin/res/#\",0\r\n");
                sprintf(buffer, "AT+MQTTSUB=\"$iothub/twin/PATCH/properties/desired/#\",0\r\n");

                WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                printf_2RIO(("%s", buffer));

                app_rio2Data.state = APP_RIO2_STATE_AZURE_SUBSCRIBE_3; //APP_RIO2_STATE_AZURE_PUBLISH;

            }

            break;
        }
        case APP_RIO2_STATE_AZURE_SUBSCRIBE_3:
        {
            //char buffer[125];
            {
                if (!gMQTTSUB)
                    break;
                gMQTTSUB = false;
                sprintf(buffer, "AT+MQTTSUB=\"$iothub/twin/res/#\",0\r\n");
                //sprintf(buffer, "AT+MQTTSUB=\"$iothub/twin/PATCH/properties/desired/#\",0\r\n");

                WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
                printf_2RIO(("%s", buffer));

                app_rio2Data.state = APP_RIO2_STATE_AZURE_SUBSCRIBE_DONE; //APP_RIO2_STATE_AZURE_PUBLISH;

            }

            break;
        }
        case APP_RIO2_STATE_AZURE_SUBSCRIBE_DONE:
        {

            {
                if (!gMQTTSUB)
                    break;
                //PATCH since after +MQTTCONN:1,  AnyCLoud starts echoing back again!
                //Issue another ATE0 :)
                WriteAnyCloud((uint8_t*) "ATE0\r\n", strlen("ATE0\r\n"));
                gMQTTSUB = false;
                app_rio2Data.state = APP_MQTT_STATE_PUBLISH; //APP_RIO2_STATE_AZURE_PUBLISH;

            }

            break;
        }


#endif
#define PUBSTATESNUM_INIT 2 // This should *not* change
#define PUBSTATESNUM_TOTAL 4 // This can be changed with less/more publish states

        case APP_MQTT_STATE_PUBLISH:
        {

            //char buffer[250];

            app_rio2Data.state = APP_MQTT_STATE_WAIT_PUB;
#ifdef USE_AZURE
            static uint8_t pubState;

            switch (pubState) {
                case 0: // Get the Device Twin's information
                {
                    strcpy(buffer, "AT+MQTTPUB=0,0,0,\"$iothub/twin/GET/?$rid=getTwin\",\"\"\n\r");
                    pubState++;
                    break;
                }
                case 1:
                {
                    sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC_PROPERTIES"\",\""PUB_PAYLOAD_IP"\"\r\n", valueIP4, valueIP3, valueIP2, valueIP1);
                    pubState = PUBSTATESNUM_TOTAL;
                    break;
                }
                    /* Each publish state above is only executed once after reset */
                    /* From here, this is the start of telemetry loop which should execute on every "telemetryInterval" seconds */
                case PUBSTATESNUM_INIT:
                {
                    if (SwPressFlag == true) // If SW0 button was pressed, send button telemetry event
                    {
                        sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC_TELEMETRY"\",\"{\\\"button_event\\\":{\\\"button_name\\\":\\\"SW0\\\",\\\"press_count\\\":%d}}\"\r\n", thingID, press_count);
                        SwPressFlag = false;
                    } else // ...otherwise just update the User LED state
                    {
                        strcpy(buffer, "AT+MQTTPUB=0,0,0,\"$iothub/twin/PATCH/properties/reported/?rid=user_LED\",\"{");
                        sprintf(buffer1, PUB_AZURE_DESIRED_VALUE_PAYLOAD, "led_user", appData.LED_user);
                        strcat(buffer, buffer1);
                        strcat(buffer, "}\"\r\n");
                    }
                    pubState++;
                    break;
                }
                case (PUBSTATESNUM_TOTAL - 1): /* Only this state (just before the final state) can do nothing */
                {
                    sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC_TELEMETRY"\",\""PUB_AZURE_PAYLOAD_TELEMETRY_TEMPERATURE"\"\r\n", thingID, (float) TEMPERATURE_readDegC());
                    pubState++;
                    break;
                }
                case PUBSTATESNUM_TOTAL:
                {
                    pubState = PUBSTATESNUM_INIT;
                    //sprintf(buffer, "\r\n");
                    sprintf(buffer, "");
                    app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
                    myPUBDelay = getTick() + telemetryInterval * SECOND;
                    break;
                }
            }
#endif
#ifdef USE_AWS

            typedef enum {
                SM_AWS_PUB_IP,
                SM_AWS_PUB_COUNT,
                SM_AWS_PUB_BUTTON,
                SM_AWS_PUB_VOLTAGE,
                SM_AWS_PUB_CURRENT,
                SM_AWS_PUB_RESISTOR,
                SM_AWS_PUB_CAPACITOR,
                SM_AWS_PUB_MULTIMETER_ALL,
                SM_AWS_PUB_DONE = 0xff

            } aws_pub_state;
            static uint8_t pubState = SM_AWS_PUB_IP;
            switch (pubState) {
                case SM_AWS_PUB_IP:
                {
                    sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\""PUB_STATE_REPORTED_IP"\"\r\n", thingID, valueIP4, valueIP3, valueIP2, valueIP1);
                    //We will be PUBlishing all desired value in one JSON payload
                    pubState = SM_AWS_PUB_MULTIMETER_ALL;
                    break;
                }
                case SM_AWS_PUB_COUNT:
                {//PUB Count ONLY
                    sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\""PUB_PAYLODAD"\"\r\n", thingID, (int) pubCount++);
                    pubState = SM_AWS_PUB_VOLTAGE;
                    break;
                }
                case SM_AWS_PUB_VOLTAGE:
                { //PUB Volt ONLY
                    float MULTIMETER_getVoltage(void);
                    sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\""PUB_STATE_REPORTED_VOLTAGE"\"\r\n", thingID, (float) MULTIMETER_getVoltage());
                    pubState = SM_AWS_PUB_CURRENT;
                    break;
                }
                case SM_AWS_PUB_CURRENT:
                {//PUB Current  ONLY
                    pubState = SM_AWS_PUB_RESISTOR;
                    break;
                }
                case SM_AWS_PUB_RESISTOR:
                {//PUB Resistor ONLY
                    pubState = SM_AWS_PUB_CAPACITOR;
                    break;
                }
                case SM_AWS_PUB_CAPACITOR:
                {//PUB Capacitor ONLY
                    pubState = SM_AWS_PUB_DONE;
                    break;
                }

                case SM_AWS_PUB_MULTIMETER_ALL:
                    // TODO LAB3 STEP3 E  Add Telemetry for Current, Resistor and Capacitor to original one                   
                {//PUB Volt and Count
                    char tempBuffer[100];
                    extern APP_DATA appData;

                    sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\""PUB_STATE_REPORTED_START, thingID);
                    sprintf(tempBuffer, JSON_IP, valueIP4, valueIP3, valueIP2, valueIP1);
                    strcat(buffer, tempBuffer);
                    strcat(buffer, ",");
                    sprintf(tempBuffer, JSON_COUNT, (int) pubCount++);
                    strcat(buffer, tempBuffer);
                    strcat(buffer, ",");
                    sprintf(tempBuffer, JSON_BUTTON, (SW0) ? "up" : "down");
                    strcat(buffer, tempBuffer);
                    strcat(buffer, ",");
                    sprintf(tempBuffer, JSON_LEDSTATE, appData.LED_user);
                    strcat(buffer, tempBuffer);
                    strcat(buffer, PUB_STATE_REPORTED_END);
                    pubState = SM_AWS_PUB_DONE;
                    break;
                }
                case SM_AWS_PUB_DONE:
                {
                    //After myPUBDelay, select SM_AWS_PUB_MULTIMETER_ALL
                    pubState = SM_AWS_PUB_MULTIMETER_ALL;
                    sprintf(buffer, "");
                    app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
                    myPUBDelay = getTick() + telemetryInterval * SECOND;

                    break;
                }

            }
#endif
#ifdef USE_MOSQUITTO
            sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\""PUB_PAYLODAD"\"\r\n", (int) pubCount++);
            myPUBDelay = getTick() + telemetryInterval * SECOND;
            app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
#endif                    


            //WriteAnyCloud("ATE0\r\n", 6);
            gOK = false;
            WriteAnyCloud((uint8_t*) buffer, strlen(buffer));
            printf_2RIO(("%s", buffer));
            myDelay = getTick();


            break;
        }
        case APP_MQTT_STATE_WAIT_PUB:
        {
            // if no response TIMEOUT

            if ((getTick() - SECOND) > myDelay)
                app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
            if (gOK) {
                //printf_DBG(("PUB OK\r\n\r\n"));
                gOK = false;
                //See if there is more PUB to do!
                app_rio2Data.state = APP_MQTT_STATE_PUBLISH; //APP_RIO2_STATE_SOCKET_IDLE;
                //myPUBDelay = getTick() + telemetryInterval * SECOND;
            }
            break;
        }
#ifdef USE_AZURE
            static char rebootDelay[10];
            static char requestID[10];
#endif
            static char *responseMQTTPUB;
        case APP_MQTT_STATE_RCV_DATA:
        {
            if (!gMQTTSUBRX)
                break;
            gMQTTSUBRX = false;
            //printf_MQTT(("MQTT Receive => %s\r\n", resultPtrMQTTPUB));
            // Parse message for TOPEC
            char *ptr, *topicPtr, *messPtr;
            messPtr = topicPtr = NULL;

            // String receive in resultPtrMQTTPUB is
            //  0,0,0,"Topic","Message"
            //Initialize  topicPtr an messPtr
            ptr = strstr(resultPtrMQTTSUBRX, "\""); //First "
            if (ptr) {
                topicPtr = ptr + 1; //TopicName begin

                ptr = strstr(ptr + 1, ","); //Second ","
                if (ptr) {
                    *ptr = 0; //TopicName end
                    *(ptr - 1) = 0;
                    ptr = strstr(ptr + 1, "\""); //Third ","
                    if (ptr) {
                        messPtr = ptr + 1; //Message Begin
                        *ptr = 0;
                    }
                }
            }

            if (messPtr) {
                printf_MQTT(("\r\nMQTT Topic => %s\r\nMQTT Message => %s\r\n", topicPtr, messPtr));
                gOK = false;
                //sprintf (messPtr,"TOPIC : %s  MESS : %s",topicPtr, messPtr);
                responseMQTTPUB = largeBuffer;
#ifdef USE_AWS
                uint8_t *ptr1;
                //Search in JSON OBJ for KEY:VALUE of "Msg":"A message"
                if ((ptr = strstr(messPtr, "\\\"Msg\\\":"))) {
                    ptr += strlen("\\\"Msg\\\":");
                    if ((ptr = strstr(ptr, "\""))) //Find first "\""
                    {
                        ptr = ptr + 1;
                        if ((ptr1 = strstr(ptr + 1, "\\"))) //Find second "\""
                        {
                            *(ptr1) = 0; //Terminate with NULL

                        }
                    }
                    sprintf(mqttRCVMessage, ptr);
                    printf_DBG(("%s\r\n", mqttRCVMessage));
                    //Init 

                    sprintf(responseMQTTPUB, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\""PUB_STATE_REPORTED_MSG"\"\r\n", thingID, mqttRCVMessage);

                    app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE;
                } else
                    if ((ptr = strstr(messPtr, "\\\"ledstate\\\":"))) {

                    ptr += strlen("\\\"ledstate\\\":");
                    *(strstr(ptr, "}")); //  find } and replace with NULL
                    ledState = atoi(ptr);
                    extern APP_DATA appData;
                    appData.LED_user = ledState;

                    sprintf(responseMQTTPUB, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\""PUB_STATE_REPORTED_LEDSTATE"\"\r\n", thingID, ledState);

                    app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE;
                } else {
                    sprintf(responseMQTTPUB, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC"\",\"{\\\"state\\\":null}\"\r\n", thingID);
                    app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE;
                }
#endif
#ifdef USE_AZURE                
                //+MQTTPUB:35,"$iothub/methods/POST/reboot/?$rid=4",16,"{"delay":"PT5S"}"
                if ((ptr = strstr(messPtr, "\"delay")) && strstr(topicPtr, "$iothub/methods/POST/reboot/")) {
                    // Get reboot delay value     
                    if ((ptr = strstr(messPtr, "PT"))) {
                        ptr += strlen("PT");
                        sprintf(rebootDelay, ptr);
                    }
                    printf_MQTT(("\r\n\r\n\r\n""Rebooting in %u seconds.... Just Kidding!!! :p\r\n\r\n\r\n\r\n", atoi(rebootDelay)));
                    // Get "rid" value
                    if ((ptr = strstr(topicPtr, "$rid="))) {
                        ptr += strlen("$rid=");
                        sprintf(requestID, ptr);
                    }
                    // Prepare response TOPIC and PAYLOAD
                    sprintf(responseMQTTPUB, "AT+MQTTPUB=0,0,0,\"$iothub/methods/res/200/?$rid=%s\",\"{\\\"status\\\":\\\"Success\\\",\\\"delay\\\":\%u\}\"\r\n", requestID, atoi(rebootDelay));
                    app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE;
                    break;
                }

               //Search in JSON OBJ for KEY:VALUE of "echoMsgString":"A message"
                //+MQTTPUB:36,"$iothub/methods/POST/echoMsg/?$rid=1",25,"{"echoMsgString":"Hello World!!!"}"
                if ((ptr = strstr(messPtr, "\\\"echoMsgString\\\":")) && strstr(topicPtr, "methods/POST/echoMsg/")) {
                    ptr += strlen("\\\"echoMsgString\\\":");
                    if ((ptr = strstr(ptr, "\""))) //Find first "\""
                    {

                        *strstr(++ptr, "\\") = 0; //Find second "\\ and NULL it

                    }
                    sprintf(mqttRCVMessage, ptr);
                    printf_MQTT((mqttRCVMessage));
                    //Get "rid" value
                    if ((ptr = strstr(topicPtr, "$rid="))) {
                        ptr += strlen("$rid=");
                        sprintf(requestID, ptr);
                    }
                    //Prepare response TOPIC and PAYLAOD
                    //sprintf(responseMQTTPUB, "AT+MQTTPUB=0,0,0,\"$iothub/methods/res/200/?$rid=%s\",\"{\\\"status\\\":\\\"Success\\\"}\"\r\n", requestID);
                    sprintf(responseMQTTPUB, "AT+MQTTPUB=0,0,0,\"$iothub/methods/res/200/?$rid=%s\",\"{\\\"echoMsgString\\\":\\\"%s\\\"}\"\r\n", requestID, mqttRCVMessage);
                    app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE;
                    break;
                }
                //char jsonPayloadResponse[500];
                char JsonRespStarted;
                JsonRespStarted = false;


                //+MQTTPUB:51,"$iothub/twin/PATCH/properties/desired/?$version=235",46,"{"led_b":2,"led_g":1,"led_r":1,"$version":235}
                if ((strstr(topicPtr, "$iothub/twin/PATCH/properties/desired/"))) {
                    memset(largeBuffer, 0, sizeof (largeBuffer));
                    jsonPayloadResponse = largeBuffer;


                    strcpy(jsonPayloadResponse, "AT+MQTTPUB=0,0,0,\"$iothub/twin/PATCH/properties/reported/?rid=55\",\"{");

                    char version[10];
                    //Get version from TOPIC desired state
                    //+MQTTPUB:51,"$iothub/twin/PATCH/properties/desired/?$version=235",46,"{"led_b":2,"led_g":1,"led_r":1,"$version":235}
                    if ((ptr = strstr(messPtr, "$version\\\":"))) {
                        ptr += strlen("$version\\\":");
                        *strstr(ptr, "}") = 0;
                        strcpy(version, ptr);

                    }
                    if ((ptr = strstr(messPtr, "\\\"led_user\\\":"))) {
                        ptr += strlen("\\\"led_user\\\":");
                        extern APP_DATA appData;

                        if (*ptr == '1') {
                            appData.LED_user = LED_STATE_ON;
                        }
                        if (*ptr == '2') {
                            appData.LED_user = LED_STATE_OFF;
                        }
                        if (*ptr == '3') {
                            appData.LED_user = LED_STATE_BLINKING;
                        }

                        sprintf(buffer, PUB_AZURE_REPORTED_PAYLOAD, "led_user", version, appData.LED_user);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;
                    }

                    if ((ptr = strstr(messPtr, "\"rgb_led_blue\":"))) {
                        ptr += strlen("\"rgb_led_blue\":");
                        //                        if (*ptr == '1')
                        //                            BLUE_LED_Set();
                        //                        else
                        //                            BLUE_LED_Clear();

                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");

                        sprintf(buffer, PUB_AZURE_REPORTED_PAYLOAD, "rgb_led_blue", version, USER_LED == 0 ? 0 : 100);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;

                    }
                    if ((ptr = strstr(messPtr, "\"rgb_led_red\":"))) {
                        ptr += strlen("\"rgb_led_red\":");
                        //                        if (*ptr == '1')
                        //                            RED_LED_Set();
                        //                        else
                        //                            RED_LED_Clear();

                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");
                        sprintf(buffer, PUB_AZURE_REPORTED_PAYLOAD, "rgb_led_red", version, USER_LED == 0 ? 0 : 100);
                        strcat(jsonPayloadResponse, buffer);

                        JsonRespStarted = true;

                    }
                    if ((ptr = strstr(messPtr, "\"rgb_led_green\":"))) {
                        ptr += strlen("\"rgb_led_green\":");
                        //                        if (*ptr == '1')
                        //                            GREEN_LED_Set();
                        //                        else
                        //                            GREEN_LED_Clear();


                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");
                        sprintf(buffer, PUB_AZURE_REPORTED_PAYLOAD, "rgb_led_green", version, USER_LED == 0 ? 0 : 100);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;

                    }

                    //Do that last since you are replacing "," by NULL
                    if ((ptr = strstr(messPtr, "\\\"telemetryInterval\\\":"))) {
                        ptr += strlen("\\\"telemetryInterval\\\":");
                        *strstr(ptr, ",") = 0; //


                        telemetryInterval = atoi(ptr);
                        if (telemetryInterval < 1)
                            telemetryInterval = 1;
                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");
                        sprintf(buffer, PUB_AZURE_REPORTED_PAYLOAD, "telemetryInterval", version, telemetryInterval);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;

                    }

                    //strcat(jsonPayloadResponse,",\\\"telemetryInterval\\\":{\\\"ac\\\":200,\\\"av\\\":5,\\\"ad\\\":\\\"success\\\",\\\"value\\\":5}");

                    strcat(jsonPayloadResponse, "}\"\r\n");
                    responseMQTTPUB = jsonPayloadResponse;
                    app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE;
                    break;
                }
                //Update TWIN GET desired state

                //+MQTTPUB:31,"$iothub/twin/res/200/?$rid=getTwin",355,
                //"{"desired":{"led_b":2,"led_g":2,"led_r":2,"telemetryInterval":60,"$version":447},
                //"reported":{
                //"ipAddress":"192.168.1.103",
                //"led_b":2,"led_g":2,"led_r":2,
                //"telemetryInterval":{"ac":200,"av":447,"ad":"success","value":60},
                //"temperature":{"ac":200,"av":5,"ad":"success","value":25},
                //"WBZ451Message":"WBZ451 multimeter Voltage is -2953.5 mVolt!","$version":68679}}"


                /* This is in response of  MQTTPUB get TWIN value  
                 AT+MQTTPUB=0,0,0,\"$iothub/twin/GET/?$rid=getTwin
                 Notice same rid value getTwin
                 */
                char *ptrReported;
                ptrReported = strstr(messPtr, "\\\"reported\\\"");

                if ((ptr = strstr(messPtr, "\\\"desired\\\"")) && strstr(topicPtr, "$iothub/twin/res/200/?$rid=getTwin")) {
                    memset(largeBuffer, 0, sizeof (largeBuffer));
                    jsonPayloadResponse = largeBuffer;

                    strcpy(jsonPayloadResponse, "AT+MQTTPUB=0,0,0,\"$iothub/twin/PATCH/properties/reported/?rid=postTwin\",\"{");


                    if ((ptr = strstr(messPtr, "\\\"rgb_led_blue\\\":"))) {
                        ptr += strlen("\\\"rgb_led_blue\\\":");
                        //                        if (*ptr == '1')
                        //                            BLUE_LED_Set();
                        //                        else
                        //                            BLUE_LED_Clear();

                        sprintf(buffer, PUB_AZURE_DESIRED_VALUE_PAYLOAD, "rgb_led_blue", USER_LED == 0 ? 0 : 100);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;

                    }
                    if ((ptr = strstr(messPtr, "\\\"rgb_led_red\\\":"))) {
                        ptr += strlen("\\\"rgb_led_red\\\":");
                        //                        if (*ptr == '1')
                        //                            RED_LED_Set();
                        //                        else
                        //                            RED_LED_Clear();

                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");
                        sprintf(buffer, PUB_AZURE_DESIRED_VALUE_PAYLOAD, "rgb_led_red", USER_LED == 0 ? 0 : 100);
                        strcat(jsonPayloadResponse, buffer);

                        JsonRespStarted = true;

                    }
                    if ((ptr = strstr(messPtr, "\\\"rgb_led_green\\\":"))) {
                        ptr += strlen("\\\"rgb_led_green\\\":");
                        //                        if (*ptr == '1')
                        //                            GREEN_LED_Set();
                        //                        else
                        //                            GREEN_LED_Clear();


                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");
                        sprintf(buffer, PUB_AZURE_DESIRED_VALUE_PAYLOAD, "rgb_led_green", USER_LED == 0 ? 0 : 100);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;

                    }
                    if ((ptr = strstr(messPtr, "\\\"led_user\\\":"))) {
                        ptr += strlen("\\\"led_user\\\":");
                        extern APP_DATA appData;
                        if (*ptr == '1') {
                            appData.LED_user = LED_STATE_ON;
                        }
                        if (*ptr == '2') {
                            appData.LED_user = LED_STATE_OFF;
                        }
                        if (*ptr == '3') {
                            appData.LED_user = LED_STATE_BLINKING;
                        }

                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");
                        sprintf(buffer, PUB_AZURE_DESIRED_VALUE_PAYLOAD, "led_user", appData.LED_user);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;
                    }
                    //Do that last since you are replacing "," by NULL
                    if ((ptr = strstr(messPtr, "\\\"telemetryInterval\\\":"))) {
                        if (ptr < ptrReported) {
                            ptr += strlen("\\\"telemetryInterval\\\":");
                            *strstr(ptr, ",") = 0;
                            telemetryInterval = atoi(ptr);
                        }
                        if (JsonRespStarted)
                            strcat(jsonPayloadResponse, ",");
                        sprintf(buffer, PUB_AZURE_DESIRED_VALUE_PAYLOAD, "telemetryInterval", telemetryInterval);
                        strcat(jsonPayloadResponse, buffer);
                        JsonRespStarted = true;

                    }
                    //strcat(jsonPayloadResponse,",\\\"telemetryInterval\\\":{\\\"ac\\\":200,\\\"av\\\":5,\\\"ad\\\":\\\"success\\\",\\\"value\\\":5}");

                    strcat(jsonPayloadResponse, "}\"\r\n");
                    responseMQTTPUB = jsonPayloadResponse;
                    app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE;
                    break;

                }

                app_rio2Data.state = APP_MQTT_STATE_PUB_RESPONSE; //APP_RIO2_STATE_SOCKET_IDLE; // APP_MQTT_STATE_PUB_RESPONSE;
#endif
#ifdef USE_MOSQUITTO                //Mosquitto
                sprintf(mqttRCVMessage, messPtr);
                printf_MQTT((mqttRCVMessage));
                app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;
#endif                


            } else
                app_rio2Data.state = APP_RIO2_STATE_SOCKET_IDLE;




            break;
        }
        case APP_MQTT_STATE_PUB_RESPONSE:
        {
            //char buffer[100];

            //sprintf(buffer,responseMQTTPUB);

            WriteAnyCloud((uint8_t*) responseMQTTPUB, strlen(responseMQTTPUB));
            printf_2RIO(("%s", responseMQTTPUB));
            myDelay = getTick();
            app_rio2Data.state = APP_MQTT_STATE_WAIT_PUB; //APP_RIO2_STATE_SOCKET_IDLE;//APP_MQTT_STATE_WAIT_PUB;
            break;
        }
        case APP_MQTT_STATE_DISCONNECTED:
        {
            gMQTTCONN = false;
            printf_MQTT(("MQTT Disconnected\r\n"));
#ifdef USE_MOSQUITTO
            //Change clientID before reconnecting
            sprintf(clientID, "%s%d", CLIENTID, rand());
#endif
#ifdef USE_AZURE
            //reinit MQTT Config
            strcpy(broker, BROKER);
            strcpy(brokerUserName, BROKER_USER_NAME);
#endif
            app_rio2Data.state = APP_MQTT_STATE_INIT_MQTT;
            gOK = true; // Trigger first MQTT Init packet send;

            break;
        }
#endif
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

/*******************************************************************************
 End of File
 */
