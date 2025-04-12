/*
 * gpio.c
 *
 *  Created on: Feb 15, 2025
 *      Author: ghassen
 */
#include "gpio.h"
#include "rcc.h"

int GPIO_ReadPin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN){
	if ((GPIOx->GPIO_IDR & GPIO_PIN) == 0)
		return 0;
	else
		return 1;
}

void GPIO_SetPin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN){
	GPIOx->GPIO_BSRR = ((uint32_t) GPIO_PIN);
}

void GPIO_ResetPin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN){
	GPIOx->GPIO_BSRR = ((uint32_t) GPIO_PIN ) << 16;
}

void GPIO_TogglePin(GPIO_Typdef_t* GPIOx, uint16_t GPIO_PIN){
	if ((GPIOx->GPIO_IDR & GPIO_PIN) == 0)
		GPIOx->GPIO_BSRR = GPIO_PIN;
	else
		GPIOx->GPIO_BSRR = GPIO_PIN << 16;
}

void GPIO_CLK_Ctrl(GPIO_Typdef_t* GPIOx, uint8_t control){
	if (control == ENABLE){
		if (GPIOx == GPIOA){
			GPIOA_CLK_EN();
		}
		else if (GPIOx == GPIOB){
			GPIOB_CLK_EN();
		}
		else if (GPIOx == GPIOC){
			GPIOC_CLK_EN();
		}
		else if (GPIOx == GPIOD){
			GPIOD_CLK_EN();
		}
	}
	else if (control == DISABLE){
		if (GPIOx == GPIOA){
			GPIOA_CLK_DIS();
		}
		else if (GPIOx == GPIOB){
			GPIOB_CLK_DIS();
		}
		else if (GPIOx == GPIOC){
			GPIOC_CLK_DIS();
		}
		else if (GPIOx == GPIOD){
			GPIOD_CLK_DIS();
		}
	}
}
void GPIO_Init(GPIO_Typdef_t* GPIOx, GPIO_PinConfig_t* gpio){

	uint32_t pin = 0x00u;
	uint32_t target_pin = 0x00u;
	for (uint32_t position = 0; position < 16; position++){
		pin = 0x01u << position;
		target_pin = (gpio->GPIO_PIN) & pin;
		if (target_pin == pin){
			if (((gpio->GPIO_Mode != GPIO_MODE_IT_RAISING) |
					(gpio->GPIO_Mode != GPIO_MODE_IT_FALLING)) |
					((gpio->GPIO_Mode != GPIO_MODE_IT_RAISING_FALLING))){
				GPIOx->GPIO_MODER &= ~(0x3u << 2*position);
				GPIOx->GPIO_MODER |= (gpio->GPIO_Mode) << 2*position;

				GPIOx->GPIO_OTYPER &= ~(0x3u << 2*position);
				GPIOx->GPIO_OTYPER |= (gpio->GPIO_Otyper) << 2*position;

				GPIOx->GPIO_OSPEEDR &= ~(0x3u << 2*position);
				GPIOx->GPIO_OSPEEDR |= (gpio->GPIO_Ospeeder) << 2*position;

				GPIOx->GPIO_PUPDR &= ~(0x3u << 2*position);
				GPIOx->GPIO_PUPDR |= (gpio->GPIO_PullPush) << 2*position;

				if (gpio->GPIO_Mode == GPIO_MODE_ALTERNATE){
					uint32_t AF_reg = gpio->GPIO_PIN / 8;
					uint32_t AFx = gpio->GPIO_PIN % 8;
					GPIOx->GPIO_AF[AF_reg] |= (gpio->GPIO_AF << AFx * 4);
				}
			}
			else {
				if (gpio->GPIO_Mode == GPIO_MODE_IT_RAISING){
					EXTI->EXTI_FTSR |= ( 1 << gpio->GPIO_PIN );
					EXTI->EXTI_RTSR &= ~( 1 << gpio->GPIO_PIN );
				}
				else if (gpio->GPIO_Mode == GPIO_MODE_IT_FALLING){
					EXTI->EXTI_RTSR |= ( 1 << gpio->GPIO_PIN );
					EXTI->EXTI_FTSR &= ~( 1 << gpio->GPIO_PIN );
				}
				else if (gpio->GPIO_Mode != GPIO_MODE_IT_RAISING_FALLING){
					EXTI->EXTI_FTSR |= ( 1 << gpio->GPIO_PIN );
					EXTI->EXTI_RTSR |= ( 1 << gpio->GPIO_PIN );
				}
				// configure the GPIO port selection using SYSCFG_EXTICR
				uint8_t portcode = GPIO_PORT_CODE(GPIOx);
				uint8_t temp1 = position / 4;
				uint8_t temp2 = position % 4;
				SYSCFG->SYSCFG_EXTICR[temp1] = (portcode << temp2 * 4);
				// enable the interrupt delivery from peripheral to processor on peripheral side
				EXTI->EXTI_IMR |= ( 1 << gpio->GPIO_PIN );

			}

		}
	}
}

void GPIO_DeInit(GPIO_Typdef_t* GPIOx){
	GPIO_CLK_Ctrl(GPIOx, DISABLE);
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDis){
	if (EnOrDis == ENABLE){
		if (IRQNumber <= 31){
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if (IRQNumber > 31 && IRQNumber < 64){
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}
		else if (IRQNumber >= 64 && IRQNumber < 96){
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}
	}
	else {
		if (IRQNumber <= 31){
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if (IRQNumber > 31 && IRQNumber < 64){
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}
		else if (IRQNumber >= 64 && IRQNumber < 96){
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}
	}
}

void GPIO_IRQ_Set_Priority(uint8_t IRQ_number, uint8_t IRQ_priority){
	uint8_t iprx = IRQ_number / 4;
	uint8_t iprx_section = IRQ_number % 4;
	*(NVIC_IPR + (iprx*4)) |= (IRQ_priority << ((iprx_section * 8) + 4));
}

void GPIO_IRQHandling(uint8_t pin_number){
	if(EXTI->EXTI_PR & (1 << pin_number)){
		EXTI->EXTI_PR |= (1 << pin_number);
	}
}








