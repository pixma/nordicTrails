#line 1 "..\\..\\..\\..\\Source\\ext_sensors\\synaptics_touchpad\\synaptics_touchpad.c"










 

#line 1 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdbool.h"
 






 





#line 25 "C:\\Keil_v5\\ARM\\ARMCC\\bin\\..\\include\\stdbool.h"



#line 14 "..\\..\\..\\..\\Source\\ext_sensors\\synaptics_touchpad\\synaptics_touchpad.c"
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



 



#line 15 "..\\..\\..\\..\\Source\\ext_sensors\\synaptics_touchpad\\synaptics_touchpad.c"

#line 1 "..\\..\\..\\..\\Include\\twi_master.h"
 









 




 

#line 19 "..\\..\\..\\..\\Include\\twi_master.h"
#line 20 "..\\..\\..\\..\\Include\\twi_master.h"
















 






 
 
 
#line 53 "..\\..\\..\\..\\Include\\twi_master.h"
 















 
_Bool twi_master_init(void);




















 
_Bool twi_master_transfer(uint8_t address, uint8_t *data, uint8_t data_length, _Bool issue_stop_condition);



 

 
#line 17 "..\\..\\..\\..\\Source\\ext_sensors\\synaptics_touchpad\\synaptics_touchpad.c"
#line 1 "..\\..\\..\\..\\Include\\ext_sensors\\synaptics_touchpad.h"










 




 

#line 19 "..\\..\\..\\..\\Include\\ext_sensors\\synaptics_touchpad.h"
#line 20 "..\\..\\..\\..\\Include\\ext_sensors\\synaptics_touchpad.h"









 



 
#line 41 "..\\..\\..\\..\\Include\\ext_sensors\\synaptics_touchpad.h"







 
typedef enum                     
{  
  SleepmodeNormal        = 0x00,  
  SleepmodeSensorSleep   = 0x01  
} TouchpadSleepMode_t;






 
_Bool touchpad_init(uint8_t device_address);





 
_Bool touchpad_reset(void);






 
_Bool touchpad_interrupt_status_read(uint8_t *interrupt_status);







 
_Bool touchpad_set_sleep_mode(TouchpadSleepMode_t mode);







 
_Bool touchpad_read_register(uint8_t register_address, uint8_t *value);







 
_Bool touchpad_write_register(uint8_t register_address, uint8_t value);








 
_Bool touchpad_product_id_read(uint8_t *product_id, uint8_t product_id_bytes);





 
_Bool touchpad_product_id_verify(void);



 

 

#line 18 "..\\..\\..\\..\\Source\\ext_sensors\\synaptics_touchpad\\synaptics_touchpad.c"

 



static uint8_t       m_device_address; 
static const uint8_t expected_product_id[10U] = {'T', 'M', '1', '9', '4', '4', '-', '0', '0', '2'};  

_Bool touchpad_init(uint8_t device_address)
{
    _Bool transfer_succeeded = 1;

    m_device_address = (uint8_t)(device_address << 1);

    
    uint8_t reset_command = 0x01;
    transfer_succeeded &= touchpad_write_register(0x54, reset_command);

    
    uint8_t page_to_select = 0x00;
    transfer_succeeded &= touchpad_write_register(0xFF, page_to_select);

    
    transfer_succeeded &= touchpad_product_id_verify();

    return transfer_succeeded;
}


_Bool touchpad_product_id_verify(void)
{
    _Bool    transfer_succeeded = 1;
    uint8_t product_id[10U];
    transfer_succeeded &= touchpad_product_id_read(product_id, 10U);

    for (uint8_t i = 0; i < 10; i++)
    {
        if (product_id[i] != expected_product_id[i])
        {
            transfer_succeeded = 0;
        }
    }

    return transfer_succeeded;
}

_Bool touchpad_reset(void)
{
    uint8_t w2_data[2] = {0x8F, 0x01};

    return twi_master_transfer(m_device_address, w2_data, 2, ((_Bool)1));
}

_Bool touchpad_interrupt_status_read(uint8_t *interrupt_status)
{
    return touchpad_read_register(0x14, interrupt_status);
}

_Bool touchpad_set_sleep_mode(TouchpadSleepMode_t mode)
{
    return touchpad_write_register(0x42, (uint8_t)mode);
}

_Bool touchpad_read_register(uint8_t register_address, uint8_t *value)
{
    _Bool transfer_succeeded = 1;
    transfer_succeeded &= twi_master_transfer(m_device_address, &register_address, 1, ((_Bool)0));
    if (transfer_succeeded) 
    {
        transfer_succeeded &= twi_master_transfer(m_device_address | (0x01), value, 1, ((_Bool)1));
    }
    return transfer_succeeded;
}

_Bool touchpad_write_register(uint8_t register_address, const uint8_t value)
{
    uint8_t w2_data[2];

    w2_data[0] = register_address;
    w2_data[1] = value;
    return twi_master_transfer(m_device_address, w2_data, 2, ((_Bool)1));
}

_Bool touchpad_product_id_read(uint8_t * product_id, uint8_t product_id_bytes)
{
    uint8_t w2_data[1];
    _Bool transfer_succeeded = 1;

    w2_data[0] = 0xA2;
    transfer_succeeded &= twi_master_transfer(m_device_address, w2_data, 1, ((_Bool)0));
    if (transfer_succeeded) 
    {
        transfer_succeeded &= twi_master_transfer(m_device_address | (0x01), product_id, product_id_bytes, ((_Bool)1));
    }
    return transfer_succeeded;
}

 
