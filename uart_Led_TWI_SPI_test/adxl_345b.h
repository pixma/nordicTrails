


#ifndef ADXL_345B_H
#define ADXL_345B_H


/*lint ++flb "Enter library region" */

#include <stdbool.h>
#include <stdint.h>


#define ADXL_DEVID								0x00
#define ADXL_THRESH_TAP						0x1D
#define ADXL_OFSX									0x1E
#define ADXL_OFSY									0X1F
#define ADXL_OFSZ									0X20
#define ADXL_TAP_DUR							0X21
#define ADXL_TAP_LATENT						0X22
#define ADXL_TAP_WINDOW						0X23
#define ADXL_THRESH_ACTIVITY			0X24
#define ADXL_THRESH_INACTIVITY		0X25
#define ADXL_TIME_INACTIVITY			0X26
#define ADXL_ACT_INACT_CTRL				0X27
#define ADXL_THRESH_FF						0X28
#define ADXL_TIME_FF							0X29
#define ADXL_TAP_AXIS							0X2A
#define ADXL_ACT_TAP_STATUS				0X2B
#define ADXL_BW_RATE							0X2C
#define ADXL_POWER_CTRL						0X2D
#define ADXL_INT_ENABLE						0X2E
#define ADXL_INT_MAP							0X2F
#define ADXL_INT_SOURCE						0X30
#define ADXL_DATA_FORMAT					0X31
#define ADXL_DATAX0								0X32
#define ADXL_DATAX1								0X33
#define ADXL_DATAY0								0X34
#define ADXL_DATAY1								0X35
#define ADXL_DATAZ0								0X36
#define ADXL_DATAZ1								0X37
#define ADXL_FIFO_CTRL						0X38
#define ADXL_FIFO_STATUS					0X39

#define ADXL_ADDRESS							0X53			// PLEASE GROUND ALT_ADDRESS PIN .e.i SDO pin.
#define ADXL_REGISTER_READ				0XA7
#define ADXL_REGISTER_WRITE				0XA6
#define ADXL_PRODUCT_ID						0xE5

#define TWI_ISSUE_STOP               ((bool)true)  //!< Parameter for @ref twi_master_transfer
#define TWI_DONT_ISSUE_STOP          ((bool)false) //!< Parameter for @ref twi_master_transfer
	
#define TWI_READ_BIT                 (0x01)        //!< If this bit is set in the address field, transfer direction is from slave to master.

/**

	@brief Function for ADXL initialization.
	@param device address TWI address of te device.
	@retval true if successfully initialized.
	@retval false if failed to initialized.

*/

bool adxl_int(uint8_t dev_address);

/**
  @brief Function for reading and verifying adxl's product ID.
  @retval true Product ID is what was expected
  @retval false Product ID was not what was expected
*/
bool adxl_product_id_verify(void);


/**
  @brief Function for writing adxl register contents over TWI.
  Writes one or more consecutive registers.
  @param[out] product_id Pointer to a address to store product ID. Memory must be allocated for product_id_bytes number of bytes.
  @param[in]  product_id_bytes Number of bytes to read
  @retval true Product ID read succeeded
  @retval false Product ID read failed
*/
bool adxl_product_id_read(uint8_t *product_id, uint8_t product_id_bytes);

#endif

