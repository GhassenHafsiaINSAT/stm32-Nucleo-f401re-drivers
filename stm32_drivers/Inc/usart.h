/*
 * uart.h
 *
 *  Created on: Feb 25, 2025
 *      Author: ghassen
 */

#ifndef USART_H_
#define USART_H_

#include "NUCLEOf401_mem_map.h"

// Word length (M)
#define UART_WORDLENGTH_8B 		0
#define UART_WORDLENGTH_9B 		1

// Baudrate
#define USART_STD_BAUD_1200		1200
#define USART_STD_BAUD_2400		400
#define USART_STD_BAUD_9600		9600
#define USART_STD_BAUD_19200 	19200
#define USART_STD_BAUD_38400 	38400
#define USART_STD_BAUD_57600 	57600
#define USART_STD_BAUD_115200 	115200
#define USART_STD_BAUD_230400 	230400
#define USART_STD_BAUD_460800 	460800
#define USART_STD_BAUD_921600 	921600
#define USART_STD_BAUD_2M 		2000000
#define SUART_STD_BAUD_3M 		3000000

// Stop bits
#define UART_STOPBITS_1			0
#define UART_STOPBITS_0_5 		1
#define UART_STOPBITS_2			2
#define UART_STOPBITS_1_5		3

// Parity bit
#define UART_PARITY_EVEN		0
#define UART_PARITY_ODD			1
#define UART_PARITY_DISABLE		2

// MODE
#define UART_MODE_RX			0
#define UART_MODE_TX			1
#define UART_MODE_TX_RX			2

// HwFlowCtl
#define UART_HWCONTROL_NONE		0
#define UART_HWCONTROL_RTS		1
#define UART_HWCONTROL_CTS		2
#define UART_HWCONTROL_CTS_RTS	3


#define USART_SR_TXE 	0
#define USART_SR_TC 	1
#define USART_SR_RXNE 	2

typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baudrate;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HwFloatCtl;
} USART_config_t;

typedef struct
{
	USART_TypeDef_t* 		pUSARTx;
	USART_config_t 		usart_config;
	uint8_t             *pTxBuffer;
	uint8_t             *pRxBuffer;
	uint32_t            TxLength;
	uint32_t            RxLength;
	uint32_t            TxCount;
	uint32_t            RxCount;
	uint8_t             TxBusyState;
	uint8_t             RxBusyState;
	uint32_t            Timeout;
} USART_Handle_t;

typedef enum {
	HAL_OK  	= 0x0000U,
	HAL_ERROR 	= 0x0001U,
	HAL_BUSY	= 0x0002U,
	HAL_TIMEOUT = 0x0003U
} HAL_StatusTypeDef;


void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_PeriClockControl(USART_TypeDef_t* USARTx, uint8_t EnOrDis);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len);
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_GetFlagStatus(USART_TypeDef_t *pUSARTx , uint32_t FlagName);
void USART_IRQHandling(USART_Handle_t *pHandle);

void USART_SetBaudRate(USART_TypeDef_t *pUSARTx, uint32_t BaudRate);

/*
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);


void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);


void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);


void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv);
*/
#endif /* USART_H_ */
