//программf с использованием прерываний, реализующая функцию секундомера с шагом 1 мс. 
//Секундомер запускается и останавливается кратковременным нажатием на кнопку, подключенную к RG#. 
//Текущее значение измеренного времени и результат измерения выводятся на экран.

#include <xc.h>
#define FOSC 7370000ULL
#define FCY (FOSC / 2)
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <stdint.h>
#define LCD_ADRESS 0x7E
#define PIN_EN (1 << 2)
#define BACKLIGHT (1 << 3)
#define RET_STRING 0b10000000
#define LCD_CLEAR 0b00000001

int is_stop = 0; 
typedef enum
{
    COMMAND=0,
    DATA=1
} 
LCDType_t;

void initI2C(void)
{
    I2C1CONbits.I2CEN = 1; //Включить I2C
    I2C1BRG = 0x95; // частота синхронизации  
}

void I2CWrite(uint8_t adr,uint8_t data)
{
    while(I2C1CONbits.PEN);
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN != 0);
    I2C1TRN = adr;
    while (I2C1STATbits.TRSTAT == 1);
    I2C1TRN = data;
    while (I2C1STATbits.TRSTAT == 1);
    I2C1CONbits.PEN = 1;
}

void I2CWriteBuff(uint8_t adr,uint8_t* data,uint16_t len)
{
    uint16_t i;
    while(I2C1CONbits.PEN);
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN != 0);
    I2C1TRN = adr;
    for(i=0;i<len;i++)
    {
        while (I2C1STATbits.TRSTAT == 1);
        I2C1TRN=data[i];
    }
    while (I2C1STATbits.TRSTAT == 1);
    I2C1CONbits.PEN = 1;
}

void LCDSend(uint8_t lcd_addr, char data, LCDType_t flags)
{
    uint8_t high = data & 0xF0;
    uint8_t low = (data << 4) & 0xF0;
    uint8_t data_arr[6];
    data_arr[0] = high|flags|BACKLIGHT;
    data_arr[1] = high|flags|BACKLIGHT|PIN_EN;
    data_arr[2] = high|flags|BACKLIGHT;
    data_arr[3] = low|flags|BACKLIGHT;
    data_arr[4] = low|flags|BACKLIGHT|PIN_EN;
    data_arr[5] = low|flags|BACKLIGHT;
    I2CWriteBuff(lcd_addr,data_arr,6);
}

void LCDInit(void)
{
    LCDSend(LCD_ADRESS, 0b00110000,COMMAND);
    LCDSend(LCD_ADRESS, 0b00000010,COMMAND);
    LCDSend(LCD_ADRESS, 0b00001100,COMMAND);
    LCDSend(LCD_ADRESS, LCD_CLEAR,COMMAND);
}

void LCDPrintStr(char* str, uint16_t len)
{
    uint16_t i;
    for(i=0;i<len;i++)
    {
        LCDSend(LCD_ADRESS, str[i],DATA);
    }
}

void LCDPrintInt(int number) 
{
    int i;
    char *str[10]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}; 
    char *num[5];
    for(i=0; i<5;i++) 
    {
        num[i]=str[number%10]; 
        number=number/10; 
    }
    LCDSend(LCD_ADRESS, RET_STRING, COMMAND);
    for(i=6; i > 1;i--) LCDPrintStr(num[i-2],1); 
}

void __attribute__((interrupt,no_auto_psv))_T1Interrupt()
{
    if (PORTGbits.RG6 == 0) 
{
        if (PORTGbits.RG6 == 0) 
{
            is_stop = is_stop ^ 1;
            LATDbits.LATD5 = !LATDbits.LATD5; 
            __delay_ms(100);
}
    }
    IFS0bits.T1IF=0;
}

void Timer_init(){
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCS = 0; // Select internal instruction cycle clock
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11; // Select 1:125 Prescaler
    TMR1 = 0x0000; // Clear timer register
    PR1 = 0x3839; // Load the period value
    IPC0bits.T1IP = 0x01; // Set Timer 1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T1CONbits.TON = 1; // Start Timer
}

int main(int argc, char** argv){
    TRISDbits.TRISD5 = 0;
    TRISGbits.TRISG6 = 1;
    CNPUGbits.CNPUG6 = 1;
    initI2C();
    LCDInit();
    Timer_init();
    int time = 0;
    while(1){
        if (is_stop != 0) 
{
            time++;
            LCDSend(LCD_ADRESS, LCD_CLEAR, COMMAND);
            LCDPrintInt(time);
            __delay_ms(1);
}
    }
    return(EXIT_SUCCESS);
}
