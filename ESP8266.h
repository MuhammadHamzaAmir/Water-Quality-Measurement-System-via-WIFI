#ifndef ESP8266_H_
#define ESP8266_H_
#define F_CPU 16000000UL /* Define CPU Frequency e.g. here its \
Ext. 16MHz */
#include <avr/io.h>
#include <stdbool.h>
#define SREG _SFR_IO8(0x3F)
#define DEFAULT_BUFFER_SIZE 160
#define DEFAULT_TIMEOUT 10000
/* Connection Mode */
#define SINGLE 0
#define MULTIPLE 1
/* Application Mode */
#define NORMAL 0
#define TRANSPERANT 1
/* Application Mode */
#define STATION 1
#define ACCESSPOINT 2
#define BOTH_STATION_AND_ACCESPOINT 3
/* Select Demo */
#define RECEIVE_DEMO /* Define RECEIVE demo */
#define SEND_DEMO    /* Define SEND demo */
/* Define Required fields shown below */
#define DOMAIN "api.thingspeak.com"
#define PORT "80"
#define API_WRITE_KEY "C7JFHZY54GLCJY38"
#define CHANNEL_ID "119922"
#define SSID "ssid"
#define PASSWORD "password"
void Read_Response(char *_Expected_Response); //Read the response of server
void ESP8266_Clear();                         //Clear the Buffer Size
void Start_Read_Response(char *_ExpectedResponse);                                //Analyze the Response
received by server void GetResponseBody(char *Response, uint16_t ResponseLength); //Receive the
response of server bool WaitForExpectedResponse(char *ExpectedResponse);          //Wait for to server to
respond bool SendATandExpectResponse(char *ATCommand, char *ExpectedResponse);
//Send data and wait for response
bool ESP8266_ApplicationMode(uint8_t Mode);                    //ESP8266 WIFI activates the
application mode bool ESP8266_ConnectionMode(uint8_t Mode);    //ESP8266 WIFI set the connection
mode bool ESP8266_Begin();                                     //ESP8266 WIFI begins the connection
bool ESP8266_Close();                                          //ESP8266 WIFI closes the connection
bool ESP8266_WIFIMode(uint8_t _mode);                          //ESP8266 WIFI begins the connection
uint8_t ESP8266_JoinAccessPoint(char *_SSID, char *_PASSWORD); //ESP8266 WIFI
joins or connects to the wifi connection
             uint8_t
             ESP8266_connected(); //ESP8266 WIFI tells whether it is connected to a wifi
or not uint8_t ESP8266_Start(uint8_t _ConnectionNumber, char *Domain, char *Port);
//ESP8266 WIFI starts a connection to the cloud server
uint8_t ESP8266_Send(char *Data);  //ESP8266 WIFI sends the data
int16_t ESP8266_DataAvailable();   //ESP8266 WIFI clears the send buffer
uint8_t ESP8266_DataRead();        //ESP8266 WIFI receives the data
uint16_t Read_Data(char *_buffer); //ESP8266 WIFI clears the receive buffer
#endif                             /* ESP8266_H_ */