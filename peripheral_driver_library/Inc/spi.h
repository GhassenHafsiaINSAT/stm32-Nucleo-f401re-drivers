/*
 * spi.h
 *
 *  Created on: Mar 26, 2025
 *      Author: ghass
 */

#ifndef SPI_H_
#define SPI_H_

#include "NUCLEOf401_mem_map.h"
typedef struct {
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_CLK_SPEED;
	uint8_t SPI_DFF;
	uint8_t SPI_CPHA;
	uint8_t SPI_CPOL;
	uint8_t SPI_SSM;
	uint8_t SPI_Speed;
} SPI_config_t;

typedef struct{
	SPI_Typedef_t* SPIx;
	SPI_config_t spi_config;
	uint8_t* pTX_buffer;
	uint8_t* pRX_buffer;
	uint32_t TX_len;
	uint32_t RX_len;
	uint8_t TX_state;
	uint8_t RX_state;
} SPI_Handle_t;

/*
 * @SPI_DeviceMode
 */
#define SPI_DEV_MODE_MASTER	1
#define SPI_DEV_MODE_SLAVE	0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FULL_DUPLEX		0
#define SPI_BUS_CONFIG_HALF_DUPLEX		1
#define SPI_BUS_CONFIG_SIMPLEX_TXONLY	2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY	3

/*
 * @SPI_CLK_SPEED
 */
#define SPI_CLK_SPEED_DIV2		0
#define SPI_CLK_SPEED_DIV4		1
#define SPI_CLK_SPEED_DIV8		2
#define SPI_CLK_SPEED_DIV16		3
#define SPI_CLK_SPEED_DIV32		4
#define SPI_CLK_SPEED_DIV64		5
#define SPI_CLK_SPEED_DIV128	6
#define SPI_CLK_SPEED_DIV256	7

/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS	0
#define SPI_DFF_16BITS  1

/*
 * @CPOL
 */
#define SPI_CPOL_LOW	0
#define SPI_CPOL_HIGH	1


/*
 * @CPHA
 */
#define SPI_CPHA_LOW	0
#define SPI_CPHA_HIGH	1


/*
 * @SSM
 */
#define SPI_SSM_DIS		0
#define SPI_SSM_EN		1

/*
 * SPI states
 */
#define SPI_READY 		0
#define SPI_BUSY_RX 	1
#define SPI_BUSY_TX 	2

void SPI_PeriClockControl(SPI_Typedef_t* SPIx, uint8_t EnOrDis);
void SPI_Init(SPI_Handle_t* Spi_handler);
void SPI_DeInit(SPI_Typedef_t SPIx);

void SPI_Send(SPI_Typedef_t* SPIx, uint8_t* pTxBuffer, uint32_t len);
void SPI_Receive(SPI_Typedef_t* SPIx, uint8_t* pRxBuffer, uint32_t len);

uint8_t SPI_Send_IT(SPI_Handle_t* SPI_handler, uint8_t* pTxBuffer, uint32_t len);
uint8_t SPI_Receive_IT(SPI_Handle_t* SPI_handler, uint8_t* pRxBuffer, uint32_t len);

void SPI_IRQInterruptConfig(uint8_t IRQ_Number, uint8_t EnOrDis);
void SPI_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t* Spi_handler);
#endif /* SPI_H_ */
