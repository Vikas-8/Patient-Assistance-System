/*
========================================================
 REAL-TIME PATIENT ASSISTANCE SYSTEM
 TM4C123GH6PM - FINAL CORRECTED CODE
========================================================
*/

#include <stdint.h>
#include "tm4c123gh6pm.h"

/*--------------------------------------------------
 LCD MACROS
--------------------------------------------------*/
#define LCD_row1()  LCD_command(0x80)
#define LCD_row2()  LCD_command(0xC0)

/*--------------------------------------------------
 SWITCHES
--------------------------------------------------*/
#define SWITCH1 0x10
#define SWITCH2 0x01

/*--------------------------------------------------
 LED
--------------------------------------------------*/
#define LED 0x04

/*--------------------------------------------------
 BUZZER
--------------------------------------------------*/
#define BUZZER 0x08

/*--------------------------------------------------
 FUNCTION PROTOTYPES
--------------------------------------------------*/
void PortF_Init(void);
void PortB_Init(void);
void PortE_Init(void);

void LCD_init(void);
void LCD_command(unsigned char command);
void LCD_putc(unsigned char ascii);
void LCD_puts(char *lcd_string);
void shift_out1(unsigned char str);

void delayMs(int n);

void buzzer_short(void);

int Count_Presses(void);

/*--------------------------------------------------
 GLOBAL VARIABLES
--------------------------------------------------*/
unsigned char PP2 = 0x00;

int request_pending = 0;

/*==================================================
                    MAIN
==================================================*/
int main(void)
{
    int press_count;
    int wait_time;

    PortF_Init();
    PortB_Init();
    PortE_Init();

    LCD_init();

    LCD_command(0x01);

    LCD_row1();
    LCD_puts("PATIENT CAR PATIENT CARE");

    LCD_row2();
    LCD_puts("SYSTEM READY");

    delayMs(2000);

    LCD_command(0x01);

    while(1)
    {
        /*==================================================
          EMERGENCY SWITCH
        ==================================================*/
        if((GPIO_PORTF_DATA_R & SWITCH2) == 0)
        {
            delayMs(20);

            if((GPIO_PORTF_DATA_R & SWITCH2) == 0)
            {
                GPIO_PORTF_DATA_R |= LED;

                LCD_command(0x01);

                LCD_row1();
                LCD_puts("EMERGENCY!   EMERGENCY!!!");

                LCD_row2();
                LCD_puts("CALL NURSE");

                /*
                CONTINUOUS BUZZER
                UNTIL SWITCH1 PRESSED
                */

                while(1)
                {
                    GPIO_PORTB_DATA_R |= BUZZER;

                    delayMs(200);

                    GPIO_PORTB_DATA_R &= ~BUZZER;

                    delayMs(200);

                    /*
                    STOP EMERGENCY
                    USING SWITCH1
                    */

                    if((GPIO_PORTF_DATA_R & SWITCH1) == 0)
                    {
                        delayMs(20);

                        while((GPIO_PORTF_DATA_R & SWITCH1) == 0);

                        GPIO_PORTB_DATA_R &= ~BUZZER;

                        GPIO_PORTF_DATA_R &= ~LED;

                        LCD_command(0x01);

                        LCD_row1();
                        LCD_puts("EMERGENC ");

                        LCD_row2();
                        LCD_puts("  EMERGENCY ATTENDED");

                        delayMs(2000);

                        LCD_command(0x01);

                        LCD_row1();
                        LCD_puts("SYSTEM READ SYSTEM READY");

                        break;
                    }
                }
            }
        }

        /*==================================================
          PATIENT REQUEST SWITCH
        ==================================================*/
        if((GPIO_PORTF_DATA_R & SWITCH1) == 0)
        {
            press_count = Count_Presses();

            request_pending = 1;

            LCD_command(0x01);

            /*---------------- WATER ----------------*/
            if(press_count == 1)
            {
                LCD_row1();
                LCD_puts("PATIENT NEE PATIENT NEED");

                LCD_row2();
                LCD_puts("WATER");

                buzzer_short();
            }

            /*---------------- FOOD ----------------*/
            else if(press_count == 2)
            {
                LCD_row1();
                LCD_puts("PATIENT NEE PATIENT NEED");

                LCD_row2();
                LCD_puts("FOOD");

                buzzer_short();
            }

            /*-------------- MEDICINE --------------*/
            else if(press_count >= 3)
            {
                LCD_row1();
                LCD_puts("PATIENT NEE PATIENT NEED");

                LCD_row2();
                LCD_puts("MEDICINE");

                buzzer_short();
            }

            /*
            REMINDER LOOP
            */
            while(request_pending)
            {
                for(wait_time = 0; wait_time < 500; wait_time++)
                {
                    delayMs(10);

                    /*
                    CARETAKER ACKNOWLEDGE
                    USING SWITCH2
                    */

                    if((GPIO_PORTF_DATA_R & SWITCH2) == 0)
                    {
                        delayMs(20);

                        while((GPIO_PORTF_DATA_R & SWITCH2) == 0);

                        request_pending = 0;

                        LCD_command(0x01);

                        LCD_row1();
                        LCD_puts("REQUEST");

                        LCD_row2();
                        LCD_puts("ATTENDED");

                        delayMs(2000);

                        LCD_command(0x01);

                        LCD_row1();
                        LCD_puts("SYSTEM READ SYSTEM READY");

                        break;
                    }
                }

                /*
                REPEAT BUZZER
                */
                if(request_pending)
                {
                    buzzer_short();
                }
            }
        }
    }
}

/*==================================================
          COUNT MULTIPLE PRESSES
==================================================*/
int Count_Presses(void)
{
    int count = 0;

    int timeout = 0;

    while(timeout < 40)
    {
        if((GPIO_PORTF_DATA_R & SWITCH1) == 0)
        {
            delayMs(20);

            while((GPIO_PORTF_DATA_R & SWITCH1) == 0);

            count++;

            timeout = 0;
        }

        delayMs(100);

        timeout++;
    }

    return count;
}

/*==================================================
            SHORT BUZZER
==================================================*/
void buzzer_short(void)
{
    GPIO_PORTB_DATA_R |= BUZZER;

    delayMs(500);

    GPIO_PORTB_DATA_R &= ~BUZZER;
}

/*==================================================
                PORT F INIT
==================================================*/
void PortF_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;

    delayMs(1);

    GPIO_PORTF_LOCK_R = 0x4C4F434B;

    GPIO_PORTF_CR_R |= 0x1F;

    GPIO_PORTF_DIR_R |= 0x0C;

    GPIO_PORTF_DIR_R &= ~0x11;

    GPIO_PORTF_PUR_R |= 0x11;

    GPIO_PORTF_DEN_R |= 0x1F;
}

/*==================================================
                PORT B INIT
==================================================*/
void PortB_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x02;

    delayMs(1);

    GPIO_PORTB_DIR_R |= BUZZER;

    GPIO_PORTB_DEN_R |= BUZZER;
}

/*==================================================
                PORT E INIT
==================================================*/
void PortE_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x10;

    delayMs(1);

    GPIO_PORTE_DIR_R |= 0x20;

    GPIO_PORTE_DEN_R |= 0x20;
}

/*==================================================
                  LCD INIT
==================================================*/
void LCD_init(void)
{
    delayMs(20);

    LCD_command(0x30);
    delayMs(5);

    LCD_command(0x30);
    delayMs(5);

    LCD_command(0x30);
    delayMs(5);

    LCD_command(0x20);
    delayMs(5);

    LCD_command(0x28);
    delayMs(5);

    LCD_command(0x0C);
    delayMs(5);

    LCD_command(0x06);
    delayMs(5);

    LCD_command(0x01);
    delayMs(5);
}

/*==================================================
                LCD STRING
==================================================*/
void LCD_puts(char *lcd_string)
{
    while(*lcd_string)
    {
        LCD_putc(*lcd_string++);
    }
}

/*==================================================
                LCD COMMAND
==================================================*/
void LCD_command(unsigned char command)
{
    unsigned char volatile num;

    num = command;

    num = ((num & 0x11)<<3) |
          ((num & 0x22)<<1) |
          ((num & 0x44)>>1) |
          ((num & 0x88)>>3);

    command = num;

    PP2 = (PP2 & 0xF0)|((command>>4) & 0x0F);

    PP2 = PP2 & (~(3 << 7));

    PP2 = PP2 | 0x20;

    shift_out1(PP2);

    PP2 = PP2 & ~(0x20);

    shift_out1(PP2);

    PP2 = (PP2 & 0xF0)|(command & 0x0F);

    PP2 = PP2 & (~(3 << 7));

    PP2 = PP2 | 0x20;

    shift_out1(PP2);

    PP2 = PP2 & ~(0x20);

    shift_out1(PP2);
}

/*==================================================
                LCD CHARACTER
==================================================*/
void LCD_putc(unsigned char ascii)
{
    unsigned char volatile num;

    num = ascii;

    num = ((num & 0x11)<<3) |
          ((num & 0x22)<<1) |
          ((num & 0x44)>>1) |
          ((num & 0x88)>>3);

    ascii = num;

    PP2 = (PP2 & 0xF0)|((ascii>>4) & 0x0F);

    PP2 = PP2 | 0xA0;

    PP2 = PP2 & ~(0x40);

    shift_out1(PP2);

    PP2 = PP2 & ~(0x20);

    shift_out1(PP2);

    PP2 = (PP2 & 0xF0)|(ascii & 0x0F);

    PP2 = PP2 | 0xA0;

    PP2 = PP2 & ~(0x40);

    shift_out1(PP2);

    PP2 = PP2 & ~(0x20);

    shift_out1(PP2);
}

/*==================================================
              SHIFT REGISTER
==================================================*/
void shift_out1(unsigned char str)
{
    unsigned char j = 0;

    unsigned char check;

    GPIO_PORTE_DATA_R = 0x00;

    for(j = 0; j <= 7; j++)
    {
        GPIO_PORTF_DATA_R &= ~0x0C;

        check = (str & (1 << j));

        if(check)
            GPIO_PORTF_DATA_R |= 0x04;
        else
            GPIO_PORTF_DATA_R &= ~0x04;

        GPIO_PORTF_DATA_R |= 0x08;
    }

    GPIO_PORTE_DATA_R = 0x20;
}

/*==================================================
                DELAY
==================================================*/
void delayMs(int n)
{
    int i;

    NVIC_ST_CTRL_R = 0;

    NVIC_ST_RELOAD_R = 16000 - 1;

    NVIC_ST_CURRENT_R = 0;

    NVIC_ST_CTRL_R = 5;

    for(i = 0; i < n; i++)
    {
        while((NVIC_ST_CTRL_R & 0x10000) == 0);
    }
}
