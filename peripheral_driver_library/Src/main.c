#include "gpio.h"
#include "NUCLEOf401_mem_map.h"


uint8_t EtatBouton;
uint32_t x;
int i;
GPIO_Handle_t GPIO_InitStruct ;

void EXTI0_IRQHandler(void){
	GPIO_IRQHandling(0);
}

int main ()
{
	//Activate GPIOs Clocks
	GPIO_CLK_Ctrl(GPIOA, ENABLE);
	GPIO_CLK_Ctrl(GPIOC, ENABLE);
	//Configure GPIO Pins (Led: PA5) ! MODE INPUT ; SPEED LOW; OUTOUT PUSH PULL; NO PULL

	// push button configuration
	GPIO_InitStruct.GPIOx = GPIOC;
	GPIO_InitStruct.gpio_config.GPIO_PIN = GPIO_PIN_13  ;
	GPIO_InitStruct.gpio_config.GPIO_Mode = GPIO_MODE_INPUT ;
	GPIO_InitStruct.gpio_config.GPIO_PullPush = GPIO_PullPush_NoPull;

	GPIO_Init (GPIO_InitStruct.GPIOx, &(GPIO_InitStruct.gpio_config));

	// Led configuration
	GPIO_InitStruct.GPIOx = GPIOA;
	GPIO_InitStruct.gpio_config.GPIO_PIN = GPIO_PIN_5;
	GPIO_InitStruct.gpio_config.GPIO_Mode = GPIO_MODE_OUTPUT;
	GPIO_InitStruct.gpio_config.GPIO_PullPush = GPIO_PullPush_NoPull;

	GPIO_Init(GPIO_InitStruct.GPIOx, &(GPIO_InitStruct.gpio_config));


	while (1)
	{
		//Read Push Button State (0=Pressed, 1=Released)
		EtatBouton=GPIO_ReadPin(GPIOC,GPIO_PIN_13);

		if (EtatBouton==0)  //If Pressed, Led ON

			GPIO_SetPin(GPIOA, GPIO_PIN_5);

		else //Released, Led OFF
			GPIO_ResetPin(GPIOA, GPIO_PIN_5);

	}
}
