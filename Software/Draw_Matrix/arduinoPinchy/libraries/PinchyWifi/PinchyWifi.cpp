#include "Arduino.h"
#include "PinchyWifi.h"

void PinchyWifi::http(String output){
  Serial1.print("AT+CIPSEND=0,");
  Serial1.println(output.length());
  if (Serial1.find(">")){
    Serial1.println(output);
    delay(10);
    while ( Serial1.available() > 0 ){
      if (  Serial1.find("SEND OK") )
        break;
    }
  }
}
void PinchyWifi::webServer(){
http("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

  delay(100);
  Serial1.println("AT+CIPCLOSE=0");

}
