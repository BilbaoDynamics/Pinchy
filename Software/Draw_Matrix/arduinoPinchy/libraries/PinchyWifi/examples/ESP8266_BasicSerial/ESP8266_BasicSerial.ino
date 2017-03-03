/*
 *    AT+GMR                    (Check Firmware version)
 *    AT+CWLAP                  (Scan Wifi)
 *    AT+CWJAP="SSID","pass"    (Connect)
 *    AT+CWQAP                  (Disconnect)
 *    AT+CIFSR                  (/ipconfig)
 *    AT+CIPMUX=1               (Multiple connections)
 *    AT+CIPSERVER=1,80         (Server)
 *    Print something on serial: Serial.println("  ");
*/
#include <PinchyWifi.h>

PinchyWifi Wifi;
PinchyMatrix pm = PinchyMatrix(16,15,17);

void setup()  {
  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop(){
   while(Serial1.available() ) {
      Serial.print(Serial1.read() );
   }
 
  while(Serial.available() ) {
     Serial1.write(Serial.read() );
  }
  
}

