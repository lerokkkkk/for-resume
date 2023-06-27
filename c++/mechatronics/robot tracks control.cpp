//программа, реализующая управление усилителем мощности для привода вращения гусениц. 
//Скважность импульсов должна задается с помощью потенциометра и выводиться на дисплей.
//С помощью потенциометра можно было задать не только скважность импульсов, но и направление движения.

#include <xc.h>
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

int is_stop = 1; 

typedef enum
{
    COMMAND=0,
    DATA=1
} LCDType_t;

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

void LCDPrintStr(char* str, uint16_t len){
    uint16_t i;
    for(i=0;i<len;i++){
        LCDSend(LCD_ADRESS, str[i],DATA);
    }
}
void LCDPrintInt(int number){
    int i;
    char *str[10]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}; 
    char *num[5];
    for(i=0; i<5;i++) {
        num[i]=str[number%10]; 
        number=number/10; 
    }
    LCDSend(LCD_ADRESS, RET_STRING, COMMAND);
    for(i=6; i > 1;i--) LCDPrintStr(num[i-2],1); 
}

void Adc_init(char i){
    AD1CON1bits.AD12B=1; // 12-битный режим
    AD1CON2bits.VCFG=0;
    AD1CON1bits.FORM=0; // беззнаковое целое
    AD1CON1bits.ADON=1; // работа модуля АЦП
    AD1CON4bits.ADDMAEN=1; // определяет, как заполняются результаты преобразования в области оперативной памяти DMA
    AD1CHS0bits.CH0SA=i;//ANi
    AD1CHS0bits.CH0NA=0;// Отрицательный вход канала VREF
}

int read_Adc(){
    AD1CON1bits.SAMP=1; //запуск выборки
    __delay_ms(5);
    AD1CON1bits.SAMP=0; //запуск преобразования
    while(!AD1CON1bits.DONE);
    return ADC1BUF0;
}

void PWM_Init( void ){
    RPOR0bits.RP35R = 0b010000;
    RPOR2bits.RP39R = 0b010001;
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 0;
    OC1R = 0;
    OC2R = 0;
    OC1CON1bits.OCM = 0b110;
    OC2CON1bits.OCM = 0b110;
    PR2 = 3685; // Timer 2 period (20ms) Fcy x 20ms / 8
    T2CONbits.TCKPS = 0b00; // Timer 2 prescaler 1:8
    T2CONbits.TON = 1;
}

int main(int argc, char** argv)
{
    unsigned int a, b;
    const int limit = 800;
    initI2C();
    LCDInit();
    PWM_Init();
    __delay_ms(30);
    OC2R = 1500;
    TRISBbits.TRISB4 = 0;
    TRISFbits.TRISF1 = 0;
    LATBbits.LATB4 = 1;
    LATFbits.LATF1 = 0;
    
    while(1){
        Adc_init(7);
        a = read_Adc();
        Adc_init(8);
        b = read_Adc();
        if (a){
            LCDPrintInt(a);
            __delay_ms(500);
            LCDSend(LCD_ADRESS, LCD_CLEAR, COMMAND);
            if (a < limit) OC2R = limit;
            else OC2R = a;
            
        }
        if (b > 2000) {
                LATBbits.LATB4 = 0;
                LATFbits.LATF1 = 1;
            }
        if (b < 2000) {
                LATBbits.LATB4 = 1;
                LATFbits.LATF1 = 0;
            }
    }
    return(EXIT_SUCCESS);
}
