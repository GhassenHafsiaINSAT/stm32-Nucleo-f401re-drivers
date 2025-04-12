/*
 * rcc.h
 *
 *  Created on: Mar 22, 2025
 *      Author: ghass
 */

#ifndef RCC_H_
#define RCC_H_

#include "NUCLEOf401_mem_map.h"

//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);

uint32_t RCC_GetPLLOutputClock(void);

#endif /* RCC_H_ */
