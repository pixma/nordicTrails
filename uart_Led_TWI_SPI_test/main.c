/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup uart_example_main main.c
 * @{
 * @ingroup uart_example
 * @brief UART Example Application main file.
 *
 * This file contains the source code for a sample application using UART.
 *
 * @image html example_board_setup_a.jpg "Use board setup A for this example."
 */

#define SPI_MASTER_0_ENABLE 1
 
#include "simple_uart.h"
#include <stdbool.h>
#include "app_error.h"
#include <stdint.h>
#include "app_util_platform.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "twi_master.h"
#include "adxl_345b.h"
#include "app_error.h"
#include "spi_master.h"



#if defined(SPI_MASTER_0_ENABLE)
    #define SPI_MASTER_HW SPI_MASTER_0
#elif defined(SPI_MASTER_1_ENABLE)
    #define SPI_MASTER_HW SPI_MASTER_1
#endif


#define LED_GREEN 2
//#define LED_YELLOW 5

#define WITH_ADXL YES
//#define WITH_ADXL_I2C YES
#define WITH_ADXL_SPI YES

//#define ENABLE_LOOPBACK_TEST  /**< if defined, then this example will be a loopback test, which means that TX should be connected to RX to get data loopback. */

#define ERROR_PIN           (LED_0) /**< gpio pin number to show error if loopback is enabled. */
//#define MAX_TEST_DATA_BYTES (15U)   /**< max number of test bytes to be used for tx and rx. */

static volatile bool m_transfer_completed = true; /**< A flag to inform about completed transfer. */

#define PRODUCT_ID_BYTES 1U //!< Number of bytes to expect to be in product ID

//#ifndef ENABLE_LOOPBACK_TEST

/** @brief Function for sending 'Exit!' string to UART. 
 * 
 *  @note Execution is blocked until UART peripheral detects all characters have been sent.
 */
static __INLINE void uart_quit()
{
    simple_uart_putstring((const uint8_t *)" \n\rExit!\n\r");
}


/** @brief Function for sending 'Start:' string to UART. 
 *  @details Execution is blocked until UART peripheral detects all characters have been sent.
 */
static __INLINE void uart_start()
{
    simple_uart_putstring((const uint8_t *)" \n\rDemoApp Starting...\t[OK]\r\n ");
}
//#else

/** @brief Function for setting the @ref ERROR_PIN high, and then enter an infinite loop.
 *  This function is called if any of the nRF6350 functions fail.
 */
//static void show_error(void)
//{
//    nrf_gpio_pin_write(ERROR_PIN, 1);
//    while(true)
//    {
//        // Do nothing.
//    }
//}


/** @brief Function for testing UART loop back. 
 *  @details Transmitts one character at a time to check if the data received from the loopback is same as the transmitted data.
 *  @note  @ref TX_PIN_NUMBER must be connected to @ref RX_PIN_NUMBER)
// */
//static void uart_loopback_test()
//{
//    uint8_t *tx_data = (uint8_t *)("\n\rLOOPBACK_TEST");
//    uint8_t  rx_data;

//    // Start sending one byte and see if you get the same
//    for (uint32_t i = 0; i < MAX_TEST_DATA_BYTES; i++)
//    {
//        bool status;
//        simple_uart_put(tx_data[i]);
//        status = simple_uart_get_with_timeout(2, &rx_data);

//        if ((rx_data != tx_data[i]) || (!status))
//        {
//            show_error();
//        }
//    }
//    return; 
//}
//#endif

/*
 * This example uses only one instance of the SPI master.
 * Please make sure that only one instance of the SPI master is enabled in config file.
 */
 
#define TX_RX_BUF_LENGTH    16u     /**< SPI transaction buffer length. */

//Data buffers.
static uint8_t m_tx_data[TX_RX_BUF_LENGTH] = {0}; /**< A buffer with data to transfer. */
static uint8_t m_rx_data[TX_RX_BUF_LENGTH] = {0}; /**< A buffer for incoming data. */


/**@brief Function for error handling, which is called when an error has occurred. 
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    //Set LED2 high to indicate that error has occurred.
    //nrf_gpio_pin_set(LED_2);
	
		simple_uart_putstring((const uint8_t *)"\n\rApp Error on assertion...\t[FAILURE]\r\n");
    
    for (;;)
    {
        //No implementation needed.
    }
}

/**@brief Function for checking if data coming from a SPI slave are valid.
 *
 * @param[in] p_buf     A pointer to a data buffer.
 * @param[in] len       A length of the data buffer.
 * 
 * @note Expected ASCII characters from: 'a' to: ('a' + len - 1).
 *
 * @retval true     Data are valid.
 * @retval false    Data are invalid.
 */
static __INLINE bool buf_check(uint8_t * p_buf, uint16_t len)
{
//    uint16_t i;
//    for (i = 0; i < len; i++)
//    {
//        if (p_buf[i] != (uint8_t)('a' + i))
//        {
//            return false;
//        }
//    }
//    
    return true;
}


/** @brief Function for initializing a SPI master driver.
 */
static uint32_t spi_master_init(void)
{
    spi_master_config_t spi_config = SPI_MASTER_INIT_DEFAULT;
    
    #if defined(SPI_MASTER_0_ENABLE)
				simple_uart_putstring((const uint8_t *)"\r\nSPI Master defining at 0 Index...\t[OK]\r\n");						
        spi_config.SPI_Pin_SCK = SPIM0_SCK_PIN;
        spi_config.SPI_Pin_MISO = SPIM0_MISO_PIN;
        spi_config.SPI_Pin_MOSI = SPIM0_MOSI_PIN;
        spi_config.SPI_Pin_SS = SPIM0_SS_PIN;
    #elif defined(SPI_MASTER_1_ENABLE)
        spi_config.SPI_Pin_SCK = SPIM1_SCK_PIN;
        spi_config.SPI_Pin_MISO = SPIM1_MISO_PIN;
        spi_config.SPI_Pin_MOSI = SPIM1_MOSI_PIN;
        spi_config.SPI_Pin_SS = SPIM1_SS_PIN;
    #endif /* SPI_MASTER_ENABLE */
    
    return spi_master_open(SPI_MASTER_HW, &spi_config);
}


/**@brief Function for SPI master event callback.
 *
 * Upon receiving an SPI transaction complete event, checks if received data are valid.
 *
 * @param[in] spi_master_evt    SPI master driver event.
 */
static void spi_master_event_handler(spi_master_evt_t spi_master_evt)
{
    bool result = false;
    switch (spi_master_evt.evt_type)
    {
        case SPI_MASTER_EVT_TRANSFER_COMPLETED:
					
						simple_uart_putstring((const uint8_t *)"\r\nSPI Master transfer Completed...\t[OK]\r\n");						
            
            //Check if data are vaild.
            result = buf_check(m_rx_data, spi_master_evt.data_count);
            APP_ERROR_CHECK_BOOL(result);
        
            //Inform application that transfer is completed.
            m_transfer_completed = true;
				
						simple_uart_putstring((const uint8_t *)"\r\nSPI Master data recv...\t");
						
						for (uint8_t i = 0; i < spi_master_evt.data_count; i++)
						{
								simple_uart_put( m_rx_data[i] );
						}
						
						simple_uart_putstring((const uint8_t *)"\r\n\r\n");
            break;
        
        default:
            //No implementation needed.
            break;
    }
}

/**@brief The function initializes TX buffer to values to be sent and clears RX buffer.
 *
 * @note Function initializes TX buffer to values from 'A' to ('A' + len - 1)
 *       and clears RX buffer (fill by 0).
 *
 * @param[in] p_tx_data     A pointer to a buffer TX.
 * @param[in] p_rx_data     A pointer to a buffer RX.
 * @param[in] len           A length of the data buffers.
 */
static void init_buffers(uint8_t * const p_tx_data,
                         uint8_t * const p_rx_data,
                         const uint16_t len)
{
    		
		p_tx_data[0] = 0x80;			// will get Device ID. [R-MB-AD5--AD0]
		
		uint16_t i;
    for (i = 0; i < len; i++)
    {        
        p_rx_data[i] = 0;
    }
    
}



/**@brief Functions prepares buffers and starts data transfer.
 *
 * @param[in] p_tx_data     A pointer to a buffer TX.
 * @param[in] p_rx_data     A pointer to a buffer RX.
 * @param[in] len           A length of the data buffers.
 */
static void spi_send_recv(uint8_t * const p_tx_data,
                          uint8_t * const p_rx_data,
                          const uint16_t len)
{		
		//Initalize buffers.
    init_buffers(p_tx_data, p_rx_data, len);
    
    //Start transfer.
    uint32_t err_code = spi_master_send_recv(SPI_MASTER_HW, p_tx_data, len, p_rx_data, len);
    APP_ERROR_CHECK(err_code);
}


/**
 * @brief Function for main application entry.
 */
int main(void)
{
		#ifdef WITH_ADXL_I2C
		bool adxl_init_succeeded;
		uint8_t product_id[PRODUCT_ID_BYTES];	
		#endif
	
	
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
    
    //nrf_gpio_cfg_output(ERROR_PIN); // ERROR_PIN configured as output.
		nrf_gpio_cfg_output( LED_GREEN );
		//nrf_gpio_cfg_output( LED_YELLOW);

//#ifndef ENABLE_LOOPBACK_TEST
    uart_start();
	
		nrf_gpio_pin_clear( LED_GREEN );
		//nrf_gpio_pin_clear( LED_YELLOW );
	
		#ifdef WITH_ADXL_I2C
			if (!twi_master_init())
			{   
					// if TWi initialization fails.
					while (true) 
					{
							// Do nothing.
							//halt here.
					}
			}
		#endif
		nrf_gpio_pin_set( LED_GREEN );
	
    while (true)
    {
        uint8_t cr = simple_uart_get();
        simple_uart_put(cr);
				
				simple_uart_putstring((const uint8_t *)"\r\n");
				
				nrf_gpio_pin_set( LED_GREEN );
				nrf_delay_ms(1000);
				nrf_gpio_pin_clear( LED_GREEN );
				nrf_delay_ms(1000);

        if (cr == 'q' || cr == 'Q')
        {            
            while (true)
            {
                // Do nothing.
            }
        }
				else if(cr == '1' || cr == '+'){
						nrf_gpio_pin_set( LED_GREEN );
				}
				else if(cr == '0' || cr == '-' ){
						nrf_gpio_pin_clear( LED_GREEN );
				}
				
				#ifdef WITH_ADXL
					#ifdef WITH_ADXL_I2C
						simple_uart_putstring((const uint8_t *)"\n\radxl init...\r\n");
						adxl_init_succeeded = adxl_int(ADXL_ADDRESS);
						
						if( adxl_init_succeeded ){
								simple_uart_putstring((const uint8_t *)"\n\radxl init...\t[OK]\r\n");
							
								adxl_init_succeeded &= adxl_product_id_read(product_id, PRODUCT_ID_BYTES);
								
								if( adxl_init_succeeded ){
									
									simple_uart_putstring((const uint8_t *)"\n\rID Read...\t[OK]\r\n");
									
									for (uint8_t i = 0; i < 1; i++)
									{
											simple_uart_put(product_id[i]);
									}							
								}
								else{
									simple_uart_putstring((const uint8_t *)"\n\rID Read...\t[FAILED]\r\n");
								}
								
						}
						else{
							simple_uart_putstring((const uint8_t *)"\n\radxl init...\t[FAILED]\r\n");
						}					
					#endif
					
					#ifdef WITH_ADXL_SPI
						simple_uart_putstring((const uint8_t *)"\n\rADXL SPI init...[OK]\r\n");
						
						//Initialize SPI master.
						uint32_t err_code = spi_master_init();
						APP_ERROR_CHECK(err_code);
						
						//Register SPI master event handler.
						spi_master_evt_handler_reg(SPI_MASTER_HW, spi_master_event_handler);
						
						if (m_transfer_completed){
							
							m_transfer_completed = true;
							simple_uart_putstring((const uint8_t *)"\n\rADXL SPI data send recv initiated...\t[OK]\r\n");
							//Set buffers and start data transfer.
							spi_send_recv(m_tx_data, m_rx_data, TX_RX_BUF_LENGTH);							
							
						}
						
					#endif
						
				#endif
				
				
				
    }
		
		uart_quit();
		
		
//#else
  // This part of the example is just for testing the loopback .
//  while(true)
//{
//  uart_loopback_test();
//}
//#endif
		
}
/** @} */
