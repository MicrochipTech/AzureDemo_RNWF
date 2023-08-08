/**
 * TCA1 Generated Driver API Header File
 *
 * @file tca1.h
 *
 * @defgroup tca1_normal TCA1 in Normal Mode
 *
 * @brief This file contains the API prototypes for the TCA1 driver in Normal (16-bit) mode.
 *
 * @version TCA1 Driver Version 2.1.1
*/
/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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


#ifndef TCA1_H_INCLUDED
#define TCA1_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include "../system/system.h"
#include "timer_interface.h"
#include "../system/utils/compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup tca1_normal
 * @typedef void TCA1_cb_t
 * @brief Function pointer to the callback function called by TCA when operating in Normal mode. The default value is set to NULL which means that no callback function will be used.
 */  
typedef void (*TCA1_cb_t)(void);

extern const struct TMR_INTERFACE TCA1_Interface;

/**
 * @ingroup tca1_normal
 * @brief Initializes the TCA.
 * @param None.
 * @return None.
 */ 
void TCA1_Initialize(void);
/**
 * @ingroup tca1_normal
 * @brief Starts the 16-bit timer/counter for the TCA.
 * @param None.
 * @return None.
 */
void TCA1_Start(void);
/**
 * @ingroup tca1_normal
 * @brief Stops the 16-bit timer/counter for the TCA.
 * @param None.
 * @return None.
 */
void TCA1_Stop(void);
/**
 * @ingroup tca1_normal
 * @brief Interrupt Service Routine (ISR) callback function register to be called if the Overflow Interrupt flag is set.
 * @param TCA1_cb_t cb - Callback function for Overflow event.
 * @return None.
 */ 
void TCA1_OverflowCallbackRegister(TCA1_cb_t cb);
/**
 * @ingroup tca1_normal
 * @brief ISR callback function to be called if the Compare 0 Match Interrupt flag is set.
 * @param TCA1_cb_t cb - Callback function for Compare 0 match event.
 * @return None.
 */ 
void TCA1_Compare0CallbackRegister(TCA1_cb_t cb);
/**
 * @ingroup tca1_normal
 * @brief ISR callback function to be called if the Compare 1 Match Interrupt flag is set.
 * @param TCA1_cb_t cb - Callback function for Compare 1 match event.
 * @return None.
 */ 
void TCA1_Compare1CallbackRegister(TCA1_cb_t cb);
/**
 * @ingroup tca1_normal
 * @brief ISR callback function to be called if the Compare 2 Match Interrupt flag is set.
 * @param TCA1_cb_t cb - Callback function for Compare 2 match event.
 * @return None.
 */ 
void TCA1_Compare2CallbackRegister(TCA1_cb_t cb);
/**
 * @ingroup tca1_normal
 * @brief Enables the 16-bit timer/counter interrupt for the TCA.
 * @param None.
 * @return None.
 */
void TCA1_EnableInterrupt(void);
/**
 * @ingroup tca1_normal
 * @brief Disables the 16-bit timer/counter interrupt for the TCA.
 * @param None.
 * @return None.
 */
void TCA1_DisableInterrupt(void);
/**
 * @ingroup tca1_normal
 * @brief Reads the 16-bit timer/counter value for the TCA.
 * @param None.
 * @return uint16_t - timer/counter value returns from the TCA1.
 */
uint16_t TCA1_Read(void);
/**
 * @ingroup tca1_normal
 * @brief Writes the timer value to load to the TCA.
 * @param uint16_t timerVal - Loading the timer value for the TCA.
 * @return None.
 */
void TCA1_Write(uint16_t timerVal);
/**
 * @ingroup tca1_normal
 * @brief Clears the Overflow Interrupt flag after the Overflow flag set.
 * @param None.
 * @return None.
 */
void TCA1_ClearOverflowInterruptFlag(void);
/**
 * @ingroup tca1_normal
 * @brief Checks the Overflow Interrupt flag status for the TCA.
 * @param None.
 * @retval True  - Overflow Interrupt flag is set.
 * @retval False - Overflow Interrupt flag is not set.
 */
bool TCA1_IsOverflowInterruptFlagSet(void);
/**
 * @ingroup tca1_normal
 * @brief Clears the Compare 0 Interrupt flag after the Compare 0 flag is set.
 * @param None.
 * @return None.
 */
void TCA1_ClearCMP0InterruptFlag(void);
/**
 * @ingroup tca1_normal
 * @brief Checks the Compare 0 Interrupt flag status for the TCA.
 * @param None.
 * @retval True  - Compare 0 Interrupt flag is set.
 * @retval False - Compare 0 Interrupt flag is not set.
 */
bool TCA1_IsCMP0InterruptFlagSet(void);
/**
 * @ingroup tca1_normal
 * @brief Clears the Compare 1 Interrupt flag after the Compare 1 flag is set.
 * @param None.
 * @return None.
 */
void TCA1_ClearCMP1InterruptFlag(void);
/**
 * @ingroup tca1_normal
 * @brief Checks the Compare 1 Interrupt flag status for the TCA.
 * @param None.
 * @retval True  - Compare 1 Interrupt flag is set.
 * @retval False - Compare 1 Interrupt flag is not set.
 */
bool TCA1_IsCMP1InterruptFlagSet(void);
/**
 * @ingroup tca1_normal
 * @brief Clears the Compare 2 Interrupt flag after the Compare 2 flag is set.
 * @param None.
 * @return None.
 */
void TCA1_ClearCMP2InterruptFlag(void);

/**
 * @ingroup tca1_normal
 * @brief Checks the Compare 2 Interrupt flag status for the TCA.
 * @param None.
 * @retval True  - Compare 2 Interrupt flag is set.
 * @retval False - Compare 2 Interrupt flag is not set.
 */
bool TCA1_IsCMP2InterruptFlagSet(void);

#ifdef __cplusplus
}
#endif

#endif /* TCA1_H_INCLUDED */