#include "Scheduler.h"

int main(void)
{
    //int TX0_level = 0;
    //int TX1_level = 0;
    //int OUT_level = 0;

    atmel_start_init();
	Enable_global_interrupt();

	printf("\r\r**** DESIGNED BY HSM 22.06 ****\r\r");

    //;THCV241_A_not_use_sublink_mainlink_1lane();
    //THCV241A_for_sublink_set();
    
#if 0
	value1 = I2C_0_read2Register(0x1a, 0x60, 0x05);
	printf("0x6005 r= %d\r", value1);
	
	value1 = I2C_0_read2Register(0x1a, 0x8a, 0x01);
	printf("0x8a01 r= %d\r", value1);
	
	value1 = I2C_0_write2Register(0x1a,0x8a, 0x01, 0x80);
	printf("result w= %d\r", value1);
	
	value1 = I2C_0_read2Register(0x1a, 0x8a, 0x01);
	printf("0x8a01 r= %d\r", value1);
#endif

#if 0   // IO_TEST

    //LOCKN_TX0_set_level(false);
    //LOCKN_TX1_set_level(false);
    //LOCKN_OUT_set_level(false);

    TX0_level = LOCKN_TX0_get_level();
    printf("TX0_get level = %d\n",TX0_level);
    
    TX1_level = LOCKN_TX1_get_level();
    printf("TX1_get level = %d\n",TX1_level);

    OUT_level = LOCKN_OUT_get_level();
    printf("OUT get level = %d\n\n",OUT_level);

#endif

	while (1)
    {    
        UART_TX_STR();
        _delay_ms(100);
	}
}




