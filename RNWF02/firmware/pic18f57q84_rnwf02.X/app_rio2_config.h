/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    app_rio2_config.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H
// TODO: LAB1 STEP2 C  Ensure  PASSTROUGH is NOT commented
// TODO: LAB2 STEP2 A.1  Comment PASSTROUGH
//#define PASSTHROUGH
//#define SOLUTION
uint8_t dummyFlag, dummyArray[1];
// TODO  LAB2 STEP2 A.2 Enter you SSID and PASSPHRASE

#define WIFI_SSID "<MY_WIFI_NETWORK_SSID>"
#define WIFI_PSWD "<MY_WIFI_NETWORK_PASSWORD>"

// TODO  LAB3 STEP2 B Enable USE_AWS Cloud service
//// ONLY #define ONE MQTT Cloud service

//#define USE_MOSQUITTO
//#define USE_AWS
#define USE_AZURE

//ANSI color code 
//https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124

// Enable next #define ANSI if your terminal emulator supports COLOR
//#define ANSI
/*********ANSI COLOR************/
#define REG_BLACK   "\e[0;30m"
#define REG_RED     "\e[0;31m"
#define REG_GREEN   "\e[0;32m"
#define REG_YELLOW  "\e[0;33m"
#define REG_BLUE    "\e[0;34m"
#define REG_PURPLE  "\e[0;35m"
#define REG_CYAN    "\e[0;36m"
#define REG_WHITE   "\e[0;37m" 

#define BOLD_BLACK   "\e[1;30m"
#define BOLD_RED     "\e[1;31m"
#define BOLD_GREEN   "\e[1;32m"
#define BOLD_YELLOW  "\e[1;33m"
#define BOLD_BLUE    "\e[1;34m"
#define BOLD_PURPLE  "\e[1;35m"
#define BOLD_CYAN    "\e[1;36m"
#define BOLD_WHITE   "\e[1;37m" 

#define BKG_BLACK   "\e[40m"
#define BKG_WHITE   "\e[47m"
/*********ANSI COLOR************/


#define FRIO2_S "<FRIO2>"
#define FRIO2_E /*"</FRIO2>"*/
#define TORIO_S "<2RIO>"
#define TORIO_E /*"</2RIO>"*/
#define DBG "<DBG>"
#define MQTT "<MQTT>"
#define CALLBACK "<CB>"

#ifndef PASSTHROUGH
#ifndef ANSI
#define printf_TST(x) printf ("<TST>");printf x
#define printf_DBG(x) printf (DBG);printf x
#define printf_CB(x)   //printf (CALLBACK);printf x
#define printf_FRIO(x) printf ( FRIO2_S);printf x
#define printf_2RIO(x) printf ( TORIO_S);printf x
#define printf_MQTT(x) printf ( MQTT);printf x
#define printf_BLACK(x) printf x
#define printf_WR2TERMINAL SERCOM0_USART_Write
#else
#define printf_TST(x) printf (BOLD_RED);printf x
#define printf_DBG(x) printf (BOLD_BLACK DBG );printf x
#define printf_CB(x)   //printf (REG_RED CALLBACK);printf x
#define printf_FRIO(x) printf ( BOLD_PURPLE);printf x
#define printf_2RIO(x) printf ( BOLD_BLUE);printf x
#define printf_MQTT(x) printf ( BOLD_GREEN MQTT );printf x
#define printf_BLACK(x) printf ( BOLD_BLACK );printf x
#define printf_DesignWeek(x) printf ( BOLD_RED "PRESS SW2 to Continue" );printf x

#define printf_WR2TERMINAL SERCOM0_USART_Write
#endif
#else
#define printf_DesignWeek(x) printf x
#define printf_BLACK(x) printf x
#define printf_TST(x) printf x
#define printf_DBG(x) printf x
#define printf_CB(x)  // printf x
#define printf_FRIO(x) printf x
#define printf_2RIO(x) printf x
#define printf_MQTT(x) printf x
#define printf_WR2TERMINAL SERCOM0_USART_Write
#endif

typedef enum {
    LED_STATE_INIT = 0,
    LED_STATE_ON = 1,
    LED_STATE_OFF = 2,
    LED_STATE_BLINKING = 3,
} LED_STATE;

typedef enum {
    /* Application's state machine's initial state. */
    APP_STATE_INIT = 0,
    APP_STATE_SERVICE_TASKS,
    APP_STATE_DISPLAY_CERT,
            
    /* TODO: Define states used by the application state machine. */

} APP_STATES;

typedef struct {
    /* The application's current state */
    APP_STATES state;

    /* TODO: Define any additional data used by the application. */
    LED_STATE LED_user;

} APP_DATA;

typedef enum {
    /* Application's state machine's initial state. */
    APP_RIO2_STATE_INIT = 0,
    APP_RIO2_STATE_WAIT_RST,
    APP_RIO2_STATE_ECHO_OFF,
    APP_RIO2_STATE_GET_VERSION,
    APP_RIO2_STATE_CONFIG_AP,
    APP_RIO2_STATE_CONFIG_AP_OK,
    APP_RIO2_STATE_CONNECT_STA,
    APP_RIO2_STATE_WAIT_IP,
    APP_RIO2_STATE_UDP_SOCKET_CREATE,
    APP_RIO2_STATE_UDP_SOCKET_GET,
    APP_RIO2_STATE_UDP_SOCKET_BIND,
    APP_RIO2_STATE_UDP_SOCKET_RCV_DATA,
    APP_RIO2_STATE_UDP_SOCKET_SEND_DATA,
    APP_RIO2_STATE_RESOLVE_DNS,
    APP_RIO2_STATE_WAIT_RESOLVE_DNS,
    APP_RIO2_STATE_TCP_SOCKET_CREATE,
    APP_RIO2_STATE_TCP_SOCKET_BIND,
    APP_RIO2_STATE_TCP_SOCKET_BINDED_REMOTE,
    APP_RIO2_STATE_TCP_SOCKET_BINDED,
    APP_RIO2_STATE_TCP_SOCKET_CONNECTED,
    APP_MQTT_STATE_INIT_MQTT,
    APP_MQTT_STATE_INIT_MQTT_OK,
    APP_MQTT_STATE_CONNECTED,
    APP_MQTT_STATE_RCV_DATA,
    APP_MQTT_STATE_PUB_RESPONSE,
    APP_MQTT_STATE_SUBSCRIBE,
    APP_MQTT_STATE_SUBSCRIBE_COMPLETE,
    APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_PUT,
    APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_GET,
    APP_RIO2_STATE_AZURE_GET_ASSIGN_HUB_AND_DISCONNECT,
    APP_RIO2_STATE_AZURE_WAIT_DISCONNECT,
    APP_RIO2_STATE_AZURE_RECONNECT,
    APP_RIO2_STATE_AZURE_SUBSCRIBE_1,
    APP_RIO2_STATE_AZURE_SUBSCRIBE_2,
    APP_RIO2_STATE_AZURE_SUBSCRIBE_3,
    APP_RIO2_STATE_AZURE_SUBSCRIBE_DONE,
    APP_MQTT_STATE_PUBLISH,
    APP_MQTT_STATE_WAIT_PUB,
    APP_MQTT_STATE_DISCONNECTED,
    APP_RIO2_STATE_TCP_SOCKET_RCV_DATA,
    APP_RIO2_STATE_TCP_SOCKET_READ_DATA,
    APP_RIO2_STATE_TCP_SOCKET_WRITE,
    APP_RIO2_STATE_TCP_SOCKET_CLOSE,
    APP_RIO2_STATE_TCP_SOCKET_WAIT_CLOSE,


    APP_RIO2_STATE_SOCKET_IDLE,


    /* TODO: Define states used by the application state machine. */

} APP_RIO2_STATES;

typedef struct {
    /* The application's current state */
    APP_RIO2_STATES state;

    /* TODO: Define any additional data used by the application. */

} APP_RIO2_DATA;

APP_RIO2_DATA app_rio2Data;

#define SEC_TYPE "3" //"3"





#define BROKER_USER_PWD "myUserPWD"
#define KEEPALIVE "60"


#ifdef USE_MOSQUITTO
#define HOSTNAME "RIO2-MOSQ-LucA" /* HostName KEEP it SHORT*/ 
#define MY_THING_ID "WFI32_AnyCloud"
#define CLIENTID MY_THING_ID

#define MQTTCLIENT
#define BROKER "test.mosquitto.org"
#define PORT "1883"  //TLS PORT 8886 uses ISRGRootX1.pem from Free CA signer "Let's Encrypt"  https://letsencrypt.org/certificates/
#define TLSENABLE "0"

#define SUB_TOPIC MY_THING_ID"SUB/#"
#define PUB_TOPIC MY_THING_ID"PUB"
#define PUB_PAYLODAD "count => %d"
#define PUB_REPORTED_MSG ""

#else
#ifdef USE_AWS
// When using AWS IoT, you need to retrieve the Device Cert from the WFI32_Anycloud
// To do this,  type AT+READCERT=1  
// C&P response and save into DeviceCert.cer
//  Ensure to replace all "\r" with real CRLF !!!!!!

//TODO LAB3 STEP2 B Change  MY_THING_ID & BROKER for yours!
#define MY_THING_ID "sn01234B9DEC867FBB01"// WFI32 IoT  //"sn0123C2DB36BBC7C501"//"sn0123BF8C8062EBEF01" F01 is IoT Board, 501 is Curiosity board
#define BROKER "a3csywf8mmivy-ats.iot.us-east-2.amazonaws.com"

#define MQTTCLIENT
//AWS IoT account alexa.mchp1@gmail.com  Ohio us-east-2
#define PORT "8883"  //TLS PORT 8886 uses ISRGRootX1.pem from Free CA signer "Let's Encrypt"  https://letsencrypt.org/certificates/
#define TLSENABLE "1"
#define HOSTNAME "RIO2-AWS-LucA" /* HostName KEEP it SHORT*/ 

#define CLIENTID MY_THING_ID

#define SUB_TOPIC "$aws/things/%s/shadow/update/delta"
#define PUB_TOPIC "$aws/things/%s/shadow/update"
#define PUB_PAYLODAD "{\\\"state\\\":{\\\"reported\\\": {\\\"count\\\": %d}}}"
#define PUB_STATE_REPORTED_MSG "{\\\"state\\\":{\\\"reported\\\": {\\\"Msg\\\": \\\"%s\\\"}}}"
#define PUB_STATE_REPORTED_LEDSTATE "{\\\"state\\\":{\\\"reported\\\": {\\\"ledstate\\\":%d}}}"
#define PUB_STATE_REPORTED_START "{\\\"state\\\":{\\\"reported\\\": {"
#define PUB_STATE_REPORTED_END "}}}\"\r\n"
#define JSON_IP "\\\"IP\\\":  \\\"%s:%s:%s:%s\\\""
#define JSON_VOLTAGE "\\\"Voltage in mV\\\": %.1f"
#define JSON_BUTTON "\\\"SW0\\\": \\\"%s\\\""
#define JSON_LEDSTATE "\\\"ledstate\\\": %d"
#define JSON_COUNT "\\\"count\\\": %d"

#define PUB_STATE_REPORTED_IP "{\\\"state\\\":{\\\"reported\\\": {\\\"IP\\\": \\\"%s.%s.%s.%s\\\"}}}"
#define PUB_STATE_REPORTED_VOLTAGE "{\\\"state\\\":{\\\"reported\\\": {\\\"Volt\\\": %.1f}}}"
#else
#ifdef USE_AZURE
// When using Azure IoT, you need to retrieve the Signer Cert from the WFI32_Anycloud
// To do this,  type AT+READCERT=2  
// C&P response and save into DeviceCert.cer
//  Ensure to replace all "\r" with real CRLF !!!!!!

#define HOSTNAME "RIO2-AZURE-LucA" /* HostName KEEP it SHORT*/ 

#define MQTTCLIENT
//Azure IoT Central
#define BROKER "global.azure-devices-provisioning.net"
#define PORT "8883"  //TLS PORT 8886 uses ISRGRootX1.pem from Free CA signer "Let's Encrypt"  https://letsencrypt.org/certificates/
#define TLSENABLE "1"

#define MY_THING_ID "<MY_COMMON_NAME>" // e.g. "rnwf02_device_01"
#define ID_SCOPE "<MY_ID_SCOPE>" // e.g. "0ne000FFA42"
#define MODEL_ID "dtmi:com:Microchip:PIC18F57Q84_CNANO;1"

#define CLIENTID MY_THING_ID
#define RNWF_DEVICE_CERT MY_THING_ID
#define RNWF_DEVICE_KEY MY_THING_ID

#define BROKER_USER_NAME ID_SCOPE"/registrations/"CLIENTID"/api-version=2019-03-31"
#define SUB_DPS_REGISTRATION_TOPIC "$dps/registrations/res/#"
#define PUB_TOPIC_PROPERTIES "$iothub/twin/PATCH/properties/reported/?$rid=1"
//#define PUB_PAYLODAD_MESSSAGE "{\\\"WBZ451Message\\\":\\\"Hello from WBZ451 count => %d!!\\\"}"
#define PUB_PAYLOAD_IP "{\\\"ipAddress\\\":\\\"%s.%s.%s.%s\\\"}"
#define PUB_TOPIC_TELEMETRY "devices/%s/messages/events/"
#define PUB_AZURE_PAYLOAD_TELEMETRY_TEMPERATURE "{\\\"temperature\\\":%2.2f}"
#define PUB_AZURE_PAYLOAD_TELEMETRY_COUNTER "{\\\"counter\\\":%d}"
#define PUB_TOPIC_DPS_PUT "$dps/registrations/PUT/iotdps-register/?rid=1"
#define PUB_TOPIC_DPS_GET "$dps/registrations/GET/iotdps-get-operationstatus/?rid=2&operationId=%s"
#define PUB_REPORTED_PAYLOAD "{\\\"payload\\\" : {\\\"modelId\\\" : \\\""MODEL_ID"\\\"}}"
#define PUB_REPORTED_MSG ""
//#define PUB_AZURE_PAYLODAD "{\\\"ipAdress\\\":{\\\"reported\\\":{\\\"value\\\":\\\"Hello from WBZ451 count => %d!!\\\"}}}"
//#define PUB_AZURE_PAYLODAD "{\\\"ipAddress\\\":{\\\"ac\\\":200,\\\"av\\\":26,\\\"ad\\\":\\\"success\\\",\\\"value\\\":\\\"Hello from WBZ451 count => %d!!\\\"}}"
//#define PUB_AZURE_PAYLODAD "{\\\"ipAddress\\\":\\\"Hello from WBZ451 count => %d!!\\\"}"
//#define PUB_AZURE_PAYLODAD "{\\\"rcvMsg\\\":{\\\"ac\\\":200,\\\"av\\\":26,\\\"ad\\\":\\\"success\\\",\\\"value\\\":\\\"WBZ451 count => %d!!\\\"}}"
//#define PUB_AZURE_REPORTED_PAYLOAD "\\\"%s\\\":{\\\"ac\\\":200,\\\"av\\\":%s,\\\"ad\\\":\\\"success\\\",\\\"value\\\":%d}"
#define PUB_AZURE_REPORTED_PAYLOAD "\\\"%s\\\":{\\\"ac\\\":200,\\\"av\\\":%s,\\\"value\\\":%d}"
#define PUB_AZURE_REPORTED_VALUE_PAYLOAD "\\\"%s\\\":{\\\"value\\\":%d}"
#define PUB_AZURE_DESIRED_VALUE_PAYLOAD "\\\"%s\\\":%d"
//#define PUB_AZURE_REPORTED_PAYLODAD "\\\"%s\\\":{\\\"ac\\\":200,\\\"av\\\":%s,\\\"value\\\":%d}"


#else  //MQTT is disable
//No MQTT

#define HOSTNAME "AnyCloud_me" /* HostName KEEP it SHORT*/ 
#define CLIENTID ""
#define BROKER ""
#define PORT ""  
#define TLSENABLE ""
#define SUB_TOPIC ""
#define PUB_TOPIC ""
#define PUB_PAYLODAD ""
#define MY_THING_ID ""

#endif
#endif
#endif


// DO NOT change anything below
// DO NOT change anything below
// DO NOT change anything below
// DO NOT change anything below
// DO NOT change anything below

#define BIN_WRITE_CHUNK 500  //512 is the MAX RX buffer on the RIO2 !!!!
#define HEX_WRITE_CHUNK 100


#ifndef TCPSENDHEX
#define TCPSENDBIN
#endif

#ifndef UDPSENDHEX
#define UDPSENDBIN
#endif



#define SCANRESPONSE "HTTP/1.1 200\r\nContent-Type: text/xml\r\nConnection: close\r\n\r\n{\"results\":[%s]}"
#define SCANSINGLEJSON "{\"name\":\"%s\",\"mac\":\"%s\",\"rssi\":\"%s\"}"


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
