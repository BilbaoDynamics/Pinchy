/*

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
  pm.setIntensity(14);

  digitalWrite(11, HIGH);
  delay(1000);
  Serial.begin(9600);

}

void loop() {
  
  while (Serial.available() ) {

    r = Serial.read();
    R = R+r;
    
  } 
  if (r == '\n'){
    commands(R);
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


  
}


