/*
File: dht11.c
Description: DHT11 library
Author: Adam Orcholski, tath@o2.pl, www.tath.eu
Log (day/month/year):
- (07.04.2013) Initial
- (19.07.2015) Refactored
Note:
 - refer to official documentation to understand what is happening here
*/
#include "dht11.h"

GPIO_InitTypeDef dht11_in_struct;
GPIO_InitTypeDef dht11_out_struct;

/* Public function definitions */
void DHT11_Init(void)
{
    dht11_in_struct.Pin = DHT_11_PIN;
    dht11_in_struct.Mode =  GPIO_MODE_IT_RISING;
    dht11_in_struct.Pull = GPIO_PULLUP;
    dht11_in_struct.Speed = GPIO_SPEED_HIGH;
    
    dht11_out_struct.Pin = DHT_11_PIN;
    dht11_out_struct.Mode = GPIO_MODE_OUTPUT_PP;
    dht11_out_struct.Pull = GPIO_PULLUP;
    dht11_out_struct.Speed = GPIO_SPEED_HIGH;
    GPIO_OUTPUT_CLEAR;
    
    CRITICAL_SECTION_INIT;
    DELAY_INIT;    
}

void DHT11_Denit(void)
{
    DELAY_DEINIT;
    CRITICAL_SECTION_DEINIT;
}

DHT11_ERROR_CODE_t DHT11_Read(uint8_t * const pData)
{
    int i = 0;
    int j = 0;
    DHT11_ERROR_CODE_t errorCode = DHT11_OK;
    
    #ifdef ENABLE_TIMEOUTS
    int timeout = TIMEOUT_VALUE;
    #endif

    HAL_GPIO_Init( DHT_11_PORT, &dht11_out_struct);  //GPIO_SET_AS_OUTPUT;
    
    CRITICAL_SECTION_ENTER;
    
    DELAY_ENABLE;
    
    /* start sequence */
    GPIO_OUTPUT_CLEAR;
    
    DELAY_US(18000);
    
    GPIO_OUTPUT_SET;
    DELAY_US(40);

    HAL_GPIO_Init( DHT_11_PORT, &dht11_in_struct);  //GPIO_SET_AS_INPUT;

    while(0 == GPIO_INPUT_GET) /* 80us on '0' */
    {
        #ifdef ENABLE_TIMEOUTS
        if (--(timeout) <= 0)
        {
            errorCode = DHT11_TIMEOUT;
            break;
        }
        #endif
    };
    
    #ifdef ENABLE_TIMEOUTS
    timeout = TIMEOUT_VALUE;
    #endif
    if (DHT11_OK == errorCode)
    {
        while(1 == GPIO_INPUT_GET) /* 80us on '1' */
        {
            #ifdef ENABLE_TIMEOUTS
            if (--(timeout) <= 0)
            {
                errorCode = DHT11_TIMEOUT;
                break;
            }
            #endif
        };
    }        
    /* start sequence - end */

    /* read sequence */
    if (DHT11_OK == errorCode)
    {
        for(j=0;j<5;j++)
        {
            for(i=0;i<8;i++)
            {
                #ifdef ENABLE_TIMEOUTS
                timeout = TIMEOUT_VALUE;
                #endif
                while(0 == GPIO_INPUT_GET)
                {
                    #ifdef ENABLE_TIMEOUTS
                    if (--(timeout) <= 0)
                    {
                        errorCode = DHT11_TIMEOUT;
                        break;
                    }
                    #endif
                }; /* 50 us on 0 */

                if (1 == GPIO_INPUT_GET)
                {
                    DELAY_US(30);
                }

                pData[j] <<= 1;
                
                if(1 == GPIO_INPUT_GET)
                {
                    DELAY_US(40); /* wait 'till 70us */
                    pData[j] |= 1;
                }
                else
                {
                    pData[j] &= 0xfe;
                }
            }
        }
    }
    /* read sequence - end */
    
    DELAY_DISABLE
    CRITICAL_SECTION_LEAVE;

    /* checksum check */
    if (DHT11_OK == errorCode)
    {
        if ((pData[0] + pData[2]) != pData[4])
        {
            errorCode = DHT11_WRONG_CHCKSUM;
        }
        else
        {
            errorCode = DHT11_OK;
        }
    }

    return errorCode;
}
