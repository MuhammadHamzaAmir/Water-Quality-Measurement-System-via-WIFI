#define F_CPU 16000000L
#define RES2 820.0
#define ECREF 200.0
#define BUAD 9600
#define BRC ((F_CPU / 16 / BUAD) - 1)
/*Just eun the main.c wheres the other files in the same folder*/
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdfix.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "ds18b20.h"
#include "ESP8266.h"
char Temperature[10];
double temperature_value;
char PH[10];
double ph_value;
char Turbidity[10];
double turbidity_value;
char Conductivity[10];
double conductivity_value;
char TDS[10];
double tds_value;
int main(void)
{
    DDRB = 0xff;
    DDRD = 0x03;
    DDRE = 0x40;
    char _buffer[150];
    uint8_t Connect_Status;
#ifdef SEND_DEMO
    uint8_t Sample = 0;
#endif
    lcd_init();         // LCD initialization
    ADC_init();         //Analog to Digital Conversion initialization
    USART_Init(115200); /* Initiate USART with
115200 baud rate */
    sei();              /* Start global
interrupt */
    //ESP8266 connects to the wifi and the cloud server
    while (!ESP8266_Begin())
        ;
    ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT); /* 3 = Both (AP and
STA) */
    ESP8266_ConnectionMode(SINGLE);                /* 0 = Single; 1 = Multi */
    ESP8266_ApplicationMode(NORMAL);               /* 0 = Normal Mode; 1 =
Transperant Mode */
    if (ESP8266_connected() == 3)
        ESP8266_JoinAccessPoint(SSID, PASSWORD);
    ESP8266_Start(0, DOMAIN, PORT);
    cli();
    while (1)
    {
        //Values from sensors are collected
        Temperature_Function();
        PH_Function();
        Turbidity_Function();
        Conductivity_Function();
        TDS_Function();
        _delay_ms(100);
        sei();
    //Connection made to server
        Connect_Status = ESP8266_connected();
        if (Connect_Status == 3)
            ESP8266_JoinAccessPoint(SSID, PASSWORD);
        if (Connect_Status == 2)
            ESP8266_Start(0, DOMAIN, PORT);
//Data is send to the cloud server
#ifdef SEND_DEMO
        ESP8266_Send(Temperature);
        ESP8266_Send(Turbidity);
        ESP8266_Send(TDS);
        ESP8266_Send(Conductivity);
        ESP8266_Send(PH);
        _delay_ms(15000); /* Cloud server delay */
#endif
//Result is received from the cloud server
#ifdef RECEIVE_DEMO
        memset(_buffer, 0, 150);
        sprintf(_buffer, "GET /channels/%s/feeds/last.txt", CHANNEL_ID);
        ESP8266_Send(_buffer);
        Read_Data(_buffer);
        _delay_ms(700);
#endif
        //Water Quality Index printed on LCD
        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_str("Water Quality Index");
        lcd_cmd(0xc0);
        lcd_str(_buffer);
        _delay_ms(500);
    }
}
//for sending commands to LCD
void lcd_cmd(unsigned char cmd)
{
    PORTB = cmd;      //for sending data and commands
    PORTD &= ~(0x01); //for controlling LCD with EN RS RW pins for commands clear RS pin for sending command
    PORTD &=~(0x02); //for RW pin
    PORTE |= (0x40); // set Enable
    _delay_ms(10);
    PORTE &= ~(0x40); //CLear Enable
}
//for sending data to LCD
void lcd_data(unsigned char data)
{
    PORTB = data;    //for sending data and commands
    PORTD |= (0x01); //for controlling LCD with EN RS RW pins for commands set RS pin for sending data
    PORTD &=~(0x02); //for RW pin
    PORTE |= (0x40); // set Enable
    _delay_ms(10);
    PORTE &= ~(0x40); //CLear Enable
}
// LCD initialization
void lcd_init()
{
    lcd_cmd(0x38); //8 bit mode
    lcd_cmd(0x0e); //enable cursor
    lcd_cmd(0x06); //cursor increment
    lcd_cmd(0x01); //clear LCD
    lcd_cmd(0x80); // select 1st row 1col for data
}
// Combining Char characters to form a string
void lcd_str(char *str)
{
    while (*str)
    {
        lcd_data(*str++);
    }
}
//Analog to Digital Conversion initialization
void ADC_init()
{
    DDRF = 0x00;   /* Make ADC port as input */
    ADCSRA = 0x87; /* Enable ADC, with freq/128 */
    ADMUX = 0x40;
}
//Analog to Digital Conversion Start
int ADC_Read(int channel)
{
    ADMUX = 0x40 | (channel & 0x07); /* set input channel to read */
    ADCSRA |= (1 << ADSC);           /* Start ADC conversion */
    while (!(ADCSRA & (1 << ADIF)))
        ;
    /* Wait until end of conversion by polling ADC
    interrupt flag */
    ADCSRA |= (1 << ADIF); /* Clear interrupt flag */
    _delay_ms(1);          /* Wait a little bit */
    return ADCW;           /* Return ADC word */
}
/*A function to read the sensor and calculating the PH
and sending it the LCD display*/
void PH_Function()
{
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("PH:");
    ph_value = (ADC_Read(0) * 5);
    ph_value = ph_value / 60.3;
    dtostrf(ph_value, 3, 2, PH);
    lcd_cmd(0xc0);
    lcd_str(PH);
    memset(PH, 0, 10);
    _delay_ms(100);
}
/*A function to read the sensor and calculating the Turbidity
and sending it the LCD display*/
void Turbidity_Function()
{
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("Turbidity (UNIT:NTU)");
    turbidity_value = (ADC_Read(1) * 5);
    turbidity_value = turbidity_value / 1023;
    turbidity_value = (-1120.4 * (pow(turbidity_value, turbidity_value))) + 5742.3;
    dtostrf(turbidity_value, 3, 2, Turbidity);
    lcd_cmd(0xc0);
    lcd_str(Turbidity);
    memset(Turbidity, 0, 10);
    _delay_ms(100);
}
/*A function to read the sensor and calculating the Conductivity
and sending it the LCD display*/
void Conductivity_Function()
{
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("Conductivity (UNIT:S/m)");
    conductivity_value = (ADC_Read(4) * 5);
    conductivity_value = conductivity_value / 1023;
    conductivity_value = 1000 * conductivity_value / RES2 / ECREF;
    conductivity_value = conductivity_value / (1 + 0.0185 * (temperature_value - 25.0));
    dtostrf(conductivity_value, 3, 2, Conductivity);
    lcd_cmd(0xc0);
    lcd_str(Conductivity);
    memset(Conductivity, 0, 10);
    _delay_ms(100);
}
/*A function to calculate the Total Dissolved Solids
and sending it the LCD display*/
void TDS_Function()
{
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("Total Dissolved Solids (UNIT:mg/l)");
    tds_value = conductivity_value * 0.67 * 10;
    dtostrf(tds_value, 3, 2, TDS);
    lcd_cmd(0xc0);
    lcd_str(TDS);
    memset(TDS, 0, 10);
    _delay_ms(100);
}
/*A function to read the sensor and calculating the Temperature
and sending it the LCD display*/
void Temperature_Function()
{
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("Temperature (UNIT:C)");
    temperature_value = ds18b20_gettemp();
    dtostrf(temperature_value, 3, 2, Temperature);
    lcd_cmd(0xc0);
    lcd_str(Temperature);
    memset(Temperature, 0, 10);
    _delay_ms(100);
}

