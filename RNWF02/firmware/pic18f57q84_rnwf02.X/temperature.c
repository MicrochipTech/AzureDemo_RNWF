
#include "mcc_generated_files/adc/adc.h"
#include "mcc_generated_files/memory/memory.h"
#include "mcc_generated_files/system/system.h"
#include "app_rio2_config.h"
#include "temperature.h"
#include "string.h"
#include <stdbool.h>

int16_t TEMPERATURE_gain;
int16_t TEMPERATURE_offset;

void TEMPERATURE_init(void)
{
    TEMPERATURE_gain = FLASH_ReadWord(DIA_TSHR1);  
    TEMPERATURE_offset = FLASH_ReadWord(DIA_TSHR3);
}

float TEMPERATURE_readDegC(void)
{
    int24_t temp_c = 0;  
    uint16_t adcMeasurement = 0;
    float TEMPERATURE_degC;

    ADC_DischargeSampleCapacitor();
    adcMeasurement = ADC_GetSingleConversion(channel_Temp);
#ifdef _DEBUG_TEMPERATURE
    printf("*** Starting ADC Conversion *** \r\n");
    printf("> Value of gain: %d \r\n", TEMPERATURE_gain);
    printf("> offset: %d \r\n", TEMPERATURE_offset);
    printf("> ADC measurement: %d \r\n", adcMeasurement);
#endif /* _DEBUG_TEMPERATURE */
    temp_c = ((int24_t)(adcMeasurement) * TEMPERATURE_gain);
#ifdef _DEBUG_TEMPERATURE
    printf("> ADC x gain: %d \r\n", temp_c);
#endif /* _DEBUG_TEMPERATURE */
    temp_c = (temp_c / 256);
#ifdef _DEBUG_TEMPERATURE
    printf("> (data/256) = %d \r\n", temp_c);
#endif /* _DEBUG_TEMPERATURE */
    temp_c = (temp_c + TEMPERATURE_offset);
#ifdef _DEBUG_TEMPERATURE
    printf("> Add offset: %d \r\n", temp_c);
#endif /* _DEBUG_TEMPERATURE */
    TEMPERATURE_degC = temp_c;
    TEMPERATURE_degC = (TEMPERATURE_degC / 10.0);
#ifdef _DEBUG_TEMPERATURE
    printf("> Temperature Module Indicator: %.2fC (%.2fF)\r\n", TEMPERATURE_degC, TEMPERATURE_cnvtDegF(TEMPERATURE_degC));
    printf("------------------------------------------ \r\n");
#endif /* _DEBUG_TEMPERATURE */
    return (TEMPERATURE_degC);
}

float TEMPERATURE_cnvtDegF(float temperatureDegC)
{
    return DegF(temperatureDegC);
}