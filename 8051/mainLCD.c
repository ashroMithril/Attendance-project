#include<reg51.h>
#include "r305-fingerprint.h"
#include <stdio.h>
#include<lcd_8051.h>



//pin description for the lcd interface --- 8 bit mode

/*
sbit D0=P2^0;
sbit D1=P2^1;
sbit D2=P2^2;
sbit D3=P2^3;
sbit D4=P2^4;
sbit D5=P2^5;
sbit D6=P2^6;
sbit D7=P2^7;
*/
sfr LCD = 0xa0 ;//special function register set for LCD {Checked on Proteus}
sbit EN=P3^6;//Enable pin is for starting or enabling the module. A high
             //to low pulse of about 450ns pulse is given to this pin.

sbit RS=P3^0;//RS is the register select pin used to write display
             //data to the LCD (characters), this pin has to be high
             //when writing the data to the LCD. During the initializing
             //sequence and other commands this pin should low.
 
sbit SW_ADD        = P1^0;
sbit SW_SEARCH     = P1^1;
sbit SW_EMPTY      = P1^2;
sbit LED = P3^5 ;

 
 
sbit FP_ADD         = P3^2;
sbit FP_EMPTY       = P3^3;
sbit FP_SEARCH      = P3^4;
 

void main()
{
/*To use the 8051's on-board serial port, it is generally necessary to initialize the following
SFRs: SCON, TCON, and TMOD. This is because SCON controls the serial port. However, in most cases the
program will wish to use one of the timers to establish the serial port's baud rate. In this case, it is necessary to
configure timer 1 by initializing TCON and TMOD.
*/
    //  Intialize variables 
    response = 0;
         
    //  Intialise Serial Port 
    //Sets up MCU to use 9600 bps @ 11.059 MHz Crystal
    SCON = 0x52; //0011 0010, 8-bit UART mode , serial control , This SFR controls the baud rate of the serial
                 //port, whether the serial port is activated to receive data, and also contains flags that are set when a byte
                 //is successfully sent or received.
    TMOD = 0x20; //0010 0000 [timer 1 , mode 2] 8-bit auto reload timer/counter; THx(timer high) holds a value which is to be reloaded TLx(timer low) each time it overfolws , {mazidi , page no. 317}
    TH1= 0xfd;   // 9600 8-n-1 {mazidi pgno. 336}
    TR1 = 1; // run timer1
     
    //  Program Loop 
    while(1)
    {
         
        P0=0xff;
        P1=0xff;
        delay_ms(100);
        initial_lcd();
        while(1){
	  
        write_lcd(0x81,0);
        string_to_lcd("HELLO");
        write_lcd(0xc4,0);
        string_to_lcd("EVERYONE");
        write_lcd(0x01,0);
        delay_ms(100);
	
	
        write_lcd(0x81,0);
        string_to_lcd("PLACE YOUR");
        write_lcd(0xc4,0);
        string_to_lcd("FINGER")
        write_lcd(0x01,0);
        delay_ms(100);


	LED = 0 ;
	response = scan() ;
	if (response == CMD_SUCCESS){
	
	write_lcd(0x81,0);
        string_to_lcd("FINGER");
        write_lcd(0xc4,0);
        string_to_lcd("DETECTED");
        write_lcd(0x01,0);
        delay_ms(100);

	LED = 1 ;// check the LED , delay timings with the scan() function 
	}
	
	else {
	write_lcd(0x81,0);
        string_to_lcd("ERROR");
        write_lcd(0xc4,0);
        string_to_lcd("TRY AGAIN");
        write_lcd(0x01,0);
        delay_ms(100);
	}
//////When the finger is detected , options to auth / enroll / delete 

	write_lcd(0x81,0);
        string_to_lcd("PRESS ANY");
        write_lcd(0xc4,0);
        string_to_lcd("KEY");
        write_lcd(0x01,0);
        delay_ms(100);

///////// Add           
            if(SW_ADD==0)   // check for Add switch
            {               
                // Trigger Add Function
                FP_ADD = 0;
                delay_ms(50);
                FP_ADD = 1;
                 
               
                response = enrol();
                if(response==ENROL_SUCCESS) 
                {
                    
                    write_lcd(0xc4,0);
                    string_to_lcd("SUCCESS");
                    write_lcd(0x01,0);
                    delay_ms(100);
                } else
                {                   
                 

                    write_lcd(0xc4,0);
                    string_to_lcd("ERROR");
                    write_lcd(0x01,0);
                    delay_ms(100);
            }
             
/////////CLEAR LIBRARY [can't clear one FP , it will clear the whole lib]
            if(SW_EMPTY==0)   // check for Add switch
            {               
                // Trigger Empty Function
                FP_EMPTY = 0;
                delay_ms(50);
                FP_EMPTY = 1;
                 
               
                response = clrlib();
                if(response==CMD_SUCCESS) 
                {
                    
                    write_lcd(0xc4,0);
                    string_to_lcd("SUCCESS");
                    write_lcd(0x01,0);
                    delay_ms(100);
                } else
                {                   
                 

                    write_lcd(0xc4,0);
                    string_to_lcd("ERROR");
                    write_lcd(0x01,0);
                    delay_ms(100);
            }
             
         
         } //end if     

	if(SW_SEARCH==0)   // check for SEARCH switch
            {               
                // Trigger Empty Function
                FP_SEARCH = 0;
                delay_ms(50);
                FP_SEARCH = 1;
                 
               
                response = ;
          
             
         
         } //end if        
    } // end while
}// end main
