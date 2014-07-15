#line 1 "..\\adxl_345b.c"



#line 1 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdbool.h"
 






 





#line 25 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdbool.h"



#line 5 "..\\adxl_345b.c"
#line 1 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"
 
 





 







 




  
 







#line 37 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"







 

     

     
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

     
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

     

     
     
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

     
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

     

     
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

     
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

     
typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;

     
typedef   signed       __int64 intmax_t;
typedef unsigned       __int64 uintmax_t;




     

     





     





     





     

     





     





     





     

     





     





     





     

     


     


     


     

     


     


     


     

     



     



     


     
    
 



#line 208 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"

     







     










     











#line 272 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"



 



#line 6 "..\\adxl_345b.c"

#line 1 "..\\..\\..\\..\\Include\\twi_master.h"
 









 




 

#line 19 "..\\..\\..\\..\\Include\\twi_master.h"
#line 20 "..\\..\\..\\..\\Include\\twi_master.h"
















 






 
 
 
#line 53 "..\\..\\..\\..\\Include\\twi_master.h"
 















 
_Bool twi_master_init(void);




















 
_Bool twi_master_transfer(uint8_t address, uint8_t *data, uint8_t data_length, _Bool issue_stop_condition);



 

 
#line 8 "..\\adxl_345b.c"
#line 1 "..\\adxl_345b.h"







 

#line 11 "..\\adxl_345b.h"
#line 12 "..\\adxl_345b.h"


#line 44 "..\\adxl_345b.h"








	









 

_Bool adxl_int(uint8_t dev_address);





 
_Bool adxl_product_id_verify(void);









 
_Bool adxl_product_id_read(uint8_t *product_id, uint8_t product_id_bytes);



#line 9 "..\\adxl_345b.c"



static const uint8_t expected_product_id[8U] = {1, 1, 1, 0, 0, 1, 0, 1};  


_Bool adxl_int(uint8_t dev_address){
	 _Bool transfer_succeeded = 1;
	
    

    return transfer_succeeded;
}

_Bool adxl_product_id_verify(void){
	
		_Bool    transfer_succeeded = 1;
    uint8_t product_id[8U];
    transfer_succeeded &= adxl_product_id_read(product_id, 8U);

    for (uint8_t i = 0; i < 10; i++)
    {
        if (product_id[i] != expected_product_id[i])
        {
            transfer_succeeded = 0;
        }
    }

    return transfer_succeeded;
}

_Bool adxl_product_id_read(uint8_t * product_id, uint8_t product_id_bytes)
{
    uint8_t w2_data[1];
    _Bool transfer_succeeded = 1;
		
	
		w2_data[0] = 0x00 ;
		transfer_succeeded &= twi_master_transfer(0XA6, w2_data, 1, ((_Bool)0));		
	
    if (transfer_succeeded) 
    {
        transfer_succeeded &= twi_master_transfer(0XA7, product_id, product_id_bytes, ((_Bool)1));
    }
    return transfer_succeeded;
}
