/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

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
#include <stdbool.h>
#include "stdio.h"

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
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/


APP_DATA appData;



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

/**********************************************
 * Read Temperature Sensor value
 **********************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

uint32_t tick;
#define SECOND 1000


void sysTick(void) {
    tick++;

}
uint32_t getTick(void){
    return tick;
}



void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    printf(("PIC18F AnyCloud \r\n"));
    
    
    Timer0_OverflowCallbackRegister(sysTick);


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}



/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void WriteAnyCloud(uint8_t* buffer, uint16_t length);

#define USER_LED (RF3)
void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            
           if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
                appData.LED_user = LED_STATE_BLINKING;
            }
            break;
        }
#define SW0 (RA3)
        case APP_STATE_SERVICE_TASKS:
        {
            static uint32_t myTick,buttonTick = -1;
            static bool buttonPress, once;
            //static bool state;
            
            if ((getTick() > myTick + (SECOND / 2)))
            {
                switch ( appData.LED_user)
                {
                    case LED_STATE_OFF:  USER_LED = 1;break;
                    case LED_STATE_ON:  USER_LED = 0;break;
                    case LED_STATE_BLINKING:  USER_LED ^= 1;break;
                    default:  USER_LED ^= 1;break;
                }
               myTick = getTick();
            }

            break;
        }
        case APP_STATE_DISPLAY_CERT:
        {
            
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
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
