/* 
 * File:   temperature.h
 * Author: c14166
 *
 * Created on April 19, 2023, 5:59 PM
 */

#ifndef TEMPERATURE_H
#define	TEMPERATURE_H

#ifdef	__cplusplus
extern "C" {
#endif

//#define _DEBUG_TEMPERATURE

#define DegF(DegC) (((DegC) * 1.8) + 32.0)

void TEMPERATURE_init(void);
float TEMPERATURE_readDegC(void);
float TEMPERATURE_cnvtDegF(float temperatureDegC);

#ifdef	__cplusplus
}
#endif

#endif	/* TEMPERATURE_H */

