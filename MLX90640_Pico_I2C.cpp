/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "MLX90640_I2C_Driver.h"


void MLX90640_I2CInit()
{   
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

int MLX90640_I2CGeneralReset(void)
{
#if 0
    int ack;
    char cmd[2] = {0,0};
    
    cmd[0] = 0x00;
    cmd[1] = 0x06;    

    i2c.stop();
    wait_us(5);    
    ack = i2c_write_blocking(i2c_default, cmd[0], &cmd[1], 1, 0);
    
    if (ack != 0x00)
    {
        return -1;
    }         
    i2c.stop();   
    
    wait_us(50);    
#endif
    return 0;
}

int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
    uint8_t cmd[2] = {0,0};
    uint8_t* pByteData = (uint8_t*)data;
    
    cmd[0] = startAddress >> 8;
    cmd[1] = startAddress & 0x00FF;
    
    int write_result = i2c_write_blocking(i2c_default, slaveAddr, cmd, 2, true);
    
    if (write_result == PICO_ERROR_GENERIC)
    {
      //printf("I2C Read: Address write failure\n");
        return -1;
    }
             
    int read_result = i2c_read_blocking(i2c_default, slaveAddr, pByteData, 2*nMemAddressRead, false);
    
    if (read_result != 2*nMemAddressRead)
    {
      //printf("I2C Read: Read failure, expected %d bytes got %d\n", 2*nMemAddressRead, read_result);
        return -1; 
    }          
    //printf("I2C Read result %hx\n", data[0]);

    for (int i = 0; i < nMemAddressRead; ++i)
    {
      uint8_t tmp = pByteData[i*2];
      pByteData[i*2] = pByteData[i*2+1];
      pByteData[i*2+1] = tmp;
    }
    
    return 0;   
} 

void MLX90640_I2CFreqSet(int freq)
{
    i2c_set_baudrate(i2c_default, 1000*freq);
}

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
    uint8_t cmd[4] = {0,0,0,0};
    uint16_t dataCheck;

    cmd[0] = writeAddress >> 8;
    cmd[1] = writeAddress & 0x00FF;
    cmd[2] = data >> 8;
    cmd[3] = data & 0x00FF;

    int write_result = i2c_write_blocking(i2c_default, slaveAddr, cmd, 4, false);
    
    if (write_result == PICO_ERROR_GENERIC)
    {
      //printf("I2C Write: Write failure\n");
        return -1;
    }         
    
    MLX90640_I2CRead(slaveAddr, writeAddress, 1, &dataCheck);
    
    if ( dataCheck != data)
    {
      //printf("I2C Write: Write check failure\n");
        return -2;
    }
    
    return 0;
}

