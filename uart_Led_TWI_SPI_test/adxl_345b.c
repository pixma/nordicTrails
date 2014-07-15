


#include <stdbool.h>
#include <stdint.h>

#include "twi_master.h"
#include "adxl_345b.h"

#define PRODUCT_ID_BYTES 8U //!< Number of bytes to expect to be in product ID

static const uint8_t expected_product_id[PRODUCT_ID_BYTES] = {1, 1, 1, 0, 0, 1, 0, 1};  //!< Product ID expected to get from product ID query


bool adxl_int(uint8_t dev_address){
	 bool transfer_succeeded = true;
	
    //transfer_succeeded &= adxl_product_id_verify();

    return transfer_succeeded;
}

bool adxl_product_id_verify(void){
	
		bool    transfer_succeeded = true;
    uint8_t product_id[PRODUCT_ID_BYTES];
    transfer_succeeded &= adxl_product_id_read(product_id, PRODUCT_ID_BYTES);

    for (uint8_t i = 0; i < 10; i++)
    {
        if (product_id[i] != expected_product_id[i])
        {
            transfer_succeeded = false;
        }
    }

    return transfer_succeeded;
}

bool adxl_product_id_read(uint8_t * product_id, uint8_t product_id_bytes)
{
    uint8_t w2_data[1];
    bool transfer_succeeded = true;
		
	
		w2_data[0] = ADXL_DEVID ;
		transfer_succeeded &= twi_master_transfer(ADXL_REGISTER_WRITE, w2_data, 1, TWI_DONT_ISSUE_STOP);		
	
    if (transfer_succeeded) 
    {
        transfer_succeeded &= twi_master_transfer(ADXL_REGISTER_READ, product_id, product_id_bytes, TWI_ISSUE_STOP);
    }
    return transfer_succeeded;
}
