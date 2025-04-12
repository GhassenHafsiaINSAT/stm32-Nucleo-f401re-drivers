/*
 * spi.c
 *
 *  Created on: Mar 26, 2025
 *      Author: ghass
 */
#include "spi.h"

static void SPI_TXE_handle(SPI_Handle_t* SPI_handler);
static void SPI_RXNE_handle(SPI_Handle_t* SPI_handler);
static void SPI_OVR_handle(SPI_Handle_t* SPI_handler);

void SPI_PeriClockControl(SPI_Typedef_t* SPIx, uint8_t EnOrDis){
	if (EnOrDis == ENABLE){
		if (SPIx == SPI1){
			SPI1_CLK_EN();
		}
		else if (SPIx == SPI2){
			SPI2_CLK_EN();
		}
		else if (SPIx == SPI3){
			SPI3_CLK_EN();
		}
		else if (SPIx == SPI4){
			SPI4_CLK_EN();
		}
	}
	else if (EnOrDis == DISABLE){
		if (SPIx == SPI1){
			SPI1_CLK_DIS();
		}
		else if (SPIx == SPI2){
			SPI2_CLK_DIS();
		}
		else if (SPIx == SPI3){
			SPI3_CLK_DIS();
		}
		else if (SPIx == SPI4){
			SPI4_CLK_DIS();
		}
	}

}
void SPI_Init(SPI_Handle_t* Spi_handler){
	// enable SPI clock
	SPI_PeriClockControl(Spi_handler->SPIx, ENABLE);

	uint32_t tempReg = 0;

	// CR1 register's configuration
	// SPI device mode
	tempReg |= (Spi_handler->spi_config.SPI_DeviceMode << SPI_CR1_MSTR);

	// Sending Mode
	if (Spi_handler->spi_config.SPI_BusConfig == SPI_BUS_CONFIG_FULL_DUPLEX){
		tempReg &= ~(1 << SPI_CR1_BIDIMODE);
	}
	else if (Spi_handler->spi_config.SPI_BusConfig == SPI_BUS_CONFIG_HALF_DUPLEX){
		tempReg |= (1 << SPI_CR1_BIDIMODE);
	}
	else if (Spi_handler->spi_config.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		tempReg &= ~(1 << SPI_CR1_BIDIMODE);
		tempReg |= (1 << SPI_CR1_RXONLY);
	}

	// Clock Speed
	tempReg |= (Spi_handler->spi_config.SPI_CLK_SPEED << SPI_CR1_BR);

	// Data Format
	tempReg |= (Spi_handler->spi_config.SPI_DFF << SPI_CR1_DFF);

	// CPOL
	tempReg |= (Spi_handler->spi_config.SPI_CPOL << SPI_CR1_CPOL);

	// CPHA
	tempReg |= (Spi_handler->spi_config.SPI_CPHA << SPI_CR1_CPHA);

	Spi_handler->SPIx->SPI_CR1 = tempReg;
}
void SPI_DeInit(SPI_Typedef_t SPIx){
	SPI_PeriClockControl(&SPIx, DISABLE);
}

void SPI_Send(SPI_Typedef_t* SPIx, uint8_t* pTxBuffer, uint32_t len){

	while (len > 0){
		while(!(SPIx->SPI_SR & (1 << SPI_SR_TXE)));
		if (SPIx->SPI_SR & (1 << SPI_CR1_DFF)){
			SPIx->SPI_DR = *((uint16_t*) pTxBuffer);
			pTxBuffer++;
			pTxBuffer++;
			len--;
			len--;
		}
		else {
			SPIx->SPI_DR = *pTxBuffer;
			pTxBuffer++;
			len--;
		}
	}
}

void SPI_Receive(SPI_Typedef_t* SPIx, uint8_t* pRxBuffer, uint32_t len){

	while( len > 0){
		while (!(SPIx->SPI_SR & (1 << SPI_SR_RXNE)));
		if (SPIx->SPI_CR1 & (1 << SPI_CR1_DFF)){
			*((uint16_t*) pRxBuffer) = SPIx->SPI_DR;
			pRxBuffer++;
			pRxBuffer++;
			len--;
			len--;
		}
		else {
			*pRxBuffer = SPIx->SPI_DR;
			pRxBuffer++;
			len--;
		}
	}
}

void SPI_IRQInterruptConfig(uint8_t IRQ_Number, uint8_t EnOrDis){
	if (EnOrDis == ENABLE){
		if (IRQ_Number < 32){
			*NVIC_ISER0 |= ( 1 << IRQ_Number);
		}
		else if (IRQ_Number > 31 && IRQ_Number < 64){
			*NVIC_ISER1 |= ( 1 << IRQ_Number % 32);
		}
		else if (IRQ_Number > 63 && IRQ_Number < 96){
			*NVIC_ISER2 |= ( 1 << IRQ_Number % 64);
		}
	}
	else if (EnOrDis == DISABLE){
		if (IRQ_Number < 32){
			*NVIC_ICER0 |= ( 1 << IRQ_Number);
		}
		else if (IRQ_Number > 31 && IRQ_Number < 64){
			*NVIC_ICER1 |= ( 1 << IRQ_Number % 32);
		}
		else if (IRQ_Number > 63 && IRQ_Number < 96){
			*NVIC_ICER2 |= ( 1 << IRQ_Number % 64);
		}
	}
}
void SPI_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority){
	uint8_t iprx, iprx_section;
	iprx = IRQ_Number / 4;
	iprx_section = IRQ_Number % 4;
	*(NVIC_IPR + (iprx*4)) |= (IRQPriority << ((iprx_section * 8) + 4));
}
void SPI_IRQHandling(SPI_Handle_t* Spi_handler){

	uint8_t temp1;
	uint8_t temp2;

	temp1 = Spi_handler->SPIx->SPI_SR & (1 << SPI_SR_TXE);
	temp2 = Spi_handler->SPIx->SPI_CR2 & (1 << SPI_CR2_TXEIE);
	if(temp1 && temp2){
		SPI_TXE_handle(Spi_handler);
	}

	temp1 = Spi_handler->SPIx->SPI_SR & (1 << SPI_SR_RXNE);
	temp2 = Spi_handler->SPIx->SPI_CR2 & (1 << SPI_CR2_RXNEIE);
	if(temp1 && temp2){
		SPI_RXNE_handle(Spi_handler);
	}

	temp1 = Spi_handler->SPIx->SPI_SR & (1 << SPI_SR_OVR);
	temp2 = Spi_handler->SPIx->SPI_CR2 & (1 << SPI_CR2_ERRIE);
	if(temp1 && temp2){
		SPI_OVR_handle(Spi_handler);
	}


}

uint8_t SPI_Send_IT(SPI_Handle_t* SPI_handler, uint8_t* pTxBuffer, uint32_t len){

	if(SPI_handler->TX_state != SPI_BUSY_TX){
		// save buffer and len in global variables
		SPI_handler->TX_len = len;
		SPI_handler->pTX_buffer = pTxBuffer;
		// mark spi as busy
		SPI_handler->TX_state = SPI_BUSY_TX;
		// enable TXEIE bit
		SPI_handler->SPIx->SPI_CR2 |= ( 1 << SPI_CR2_TXEIE );
	}

	return SPI_handler->TX_state;
}
uint8_t SPI_Receive_IT(SPI_Handle_t* SPI_handler, uint8_t* pRxBuffer, uint32_t len){

	if (SPI_handler->RX_state != SPI_BUSY_RX){
		SPI_handler->RX_len = len;
		SPI_handler->pRX_buffer = pRxBuffer;

		SPI_handler->RX_state = SPI_BUSY_RX;

		SPI_handler->SPIx->SPI_CR2 |= (1 << SPI_CR2_RXNEIE);

	}
	return SPI_handler->RX_state;
}

static void SPI_TXE_handle(SPI_Handle_t* SPI_handler){
	if (SPI_handler->SPIx->SPI_SR & (1 << SPI_CR1_DFF)){
		SPI_handler->SPIx->SPI_DR = *((uint16_t*)SPI_handler->pTX_buffer);
		SPI_handler->pTX_buffer++;
		SPI_handler->pTX_buffer++;
		SPI_handler->TX_len--;
		SPI_handler->TX_len--;
	}
	else {
		SPI_handler->SPIx->SPI_DR = *(SPI_handler->pTX_buffer);
		SPI_handler->pTX_buffer++;
		SPI_handler->TX_len--;
	}
	if (!SPI_handler->TX_len){
		// disable txeie
		SPI_handler->SPIx->SPI_CR2 &= ~(1 << SPI_CR2_TXEIE);
		// reset buffers
		SPI_handler->pTX_buffer = NULL;
		SPI_handler->TX_len = 0;
		SPI_handler->TX_state = SPI_READY;
	}
}
static void SPI_RXNE_handle(SPI_Handle_t* SPI_handler){
	if (SPI_handler->SPIx->SPI_SR & (1 << SPI_CR1_DFF)){
		*((uint16_t*)SPI_handler->pRX_buffer) = SPI_handler->SPIx->SPI_DR;
		SPI_handler->pRX_buffer++;
		SPI_handler->pRX_buffer++;
		SPI_handler->RX_len--;
		SPI_handler->RX_len--;
	}
	else {
		*(SPI_handler->pRX_buffer) = SPI_handler->SPIx->SPI_DR;
		SPI_handler->pRX_buffer++;
		SPI_handler->RX_len--;
	}
	if(!SPI_handler->RX_len){
		SPI_handler->SPIx->SPI_CR2 &= ~(1 << SPI_CR2_RXNEIE);
		SPI_handler->pRX_buffer = NULL;
		SPI_handler->RX_len = 0;
		SPI_handler->RX_state = SPI_READY;
	}
}
static void SPI_OVR_handle(SPI_Handle_t* SPI_handler){

}
