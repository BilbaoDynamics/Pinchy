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
#include <PinchyMatrix.h>
#include <Servo.h>

PinchyWifi Wifi;
PinchyMatrix pm = PinchyMatrix(16, 15, 17);

char r;
String R;
boolean b = false;

Servo rueda1;
Servo rueda2;

void setup()  {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(2, OUTPUT);
  rueda1.attach(5);
  rueda1.write(90);
  rueda2.attach(6);
  rueda2.write(90);
  pinMode( 9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  pm.shutdown(false);
  pm.setIntensity(8);
  //Setup for the servers and multiple connections.
  digitalWrite(11, HIGH);
  delay(3000);
  Serial1.println("AT+CIPMUX=1");
  delay(500);
  Serial1.println("AT+CIPSERVER=1,80");
  delay(500);
  digitalWrite(11, LOW);
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
}

void loop() {
  while (Serial1.available() ) {
    r = Serial1.read();
    if (r == 71)
      b = true;

    Serial.print(r);
    R = R + r;

    if (r == '\n')
      break;
  }

  while (Serial.available() ) {
    Serial1.write(Serial.read() );
  }

  if (r == '\n' && R.indexOf("GET /") > 0) {
    int start = R.indexOf("/");
    int finish = R.indexOf("HTTP");
    String command = R.substring(start + 1, finish - 1);

    commands(command);
    R = "";
  }

  else if (r == '\n')
    R = "";

  if (b) {
    Wifi.webServer();
    b = false;
  }

}

//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
void commands(String command) {
  if (command.startsWith("LED?color=")) {
    if (command.equals("LED?color=red"))
      digitalWrite(11, !digitalRead(11) );

    else if (command.equals("LED?color=green"))
      digitalWrite(9, !digitalRead(9) );

    else if (command.equals("LED?color=blue"))
      digitalWrite(10, !digitalRead(10) );
  }

  else if (command.startsWith("BUZZER?sound=")) {
    if (command.equals("BUZZER?sound=ON"))
      digitalWrite(2, HIGH );

    else if (command.equals("BUZZER?sound=OFF"))
      digitalWrite(2, LOW );
  }

  else if (command.startsWith("LCD?Number=")) {
    String n = command.substring(11);
    int num = n.toInt();
    pm.number(num);
  }

  else if (command.startsWith("ADELANTE?seg=")) {
    String z = command.substring(13);
    int sec = z.toInt();
    rueda1.write(0);
    rueda2.write(180);
    delay(sec * 1000);
    rueda1.write(90);
    rueda2.write(90);
  }

  else if (command.startsWith("MATRIX")) {
    if(command.substring(6).length() == 35)
      pm.binary( command.substring(6) );
    else{
      pm.sad();
    }
  }

  
}


