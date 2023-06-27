// программа реализует, чтобы на дисплей выводились номера ИК датчиков на первой строке и результат 
//измерения на второй строке; переключение между датчиками осуществлять при нажатие на кнопку, подключенную к RG#.

#include <xc.h>
#define FOSC 7370000ULL
#define FCY (FOSC / 2)
#define FCY 30401250UL 
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#define LCD_ADRESS 0x4E
#define PIN_EN (1 << 2)
#define BACKLIGHT (1 << 3)
#define RET_STRING 0b10000000
#define LCD_CLEAR 0b00000001

typedef enum
{
    COMMAND=0,
    DATA=1
} 
LDCType_t;

void initI2C(void)
{
    I2C1CONbits.I2CEN = 1; //Включить I2C
    I2C1BRG = 0x95; // частота синхронизации  
}

void I2CWrite(int adr,int data)
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
void I2CWriteBuff(int adr,int* data,int len)
{
    int i;
    while(I2C1CONbits.PEN);
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN != 0);
    I2C1TRN = adr;
    for(i=0; i<len;i++)
    {
        while (I2C1STATbits.TRSTAT == 1);
        I2C1TRN=data[i];
    }
    while (I2C1STATbits.TRSTAT == 1);
    I2C1CONbits.PEN = 1;
}
void LCDSend(int lcd_addr, int data, LDCType_t flags)
{
    int high = data & 0xF0;
    int low = (data << 4) & 0xF0;
    int data_arr[6];
    data_arr[0] = high|flags|BACKLIGHT;
    data_arr[1] = high|flags|BACKLIGHT|PIN_EN;
    data_arr[2] = high|flags|BACKLIGHT;
    data_arr[3] = low|flags|BACKLIGHT;
    data_arr[4] = low|flags|BACKLIGHT|PIN_EN;
    data_arr[5] = low|flags|BACKLIGHT;
    I2CWriteBuff(lcd_addr,data_arr,6);
}
void LCDPrintStr(int* str, int len)
{
    int i;
    for(i=0;i<len;i++)
    {
        LCDSend(LCD_ADRESS, str[i],DATA);
    }
}
void LCDInit(void)
{
    LCDSend(LCD_ADRESS, 0b00110000,COMMAND);
    LCDSend(LCD_ADRESS, 0b00000010,COMMAND);
    LCDSend(LCD_ADRESS, 0b00001100,COMMAND);
    LCDSend(LCD_ADRESS, LCD_CLEAR, COMMAND);
}
void Adc_init(char i)
{
 AD1CON1bits.AD12B=1; //12-битный режим
 AD1CON2bits.VCFG=0;
 AD1CON1bits.FORM=0; //беззнаковое целое
 AD1CON1bits.ADON=1; //работа модуля АЦП
 AD1CON4bits.ADDMAEN=1; //определяет, как заполняются результаты
 AD1CHS0bits.CH0SA=i;//ANi
 AD1CHS0bits.CH0NA=0;//Отрицательный вход канала VREF
}
int read_Adc(){
 AD1CON1bits.SAMP=1; //запуск выборки
 __delay_ms(5);
 AD1CON1bits.SAMP=0; //запуск преобразования
 while(!AD1CON1bits.DONE);
 return ADC1BUF0;
}

int main(void) 
{
    int a, i;
    int j = 12;
    char *str[10]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    char *AN11[4]={"A", "N", "1", "1"};
    char *AN12[4]={"A", "N", "1", "2"};
    char *num[4];
    TRISGbits.TRISG7=1;
    CNPUGbits.CNPUG7=1;
    initI2C();
    LCDInit();
    __delay_ms(10);
    while(1)
    {
        LCDSend(LCD_ADRESS, LCD_CLEAR,COMMAND);
        if (PORTGbits.RG7 == 0) 
   {
            if (j == 12) j--;
            else j++;
        }
        Adc_init(j); 
        a=read_Adc();
        for(i=0; i<4; i++) 
        {
            num[i]=str[a%10]; //послед разряд остатка от деления
            a=a/10; // уменьшаем чтоб получить след остаток
        }
        LCDSend(LCD_ADRESS, RET_STRING_2,COMMAND);
        for(i=5; i>1;i--) {
            LCDPrintStr(num[i-2],1); //посимвольно вывод с конца
        }
        LCDSend(LCD_ADRESS, RET_STRING,COMMAND);
        if (j == 11) 
   {
            for(i=0; i<4;i++) {
                LCDPrintStr(AN11[i],1); 
            }
        }
        else 
   {
            for(i=0; i<4;i++) {
                LCDPrintStr(AN12[i],1); 
            }
        }
        __delay_ms(50);
    }
    return 0;
}
