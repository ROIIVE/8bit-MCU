/*
 * Scheduler.c
 *
 * Created: 2022-07-05 오전 9:17:56
 *  Author: Sumin.Hwang
 */ 
#include "Scheduler.h"

 ISR(USART0_RXC_vect)
 {
     while ((uart0_rx_buff[uart0_rx_len] = USART_0_read())!='\n')   
     { 
         if(uart0_rx_buff[uart0_rx_len] == '\b')    //Backspace
         {            
             if(uart0_rx_len != 0)
             {
                 uart0_rx_len--;
             }
         }        
         else  uart0_rx_len++;
     }   
     sq =1;
 } 
 
void UART_TX_STR()
{
    uint16_t SA = 0;    
    uint32_t RA = 0;
    uint8_t RH = 0;
    uint8_t RL = 0;
    uint16_t DA = 0;
    
    char *end;
    
    int error = 0;
    uint8_t DATA;
    
    switch(sq)
    {
        case 0:     break;            
        case 1:
            
#if 1       ///////////////////////////////////////////////////////////////
            // s0
            if(strncmp(uart0_rx_buff,"iic s0 ",7)==0)
            {
                strcpy(SA_buff,uart0_rx_buff+7);  
              
                SA = strtol(SA_buff,&end,16);
                
                if(((SA>>8)&255)==0)
                {
                    SA = strtol(SA_buff,&end,16);
                    printf("\nSlave Address = 0x%02X\n",SA);
                }
                else
                {
                    printf("\nENTER Slave Address Again\n");
                }
            }
            
#else   //s0 debug 

            //s0
            if(strncmp(uart0_rx_buff,"iic s0 ",7)==0)
            { 
                strcpy(SA_buff,uart0_rx_buff+7);
                printf("i2c_buff = %s",SA_buff);

                SA = strtol(SA_buff,&end,16);

                if(((SA>>8)&255)==0)
                {
                    SA = strtol(SA_buff,&end,16);
                    printf("Slave Address = 0x%02X",SA);
                }
                else
                {
                    printf("\nENTER Slave Address Again\n");
                }
            }
#endif
            
#if 0   // write,read 1byte register

            //w1
            if(strncmp(uart0_rx_buff,"iic w1 ",7)==0)
            { 
                strcpy(RA_DA_buff,uart0_rx_buff+7);
                //printf("i2c_buff = %s",RA_DA_buff);
				
				SA = strtol(SA_buff,&end,16);
				//printf("SA = 0x%02X\n",SA);

                if (SA == 0)
                {
                    printf("ENTER SLAVE ADDRESS \n");
                }
                else
                {    
                    RA = strtol(RA_DA_buff,&end,16);
                    DA = strtol(end,&end,16);

                    if(RA_DA_buff[2]!= ' ')
                    {
                        printf("\nENTER Register Addres Again\n");
                    }
                    else
                    {
                        if(((DA>>8)&255)==0)
                        {
                            //printf("RA = 0x%02X\n",RA);
                            //printf("DA = 0x%02X\n",DA);
                            
                            error = I2C_0_write1ByteRegister(SA,RA,DA);
                            printf("error = %d\n\n",error);   //I2C_NOERR,I2C_BUSY,	I2C_FAIL
                        }
                        else
                        {
                            printf("\nENTER Data Again\n");
                        }
                    }
                }
            }

            //r1
            else if(strncmp(uart0_rx_buff,"iic r1 ",7)==0)
            {
                strcpy(RA_DA_buff,uart0_rx_buff+7);
                //printf("i2c_buff = %s",RA_DA_buff);

				SA = strtol(SA_buff,&end,16);
				//printf("SA = 0x%02X\n",SA);
                
                if (SA == 0)
                {
                    printf("ENTER SLAVE ADDRESS \n");
                }
                else
                {                      
                    if(((RA>>8)&255)==0)
                    {                    
                        DATA = I2C_0_read1ByteRegister(SA,RA);
                        printf("DATA = 0x%02X\n",DATA);
                    }
                }
            }
#endif 

#if 1       ///////////////////////////////////////////////////////////////
            // w2
            else if(strncmp(uart0_rx_buff,"iic w2 ",7)==0)
            {    
                strcpy(W_buff,uart0_rx_buff+7);
                
				SA = strtol(SA_buff,&end,16);
                
                if (SA == 0)
                {
                    printf("ENTER SLAVE ADDRESS \n");
                }
                else
                {
                    RA = strtol(W_buff,&end,16);
                    DA = strtol(end,NULL,16);
                    
                    RH = ((RA >> 8) & 255);
                    RL = (RA & 255);
                    
                    if(W_buff[4] != ' ')
                    {
                        printf("\nENTER Register Addres Again\n");
                    }
                    else
                    {
                        if(((DA>>8)&255)==0)
                        {
                            error = I2C_0_write2Register(SA,RH,RL,DA);
                            printf("\nerror = %d\n",error);
                        }
                        else
                        {
                            printf("\nENTER Data Again\n");
                        }
                    }
                    
                    for(int i =0; i< uart0_rx_len-7; i++)
                    {
                        W_buff[i] = ' ';
                    }    
                    
                }
            }

            ///////////////////////////////////////////////////////////////
            // r2
            else if(strncmp(uart0_rx_buff,"iic r2 ",7)==0)
            {    
                strcpy(R_buff,uart0_rx_buff+7);

				SA = strtol(SA_buff,&end,16);

                if (SA == 0)
                {
                    printf("ENTER SLAVE ADDRESS \n");
                }
                else
                {           
                    RA = strtol(R_buff,&end,16);
                    if(((RA>>16)&255)==0)
                    {
                        RH = ((RA >> 8) & 255);
                        RL = (RA & 255);
                        
                        DATA = I2C_0_read2Register(SA,RH,RL);
                        printf("\nDATA = 0x%02X\n",DATA);
                    }
                    else
                    {
                        printf("\nENTER Register Address Again\n");
                    }

                    for(int i =0; i< uart0_rx_len-7; i++)
                    {
                        R_buff[i] = ' ';
                    } 
                                    
                }  
            } 
            
#else  //w2,r2 debug

            //w2
            else if(strncmp(uart0_rx_buff,"iic w2 ",7)==0)
            {    
                strcpy(W_buff,uart0_rx_buff+7);
                printf("i2c_buff = %s",W_buff);

				SA = strtol(SA_buff,&end,16);
				printf("SA = 0x%02X\n",SA);
                
                if (SA == 0)
                {
                    printf("ENTER SLAVE ADDRESS \n");
                }
                else
                {    
                    RA = strtol(W_buff,&end,16);
                    DA = strtol(end,NULL,16);

                    RH = ((RA >> 8) & 255);
                    RL = (RA & 255);
                    
                    if(W_buff[4] != ' ')
                    {
                        printf("\nENTER Register Addres,Data Again\n");
                    }
                    else
                    {
                        printf("SA = 0x%02X\n",SA);
                        printf("RA = 0x%02X\n",RA);
                        printf("RH = 0x%02X\n",RH);
                        printf("RL = 0x%02X\n",RL);
                        printf("DA = 0x%02X\n",DA);

                        error = I2C_0_write2Register(SA,RH,RL,DA);
                        printf("error = %d\n",error);   //I2C_NOERR,I2C_BUSY,I2C_FAIL
                    }
                    
                    for(int i =0; i< uart0_rx_len-7; i++)
                    {
                        W_buff[i] = ' ';
                    }  
                    
                }
            }
            
            //r2
            else if(strncmp(uart0_rx_buff,"iic r2 ",7)==0)
            {    
                strcpy(R_buff,uart0_rx_buff+7);
                printf("i2c_buff = %s",R_buff);

				SA = strtol(SA_buff,&end,16);
				printf("SA = 0x%02X\n",SA);
                
                if (SA == 0)
                {
                    printf("ENTER SLAVE ADDRESS \n");
                }
                else
                {
                    RA = strtol(R_buff,&end,16);
                    if(((RA>>16)&255)==0)
                    {
                        RH = ((RA >> 8) & 255);
                        RL = (RA & 255);

                        printf("RH = 0x%02X\n",RH);
                        printf("RL = 0x%02X\n",RL);
                    
                        DATA = I2C_0_read2Register(SA,RH,RL);
                        printf("DATA = 0x%02X\n",DATA);
                    }
                    else
                    {
                        printf("\nENTER Register Address Again\n");
                    }
                    
                    for(int i =0; i< uart0_rx_len-7; i++)
                    {
                        R_buff[i] = ' ';
                    } 
                    
                }
            }
#endif      
           
            for(int i=0; i<uart0_rx_len; i++)
            {
                uart0_tx_buff[i] = uart0_rx_buff[i];
                uart0_rx_buff[i] = ' ';
                uart0_tx_len++;
            }
            
            uart0_rx_len = 0;
            
            //print keyboard inputs
            printf("\nINPUT = ");
            
            for(int j=0; j<uart0_tx_len; j++)
            {    
                USART_0_write(uart0_tx_buff[j]);
            }
            
            printf("\n\n-----------------------------------\n\n");
            
            sq = 0;
            uart0_tx_len = 0;
            break;
    }//end sw   
}//end UART_TX_STR


void THCV241_A_not_use_sublink_mainlink_1lane(void)
{
	uint8_t cnt=0, value=0;

	printf("MainLink TX0 LOCKN ...................\r");
	
    // * [Tx Register Setting]
    cnt +=I2C_0_write2Register(0x34,0x00,0x10,0x00);
    cnt +=I2C_0_write2Register(0x34,0x10,0x1D,0x00);
    cnt +=I2C_0_write2Register(0x34,0x10,0x1E,0x10);
    cnt +=I2C_0_write2Register(0x34,0x10,0x00,0x00);
    cnt +=I2C_0_write2Register(0x34,0x10,0x01,0x00);

    value = I2C_0_read2Register(0x34,0x00,0x10);
	printf("0x00,0x10 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x1D);
	printf("0x10,0x1D r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x1E);
	printf("0x10,0x1E r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x00);
	printf("0x10,0x00 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x01);
	printf("0x10,0x01 r= 0x%x\r\r", value);
    
    // * ** PLL Settings **
    cnt +=I2C_0_write2Register(0x34,0x10,0x0F,0x01);
    cnt +=I2C_0_write2Register(0x34,0x10,0x11,0x32);
    cnt +=I2C_0_write2Register(0x34,0x10,0x12,0x88);
    cnt +=I2C_0_write2Register(0x34,0x10,0x13,0x00);
    cnt +=I2C_0_write2Register(0x34,0x10,0x14,0x00);
    cnt +=I2C_0_write2Register(0x34,0x10,0x15,0x73);
    cnt +=I2C_0_write2Register(0x34,0x10,0x16,0x01);

	value = I2C_0_read2Register(0x34,0x10,0x0F);
	printf("0x10,0x0F r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x11);
	printf("0x10,0x11 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x12);
	printf("0x10,0x12 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x13);
	printf("0x10,0x13 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x14);
	printf("0x10,0x14 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x15);
	printf("0x10,0x15 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x16);
	printf("0x10,0x16 r= 0x%x\r\r", value);
    
    cnt +=I2C_0_write2Register(0x34,0x10,0x2B,0x05);
    cnt +=I2C_0_write2Register(0x34,0x10,0x2F,0x00);
    cnt +=I2C_0_write2Register(0x34,0x10,0x07,0x00);
    cnt +=I2C_0_write2Register(0x34,0x10,0x2D,0x13);
    cnt +=I2C_0_write2Register(0x34,0x10,0x2C,0x01);
    cnt +=I2C_0_write2Register(0x34,0x10,0x05,0x01);
    cnt +=I2C_0_write2Register(0x34,0x10,0x06,0x01);

    cnt +=I2C_0_write2Register(0x34,0x10,0x53,0x00);//? --> distribution mode OFF

	value = I2C_0_read2Register(0x34,0x10,0x2B);
	printf("0x10,0x2B r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x2F);
	printf("0x10,0x2F r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x07);
	printf("0x10,0x07 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x2D);
	printf("0x10,0x2D r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x2C);
	printf("0x10,0x2C r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x05);
	printf("0x10,0x05 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x06);
	printf("0x10,0x06 r= 0x%x\r\r", value);

	value = I2C_0_read2Register(0x34,0x10,0x53);
	printf("0x10,0x53 r= 0x%x\r\r", value);


#if 0

	cnt +=I2C_0_write2Register(0x34,0x10,0x5F,0x01);
	
	cnt +=I2C_0_write2Register(0x34,0x10,0x68,0x07);
	cnt +=I2C_0_write2Register(0x34,0x10,0x69,0x80);

#endif

    printf("Err_CNT=%d ...................\r\r", cnt);
    
}


void THCV241A_for_sublink_set(void)
{
	uint8_t cnt=0, value;

///////////////////////////////////////////////////////////////////////////
	//* [Tx Register Setting]
//;	SensorWrite1B(0x34,0xFE,0x11);//=

	//* [Rx Register Setting only for Pass Through]
//;	SensorWrite1B(0x0B,0x00,0x32,0x00);//=

	//* [Tx Register Settings]
//;	cnt +=I2C_0_write2Register(0x34,0x00,0xFE,0x11);//=
	
	cnt +=I2C_0_write2Register(0x34,0x00,0x73,0x00);//=
	cnt +=I2C_0_write2Register(0x34,0x00,0x72,0x22);//=
	cnt +=I2C_0_write2Register(0x34,0x00,0x70,0x03);//=
	cnt +=I2C_0_write2Register(0x34,0x00,0x7F,0x19);//=
	cnt +=I2C_0_write2Register(0x34,0x00,0x76,0x15);//=
	cnt +=I2C_0_write2Register(0x34,0x00,0xC9,0x05);//=
	cnt +=I2C_0_write2Register(0x34,0x00,0xCA,0x05);//=

	value = I2C_0_read2Register(0x34,0x00,0x73);
	printf("0x00,0xF3 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x00,0x72);
	printf("0x00,0xF2 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x00,0x70);
	printf("0x00,0xF0 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x00,0x7F);
	printf("0x00,0xFF r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x00,0x76);
	printf("0x00,0xF6 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x00,0xC9);
	printf("0x00,0xC9 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x00,0xCA);
	printf("0x00,0xCA r= 0x%x\r\r", value);

	//* [Tx Register Settings]
	cnt +=I2C_0_write2Register(0x34,0x00,0xFE,0x21);//=

	//* [PLL Settings for Pass Through mode]
	cnt +=I2C_0_write2Register(0x34,0x10,0x0F,0x01);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x11,0x32);//36
	cnt +=I2C_0_write2Register(0x34,0x10,0x12,0x88);//24
	cnt +=I2C_0_write2Register(0x34,0x10,0x13,0x00);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x14,0x00);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x15,0x73);//75
	cnt +=I2C_0_write2Register(0x34,0x10,0x16,0x01);//=
	
	cnt +=I2C_0_write2Register(0x34,0x10,0x00,0x00);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x01,0x00);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x02,0x00);//=

	value = I2C_0_read2Register(0x34,0x10,0x0F);
	printf("0x10,0xF3 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x11);
	printf("0x10,0x11 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x12);
	printf("0x10,0x12 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x13);
	printf("0x10,0x13 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x14);
	printf("0x10,0x14 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x15);
	printf("0x10,0x15 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x16);
	printf("0x10,0x16 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x00);
	printf("0x10,0x00 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x01);
	printf("0x10,0x01 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x02);
	printf("0x10,0x02 r= 0x%x\r\r", value);
	

	// del...
	cnt +=I2C_0_write2Register(0x34,0x10,0x53,0x00);//? --> distribution mode OFF
	cnt +=I2C_0_write2Register(0x34,0x10,0x55,0x00);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x04,0x00);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x2B,0x05);//=
	
	cnt +=I2C_0_write2Register(0x34,0x10,0x27,0x00);//=
	
	cnt +=I2C_0_write2Register(0x34,0x10,0x07,0x00);//default
	cnt +=I2C_0_write2Register(0x34,0x10,0x08,0x00);//default
	cnt +=I2C_0_write2Register(0x34,0x10,0x09,0x00);//default
	cnt +=I2C_0_write2Register(0x34,0x10,0x0C,0x00);//default
	cnt +=I2C_0_write2Register(0x34,0x10,0x0D,0x00);//default

	value = I2C_0_read2Register(0x34,0x10,0x53);
	printf("0x10,0x53 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x55);
	printf("0x10,0x55 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x04);
	printf("0x10,0x04 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x2B);
	printf("0x10,0x2B r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x27);
	printf("0x10,0x27 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x07);
	printf("0x10,0x07 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x08);
	printf("0x10,0x08 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x09);
	printf("0x10,0x09 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x0C);
	printf("0x10,0x0C r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x0D);
	printf("0x10,0x0D r= 0x%x\r\r", value);

	
	cnt +=I2C_0_write2Register(0x34,0x10,0x2D,0x13);//--> 4lane �Է�...
	cnt +=I2C_0_write2Register(0x34,0x10,0x2C,0x01);//=
	
	cnt +=I2C_0_write2Register(0x34,0x10,0x05,0x01);//=
	cnt +=I2C_0_write2Register(0x34,0x10,0x06,0x01);//=

	// MIPI ID error additional DataType (=DT) check option.
	cnt +=I2C_0_write2Register(0x34, 0x10, 0x27, 0x00);
	// V-by-One(R) HS HTPDN assignment
	cnt +=I2C_0_write2Register(0x34, 0x10, 0x1D, 0x00);
	// V-by-One(R) HS LOCKN for lane0/lane1 assignment
	cnt +=I2C_0_write2Register(0x34, 0x10, 0x1E, 0x00);

	value = I2C_0_read2Register(0x34,0x10,0x2D);
	printf("0x10,0x2D r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x2C);
	printf("0x10,0x2C r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x05);
	printf("0x10,0x05 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x06);
	printf("0x10,0x06 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x27);
	printf("0x10,0x27 r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x1D);
	printf("0x10,0x1D r= 0x%x\r", value);
	value = I2C_0_read2Register(0x34,0x10,0x1E);
	printf("0x10,0x1E r= 0x%x\r\r", value);

	//
	/////////////////////////////////////////////////////////////////////////////

	printf("Err_CNT=%d\r", cnt);
	
}

