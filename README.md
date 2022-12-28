## OVERVIEW
This project interfaces esp8266 WIFI module and three HC-SR04 ultrasonic sensors with PIC16F877A microcontroller. The processed data are displayed on an LCD and logged on a google sheet using pushingbox API generated HTTP requests.

**User Guide:**
1) Create a new google sheet
2) Deploy a google Apps script for the generated sheet by copying the attached script.gs file and modifying the spreadsheet ID with your sheet ID. You can add other modifications to the script ex: number of columns, date and time format, title of the columns,...etc
3) Using the web app URL generated in 2, Create a scenario on PushingBox, add an action, choose the service to use and write the text to send
4) Modify esp8266_send_api() function in the header file with your scenario unique identifier i.e. DeviceID or "devid", and the data to be sent. Then use it to run the right scenario.

ex:
```
_esp8266_print("GET /pushingbox?devid=vD37CDB3DB961DB1&short=0&mid=1&long=0 HTTP/1.1\r\nHost: api.pushingbox.com\r\nConnection: keep-alive\r\n\r\n");
```
  
  * This line sends 1 in mid column and 0 in short and long columns to the scenario "vD37CDB3DB961DB1"
  * In my scenario im sending 122 bytes/request. you will need to recalculate this for your scenario as well

**Note: The project was built on the work of Aswinth-raj/PIC_ESP8266-Sending-mail project**
@misc{github,
  author={Aswinth-raj},
  title={PIC_ESP8266-Sending-mail},
  year={2017},
  url={https://github.com/Aswinth-raj/PIC_ESP8266-Sending-mail},
}

P.S. You can find multilple tutorials that walk you through steps 2 and 3 online if needed.
