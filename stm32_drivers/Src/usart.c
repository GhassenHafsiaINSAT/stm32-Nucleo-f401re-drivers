/*
 * uart.c
 *
 *  Created on: Feb 25, 2025
 *      Author: ghass
 */
#include "usart.h"
#include "rcc.h"


void USART_PeriClockControl(USART_TypeDef_t* USARTx, uint8_t EnOrDis){
	if (EnOrDis == ENABLE){
		if (USARTx == USART1){
			USART1_CLK_EN();
		}
		else if (USARTx == USART2){
			USART2_CLK_EN();
		}
		else if (USARTx == USART6){
			USART6_CLK_EN();
		}
	}
	else if (EnOrDis == DISABLE){
		if (USARTx == USART1){
			USART1_CLK_DIS();
		}
		else if (USARTx == USART2){
			USART2_CLK_DIS();
		}
		else if (USARTx == USART6){
			USART6_CLK_DIS();
		}
	}
}

void USART_Init(USART_Handle_t *pUSARTHandle)
{
	// enabling clocks
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	uint32_t tempreg=0;

	// CR1 register's configurations

	// USART Enable Bit
	tempreg |= (1 << 13);

	// TX,RX or RX_TX
	if (pUSARTHandle->usart_config.USART_Mode == UART_MODE_RX){
		tempreg |= (1 << 2);
	}
	else if (pUSARTHandle->usart_config.USART_Mode == UART_MODE_TX){
		tempreg |= ( 1 << 3);

	}
	else if (pUSARTHandle->usart_config.USART_Mode == UART_MODE_TX_RX){
		tempreg |= ( ( 1 << 2) | ( 1 << 3) );
	}

	// Word length
	tempreg |= pUSARTHandle->usart_config.USART_WordLength << 12 ;


	// parity control
	if ( pUSARTHandle->usart_config.USART_ParityControl == UART_PARITY_EVEN){
		tempreg |= ( 1 << 10);

	}
	else if (pUSARTHandle->usart_config.USART_ParityControl == UART_PARITY_ODD){
		tempreg |= ( 1 << 10);

		tempreg |= ( 1 << 11);
	}

	//Program the CR1 register
	pUSARTHandle->pUSARTx->USART_CR1 = tempreg;

	// cr2 register's configuration
	tempreg=0;

	// Number of stop bits
	if (pUSARTHandle->usart_config.USART_NoOfStopBits == UART_STOPBITS_1){
		tempreg |= (0 << 12) | (0 << 13);
	}
	else if (pUSARTHandle->usart_config.USART_NoOfStopBits == UART_STOPBITS_0_5){
		tempreg |= (1 << 12)| (0 << 13);
	}
	else if (pUSARTHandle->usart_config.USART_NoOfStopBits == UART_STOPBITS_2){
		tempreg |= (0 << 12)| (1 << 13);
	}
	else if (pUSARTHandle->usart_config.USART_NoOfStopBits == UART_STOPBITS_1_5){
		tempreg |= (1 << 12)| (1 << 13);
	}

	//Program the CR2 register
	pUSARTHandle->pUSARTx->USART_CR2 = tempreg;

	// CR3 register's configuration
	tempreg=0;

	// hardware flow control
	if ( pUSARTHandle->usart_config.USART_HwFloatCtl == UART_HWCONTROL_CTS){
		tempreg |= (1 << 9);


	}
	else if (pUSARTHandle->usart_config.USART_HwFloatCtl == UART_HWCONTROL_RTS){
		tempreg |= (1 << 8);

	}
	else if (pUSARTHandle->usart_config.USART_HwFloatCtl == UART_HWCONTROL_CTS_RTS){
		tempreg |= (1 << 8) | (1 << 9);
	}


	pUSARTHandle->pUSARTx->USART_CR3 = tempreg;

	// Baudrate register's configuration
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->usart_config.USART_Baudrate);

}

uint8_t USART_GetFlagStatus(USART_TypeDef_t *pUSARTx , uint32_t FlagName){

	if (FlagName == USART_SR_TXE){
		return ((uint8_t)(pUSARTx->USART_SR & (1 << 7)));
	}
	else if (FlagName == USART_SR_TC){
		return ((uint8_t)(pUSARTx->USART_SR & (1 << 6)));
	}
	else if (FlagName == USART_SR_RXNE){
		return ((uint8_t)(pUSARTx->USART_SR & (1 <<5)));
	}
	return 0;
}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len){

	uint16_t *pdata;
	for(uint32_t i = 0 ; i < len; i++)
	{
		// wait for TXE to be set
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_TXE));

		// 9bit data transfer
		if(pUSARTHandle->usart_config.USART_WordLength == UART_WORDLENGTH_9B)
		{

			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->USART_DR = (*pdata & (uint16_t)0x01FF);

			//check for USART_ParityControl
			if(pUSARTHandle->usart_config.USART_ParityControl == UART_PARITY_DISABLE)
			{
				// No parity is used in this transfer.
				// each data will be written on 2 bits
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				// Parity bit is used in this transfer the 9th bit will be replaced by parity bit.
				// each data will be written on 1 bit
				pTxBuffer++;
			}
		}
		else
		{
			if (pUSARTHandle->usart_config.USART_ParityControl == UART_PARITY_DISABLE){
				// 8bit data transfer
				pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer  & (uint8_t)0xFF);
				pTxBuffer++;
			}
			else{
				// 7bit data transfer
				pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer & (uint8_t)0x7F);
				pTxBuffer++;
			}
		}
	}

	// wait for TC to be set
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_TC));
}

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len){

	//Loop over until "len" number of bytes are transferred
	for(uint32_t i = 0 ; i < len; i++)
	{
		//Implement the code to wait until RXNE flag is set in the SR
		while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_RXNE));

		//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if( pUSARTHandle->usart_config.USART_WordLength == UART_WORDLENGTH_9B)
		{
			//We are going to receive 9bit data in a frame

			//check are we using USART_ParityControl control or not
			if(pUSARTHandle->usart_config.USART_ParityControl == UART_PARITY_DISABLE)
			{
				// No parity is used. so, 9bits will be received
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->USART_DR  & (uint16_t)0x01FF);

				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				// Parity is used, so, 8bits will be received
				*pRxBuffer = (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0xFF);
				pRxBuffer++;
			}
		}
		else
		{
			// receiving 8bit data in a frame

			//check are we using USART_ParityControl control or not
			if(pUSARTHandle->usart_config.USART_ParityControl == UART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				*pRxBuffer = (uint8_t)pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF;
			}

			else
			{
				// Parity is used, 7 bits will be of user data and 1 bit is parity
				*pRxBuffer = (uint8_t)pUSARTHandle->pUSARTx->USART_DR & (uint8_t) 0x7F;
			}
			pRxBuffer++;
		}
	}
}

HAL_StatusTypeDef USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t txstate = pUSARTHandle->TxBusyState;

	if(txstate != HAL_BUSY)
	{
		pUSARTHandle->TxLength = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = HAL_BUSY;

		// enable interrupt for TXE
		pUSARTHandle->pUSARTx->USART_CR1 |= (1 << 7);

		// enable interrupt for TC
		pUSARTHandle->pUSARTx->USART_CR1 |= (1 << 6);
	}
	return txstate;
}

uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	if(rxstate != HAL_BUSY)
	{
		pUSARTHandle->RxLength = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = HAL_BUSY;

		//Implement the code to enable interrupt for RXNE
		pUSARTHandle->pUSARTx->USART_CR1 |= (1 << 5);
	}

	return rxstate;
}


void USART_SetBaudRate(USART_TypeDef_t *pUSARTx, uint32_t BaudRate)
{

	uint32_t PCLKx;
	uint32_t usartdiv;

	//Mantissa and Fraction values
	uint32_t M_part,F_part;

	uint32_t tempreg=0;

	//Get the value of APB bus clock in to the variable PCLKx
	if(pUSARTx == USART1 || pUSARTx == USART6)
	{
		//USART1 and USART6 are hanging on APB2 bus
		PCLKx = RCC_GetPCLK2Value();
	}else
	{
		PCLKx = RCC_GetPCLK1Value();
	}

	//Check for OVER8 configuration bit
	if(pUSARTx->USART_CR1 & (1 << 15)){
		//OVER8 = 1 , over sampling by 8
		usartdiv = ((25 * PCLKx) / (2 *BaudRate));
	}

	else{
		//over sampling by 16
		usartdiv = ((25 * PCLKx) / (4 * BaudRate));
	}

	//Calculate the Mantissa part
	M_part = usartdiv/100;

	tempreg |= M_part << 4;

	F_part = (usartdiv - (M_part * 100));

	if(pUSARTx->USART_CR1 & ( 1 << 15))
	{
		//OVER8 = 1 , over sampling by 8
		F_part = ((( F_part * 100)+ 50) / 100)& ((uint8_t)0x07);

	}else
	{
		//over sampling by 16
		F_part = ((( F_part * 100)+ 50) / 100) & ((uint8_t)0x0F);

	}

	tempreg |= F_part;
	pUSARTx->USART_BRR = tempreg;
}


