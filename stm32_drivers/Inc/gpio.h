/*
 * gpio.h
 *
 *  Created on: Feb 15, 2025
 *      Author: ghassen
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdint.h>
#include "NUCLEOf401_mem_map.h"

typedef struct {
	uint32_t GPIO_PIN;
	uint32_t GPIO_Mode;
    uint32_t GPIO_Otyper;
    uint32_t GPIO_Ospeeder;
	uint32_t GPIO_PullPush;
	uint8_t GPIO_AF;
} GPIO_PinConfig_t;

typedef struct {
	GPIO_Typdef_t* GPIOx;
	GPIO_PinConfig_t gpio_config;
} GPIO_Handle_t;


#define GPIO_MODE_INPUT 				0x00000000
#define GPIO_MODE_OUTPUT 				0x00000001
#define GPIO_MODE_ALTERNATE 			0x00000002
#define GPIO_MODE_ANALOG 				0x00000003
#define GPIO_MODE_IT_RAISING			1
#define GPIO_MODE_IT_FALLING			2
#define GPIO_MODE_IT_RAISING_FALLING	3

#define OUTPUT_PUSH_PULL 				0x00000000
#define OUTPUT_OPEN_DRAIN 				0x00000001

#define LOW_SPEED 						0x00000000
#define MEDIUM_SPEED 					0x00000001
#define HIGH_SPEED 						0x00000002
#define VERY_HIGH_MODE 					0x00000003

#define GPIO_PullPush_NoPull 			0x00000000
#define PULL_UP 						0x00000001
#define PULL_DOWN 						0x00000002

#define GPIO_AF_0		((uint8_t)0x00)
#define GPIO_AF_1		((uint8_t)0x01)
#define GPIO_AF_2		((uint8_t)0x02)
#define GPIO_AF_3		((uint8_t)0x03)
#define GPIO_AF_4		((uint8_t)0x04)
#define GPIO_AF_5		((uint8_t)0x05)
#define GPIO_AF_6		((uint8_t)0x06)
#define GPIO_AF_7		((uint8_t)0x07)
#define GPIO_AF_8		((uint8_t)0x08)
#define GPIO_AF_9		((uint8_t)0x09)
#define GPIO_AF_10		((uint8_t)0x0A)
#define GPIO_AF_11		((uint8_t)0x0B)
#define GPIO_AF_12		((uint8_t)0x0C)
#define GPIO_AF_13		((uint8_t)0x0D)
#define GPIO_AF_14		((uint8_t)0x0E)
#define GPIO_AF_15		((uint8_t)0x0F)


#define GPIO_PIN_0 	((uint16_t)0x0001)
#define GPIO_PIN_1 	((uint16_t)0x0002)
#define GPIO_PIN_2 	((uint16_t)0x0004)
#define GPIO_PIN_3 	((uint16_t)0x0008)
#define GPIO_PIN_4 	((uint16_t)0x0010)
#define GPIO_PIN_5 	((uint16_t)0x0020)
#define GPIO_PIN_6 	((uint16_t)0x0040)
#define GPIO_PIN_7 	((uint16_t)0x0080)
#define GPIO_PIN_8 	((uint16_t)0x0100)
#define GPIO_PIN_9	((uint16_t)0x0200)
#define GPIO_PIN_10 ((uint16_t)0x0400)
#define GPIO_PIN_11 ((uint16_t)0x0800)
#define GPIO_PIN_12	((uint16_t)0x1000)
#define GPIO_PIN_13	((uint16_t)0x2000)
#define GPIO_PIN_14	((uint16_t)0x4000)
#define GPIO_PIN_15	((uint16_t)0x8000)

int GPIO_ReadPin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN);
void GPIO_SetPin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN);
void GPIO_ResetPin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN);
void GPIO_TogglePin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN);

void GPIO_Init(GPIO_Typdef_t* GPIOx, GPIO_PinConfig_t* gpio);
void GPIO_DeInit(GPIO_Typdef_t* GPIOx);
void GPIO_CLK_Ctrl(GPIO_Typdef_t* GPIOx, uint8_t control);

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDis);
void GPIO_IRQ_Set_Priority(uint8_t IRQ, uint8_t priority);
void GPIO_IRQHandling(uint8_t pin_number);
#endif /* INC_GPIO_H_ */

