/*
 *    AT+GMR                    (Check Firmware version)
 *    AT+CWLAP                  (Scan Wifi)
 *    AT+CWJAP="SSID","pass"    (Connect)
 *    AT+CWQAP                  (Disconnect)
 *    AT+CIFSR                  (/ipconfig)
 *    AT+CIPMUX=1               (Multiple connections)
 *    AT+CIPSERVER=1,80         (Server)
 *    Print something on serial: //Serial.println("  ");
 *    
 *    preconfigurada la red, la IP y el serial a 9600
 *    AT+CWJAP="bilbaodynamics","contraseña" 
 *    AT+CIPSTA="192.168.10.112"
 *    
 *    el multicon y server mode se ha de hacer con cada reinicio
*/
#include <PinchyWifi.h>
#include <PinchyMatrix.h>
#include <Servo.h>

PinchyWifi Wifi;
PinchyMatrix pm = PinchyMatrix(16, 15, 17);

char r;
String R;
int i =0;

void setup()  {
  ////Serial.begin(9600);
  
  pinMode(2, OUTPUT);
  pinMode( 9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  pm.shutdown(false);
  pm.setIntensity(8);

  digitalWrite(11, HIGH);
  delay(15000);
  Serial1.begin(9600);
  delay(500);
  Serial1.print("AT+CIPMUX=1\r\n");
  delay(500);
  while (Serial1.available() ) { Serial1.read(); }
  Serial1.print("AT+CIPSERVER=1,80\r\n");
  delay(500);
  while (Serial1.available() ) { Serial1.read(); }
  digitalWrite(11, LOW);
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
}

void loop() {
  
  delay(10);
  i++;
  if(i==100*30){
    Serial1.print("AT+CIPMUX=1\r\n");
     delay(500);
    Serial1.print("AT+CIPSERVER=1,80\r\n");
     delay(500);
     i=0;
  }

  
  while (Serial1.available() ) {

    r = Serial1.read();
    //Serial.write(r);
    R = R+r;
    
  } 


/*
  while (//Serial.available() ) {
    Serial1.write(//Serial.read() );
  }
*/

  if (r == '\n' && R.indexOf("GET /") > 0) {
    int start = R.indexOf("/");
    int finish = R.indexOf("HTTP");
    String command = R.substring(start + 1, finish - 1);

    commands(command);
    R = "";
  }

  else if (r == '\n'){
    R = "";
  }

}

//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************


void commands(String command) {

  ////Serial.println(command);
  
  if (command.startsWith("LED?color=")) {
    if (command.startsWith("LED?color=red")){
      digitalWrite(11, HIGH );
      digitalWrite(9, LOW );
      digitalWrite(10, LOW );
    }

    else if (command.startsWith("LED?color=green")){
      digitalWrite(11, LOW );
      digitalWrite(9, HIGH );
      digitalWrite(10, LOW );
    }

    else if (command.startsWith("LED?color=blue")){
      digitalWrite(11, LOW );
      digitalWrite(9, LOW );
      digitalWrite(10, HIGH );
    }

   else if (command.startsWith("LED?color=off")){
      digitalWrite(11, LOW );
      digitalWrite(9, LOW );
      digitalWrite(10, LOW );
    }
  }

  else if (command.startsWith("BUZZER?sound=")) {
    if (command.startsWith("BUZZER?sound=ON"))
      digitalWrite(2, HIGH );

    else if (command.startsWith("BUZZER?sound=OFF"))
      digitalWrite(2, LOW );
  }

  else if (command.startsWith("LCD?Number=")) {
    String n = command.substring(11);
    int num = n.toInt();
    pm.number(num);
  }
  
  else if (command.startsWith("MATRIX")) {
    
    ////Serial.println(command.substring(6,6+35));
    
    if(command.substring(6).length() >= 35){
      ////Serial.println("writingg");
      pm.binary( command.substring(6,6+35) );
    }
    else{
      pm.sad();
    }
  }
    Serial1.println("AT+CIPCLOSE=0");


  
}


