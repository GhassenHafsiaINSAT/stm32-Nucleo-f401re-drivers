/*
 * i2c.h
 *
 *  Created on: Apr 12, 2025
 *      Author: ghass
 */

#ifndef I2C_H_
#define I2C_H_

#include "NUCLEOf401_mem_map.h"

typedef struct {

} I2C_config_t;

typedef struct {
	I2C_Typedef_t I2Cx;
	I2C_config_t* i2c_config;
} I2C_handle_t;
#endif /* I2C_H_ */
