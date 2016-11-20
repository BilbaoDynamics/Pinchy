#include <SoftwareSerial.h>
SoftwareSerial ESPserial(3, 4); // RX | TX
 
void setup() 
{
    Serial.begin(9600);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(115200);  
 
  
}
 
void loop() 
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
    while ( ESPserial.available() )   {
      Serial.write( ESPserial.read() );  
    }
 
    // listen for user input and send it to the ESP8266
    while ( Serial.available() )       {  ESPserial.write( Serial.read() );  }
}
