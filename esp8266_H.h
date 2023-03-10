
#ifndef esp8266_H
#define esp8266_H



#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define ESP8266_STATION 0x01
#define ESP8266_SOFTAP 0x02

#define ESP8266_TCP 1
#define ESP8266_UDP 0

#define ESP8266_OK 1
#define ESP8266_READY 2
#define ESP8266_FAIL 3
#define ESP8266_NOCHANGE 4
#define ESP8266_LINKED 5
#define ESP8266_UNLINK 6
#define ESP8266_ERROR 7

int len;


///***______________________ESP module Function Declarations__________________**///
void _esp8266_putch(unsigned char);
unsigned char _esp8266_getch(void);
/** Function prototypes **/
bit esp8266_isStarted(void);        // Check if the module is started (AT)
bit esp8266_restart(void);          // Restart module (AT+RST)
void esp8266_echoCmds(bool);        // Enabled/disable command echoing (ATE)
// WIFI Mode (station/softAP/station+softAP) (AT+CWMODE)
void esp8266_mode(unsigned char);
// Connect to AP (AT+CWJAP)
void esp8266_connect(unsigned char*, unsigned char*);
// Disconnect from AP (AT+CWQAP)
void esp8266_disconnect(void);

// Create connection (AT+CIPSTART)
bit esp8266_start(unsigned char protocol, char* ip, unsigned char port);
// Send data (AT+CIPSEND)
bit esp8266_send(unsigned char*);
// Receive data (+IPD)
void esp8266_receive(unsigned char*, uint16_t, bool);
/** Functions for internal use only **/
// Print a string to the output
void _esp8266_print(unsigned const char *);
// Wait for a certain string on the input
inline uint16_t _esp8266_waitFor(unsigned char *);
// Wait for any response on the input
inline unsigned char _esp8266_waitResponse(void);
void Lcd_Set_Cursor(char , char b);
void Lcd_Print_Char(char);
void Lcd_Print_String(char *);
//********__________________End of Function Declaration_________________********///

//***Initialize UART for ESP8266**//
void Initialize_ESP8266(void)
{
    //****Setting I/O pins for UART****//
    TRISC6 = 0; // TX Pin set as output
    TRISC7 = 1; // RX Pin set as input
    //________I/O pins set __________//
    
    /**Initialize SPBRG register for required 
    baud rate and set BRGH for fast baud_rate**/
    SPBRG = 25;
    BRGH  = 1;  // for high baud_rate
    //_________End of baud_rate setting_________//
    
    //****Enable Asynchronous serial port*******//
    SYNC  = 0;    // Asynchronous
    SPEN  = 1;    // Enable serial port pins
    //_____Asynchronous serial port enabled_______//
    //**Lets prepare for transmission & reception**//
    TXEN  = 1;    // enable transmission
    CREN  = 1;    // enable reception
    //__UART module up and ready for transmission and reception__//
    
    //**Select 8-bit mode**//  
    TX9   = 0;    // 8-bit reception selected
    RX9   = 0;    // 8-bit reception mode selected
    //__8-bit mode selected__//     
}
//________UART module Initialized__________//



 
//**Function to send one byte of date to UART**//
void _esp8266_putch(char bt)  
{
    while(!TXIF);  // hold the program till TX buffer is free
    TXREG = bt; //Load the transmitter buffer with the received value
}
//_____________End of function________________//


 
//**Function to get one byte of date from UART**//
char _esp8266_getch()   
{
    if(OERR) // check for Error 
    {
        CREN = 0; //If error -> Reset 
        CREN = 1; //If error -> Reset 
    }
    
    while(!RCIF);  // hold the program till RX buffer is free
    
    return RCREG; //receive the value and send it to main function
}
//_____________End of function________________//
 



//**Function to convert string to byte**//
void ESP8266_send_string(char* st_pt)
{
    while(*st_pt) //if there is a char
        _esp8266_putch(*st_pt++); //process it as a byte data
}
//___________End of function______________//
//**End of modified Codes**//



//**Function to configure soft_AP**//
unsigned char esp8266_config_softAP(unsigned char* softssid, unsigned char* softpass) {
    _esp8266_print("AT+CWSAP=\"");
    _esp8266_print(softssid);
    _esp8266_print("\",\"");
    _esp8266_print(softpass);
    _esp8266_print("\",5,3\r\n");
    return _esp8266_waitResponse();
}
//___________End of function______________//


 
 //**Function to enable multiple connections**//
 void _esp8266_enale_MUX()
 {
 _esp8266_print("AT+CIPMUX=1\r\n"); //Enable Multiple Connections
 _esp8266_waitResponse();
 }
 void _esp8266_enale_MODE()
 {
 _esp8266_print("AT+CIPMODE=0\r\n"); //Enable Multiple Connections
 _esp8266_waitResponse();
 }
  //___________End of function______________//
 
 
  //**Function to create server on Port 80**//
 void _esp8266_create_server()
 {
    _esp8266_print("AT+CIPSERVER=1,80\r\n"); //Enable Server on port 80
    _esp8266_waitResponse(); 
 }
  //___________End of function______________//


 /*Connect to SMPT2GO server*/
 void _esp8266_connect_api()
 {
    _esp8266_print("AT+CIPSTART=4,\"TCP\",\"api.pushingbox.com\",80\r\n");
    _esp8266_waitResponse();
 }
 
 void _esp8266_send_api(int i){
     
    _esp8266_print("AT+CIPSEND=4,122\r\n");
    if(_esp8266_waitResponse()== ESP8266_ERROR){
        _esp8266_print("AT+CIPSTART=4,\"TCP\",\"api.pushingbox.com\",80\r\n");
    _esp8266_waitResponse();
    }
    if(i == 1){
        _esp8266_print("GET /pushingbox?devid=vD37CDB3DB961DB1&short=0&mid=1&long=0 HTTP/1.1\r\nHost: api.pushingbox.com\r\nConnection: keep-alive\r\n\r\n");
        _esp8266_waitResponse();
    }else if(i == 2){
        _esp8266_print("GET /pushingbox?devid=vD37CDB3DB961DB1&short=0&mid=0&long=1 HTTP/1.1\r\nHost: api.pushingbox.com\r\nConnection: keep-alive\r\n\r\n");
        _esp8266_waitResponse();
    }else{
        _esp8266_print("GET /pushingbox?devid=vD37CDB3DB961DB1&short=1&mid=0&long=0 HTTP/1.1\r\nHost: api.pushingbox.com\r\nConnection: keep-alive\r\n\r\n");
        _esp8266_waitResponse();
    }
 }
         
    /*connected to Server*/
/**
 * Check if the module is started
 *
 * This sends the `AT` command to the ESP and waits until it gets a response.
 *
 * @return true if the module is started, false if something went wrong
 */
bit esp8266_isStarted(void) {
    _esp8266_print("AT\r\n");
    return (_esp8266_waitResponse() == ESP8266_OK);
}

/**
 * Restart the module
 *
 * This sends the `AT+RST` command to the ESP and waits until there is a
 * response.
 *
 * @return true iff the module restarted properly
 */
bit esp8266_restart(void) {
    _esp8266_print("AT+RST\r\n");
    if (_esp8266_waitResponse() != ESP8266_OK) {
        return false;
    }
    return (_esp8266_waitResponse() == ESP8266_READY);
}

/**
 * Enable / disable command echoing.
 *
 * Enabling this is useful for debugging: one could sniff the TX line from the
 * ESP8266 with his computer and thus receive both commands and responses.
 *
 * This sends the ATE command to the ESP module.
 *
 * @param echo whether to enable command echoing or not
 */
void esp8266_echoCmds(bool echo) {
    _esp8266_print("ATE");
    if (echo) {
        _esp8266_putch('1');
    } else {
        _esp8266_putch('0');
    }
    _esp8266_print("\r\n");
    _esp8266_waitFor("OK");
}

void esp8266_mode(unsigned char mode) {
    _esp8266_print("AT+CWMODE=");
    _esp8266_putch(mode + '0');
    _esp8266_print("\r\n");
    _esp8266_waitResponse();
}

/**
 * Connect to an access point.
 *
 * This sends the AT+CWJAP command to the ESP module.
 *
 * @param ssid The SSID to connect to
 * @param pass The password of the network
 * @return an ESP status code, normally either ESP8266_OK or ESP8266_FAIL
 */
void esp8266_connect(unsigned char* ssid, unsigned char* pass) {
    _esp8266_print("AT+CWJAP=\"");
    _esp8266_print(ssid);
    _esp8266_print("\",\"");
    _esp8266_print(pass);
    _esp8266_print("\"\r\n");
    _esp8266_waitResponse();
}

/**
 * Disconnect from the access point.
 *
 * This sends the AT+CWQAP command to the ESP module.
 */
void esp8266_disconnect(void) {
    _esp8266_print("AT+CWQAP\r\n");
    _esp8266_waitFor("OK");
}


/**
 * Open a TCP or UDP connection.
 *
 * This sends the AT+CIPSTART command to the ESP module.
 *
 * @param protocol Either ESP8266_TCP or ESP8266_UDP
 * @param ip The IP or hostname to connect to; as a string
 * @param port The port to connect to
 *
 * @return true iff the connection is opened after this.
 */
bit esp8266_start(unsigned char protocol, char* ip, unsigned char port) {
    _esp8266_print("AT+CIPSTART=\"");
    if (protocol == ESP8266_TCP) {
        _esp8266_print("TCP");
    } else {
        _esp8266_print("UDP");
    }
    _esp8266_print("\",\"");
    _esp8266_print(ip);
    _esp8266_print("\",");
    unsigned char port_str[5] = "\0\0\0\0";
    sprintf(port_str, "%u", port);
    _esp8266_print(port_str);
    _esp8266_print("\r\n");
    if (_esp8266_waitResponse() != ESP8266_OK) {
        return 0;
    }
    if (_esp8266_waitResponse() != ESP8266_LINKED) {
        return 0;
    }
    return 1;
}

// Send data (AT+CIPSEND)
/**
 * Send data over a connection.
 *
 * This sends the AT+CIPSEND command to the ESP module.
 *
 * @param data The data to send
 *
 * @return true iff the data was sent correctly.
 */
bit esp8266_send(unsigned char* data) {
    unsigned char length_str[6] = "\0\0\0\0\0";
    sprintf(length_str, "%u", strlen(data));
    _esp8266_print("AT+CIPSEND=");
    _esp8266_print(length_str);
    _esp8266_print("\r\n");
    while (_esp8266_getch() != '>');
    _esp8266_print(data);
    if (_esp8266_waitResponse() == ESP8266_OK) {
        return 1;
    }
    return 0;
}

/**
 * Read a string of data that is sent to the ESP8266.
 *
 * This waits for a +IPD line from the module. If more bytes than the maximum
 * are received, the remaining bytes will be discarded.
 *
 * @param store_in a pointer to a character array to store the data in
 * @param max_length maximum amount of bytes to read in
 * @param discard_headers if set to true, we will skip until the first \r\n\r\n,
 * for HTTP this means skipping the headers.
 */
void esp8266_receive(unsigned char* store_in, uint16_t max_length, bool discard_headers) {
    _esp8266_waitFor("+IPD,");
    uint16_t length = 0;
    unsigned char received = _esp8266_getch();
    do {
        length = length * 10 + received - '0';
        received = _esp8266_getch();
    } while (received >= '0' && received <= '9');

    if (discard_headers) {
        length -= _esp8266_waitFor("\r\n\r\n");
    }

    if (length < max_length) {
        max_length = length;
    }

    /*sprintf(store_in, "%u,%u:%c%c", length, max_length, _esp8266_getch(), _esp8266_getch());
    return;*/

    uint16_t i;
    for (i = 0; i < max_length; i++) {
        store_in[i] = _esp8266_getch();
    }
    store_in[i] = 0;
    for (; i < length; i++) {
        _esp8266_getch();
    }
    _esp8266_waitFor("OK");
}

/**
 * Output a string to the ESP module.
 *
 * This is a function for internal use only.
 *
 * @param ptr A pointer to the string to send.
 */
void _esp8266_print(unsigned const char *ptr) {
    while (*ptr != 0) {
        _esp8266_putch(*ptr++);
    }
}

/**
 * Wait until we found a string on the input.
 *
 * Careful: this will read everything until that string (even if it's never
 * found). You may lose important data.
 *
 * @param string
 *
 * @return the number of characters read
 */
inline uint16_t _esp8266_waitFor(unsigned char *string) {
    unsigned char so_far = 0;
    unsigned char received;
    uint16_t counter = 0;
    do {
        received = _esp8266_getch();
        counter++;
        if (received == string[so_far]) {
            so_far++;
        } else {
            so_far = 0;
        }
    } while (string[so_far] != 0);
    return counter;
}

/**
 * Wait until we received the ESP is done and sends its response.
 *
 * This is a function for internal use only.
 *
 * Currently the following responses are implemented:
 *  * OK
 *  * ready
 *  * FAIL
 *  * no change
 *  * Linked
 *  * Unlink
 *
 * Not implemented yet:
 *  * DNS fail (or something like that)
 *
 * @return a constant from esp8266.h describing the status response.
 */
inline unsigned char _esp8266_waitResponse(void) {
    unsigned char so_far[7] = {0,0,0,0,0,0,0};
    unsigned const char lengths[7] = {2,5,4,9,6,6,5};
    unsigned const char* strings[7] = {"OK", "ready", "FAIL", "no change", "Linked", "Unlink","ERROR"};
    unsigned const char responses[7] = {ESP8266_OK, ESP8266_READY, ESP8266_FAIL, ESP8266_NOCHANGE, ESP8266_LINKED, ESP8266_UNLINK, ESP8266_ERROR};
    unsigned char received;
    unsigned char response;
    bool continue_loop = true;
    while (continue_loop) {
        received = _esp8266_getch();
        for (unsigned char i = 0; i < 6; i++) {
            if (strings[i][so_far[i]] == received) {
                so_far[i]++;
                if (so_far[i] == lengths[i]) {
                    response = responses[i];
                    continue_loop = false;
                }
            } else {
                so_far[i] = 0;
            }
        }
    }
    return response;
}

#endif
