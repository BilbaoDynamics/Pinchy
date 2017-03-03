#include <PinchyMatrix.h>
#include <Servo.h>

#define GREENLED 9
#define BLUELED 10
#define REDLED 11

Servo servo1;
Servo servo2;
Servo pinza;

PinchyMatrix pm = PinchyMatrix(16, 15, 17);

void setup() {
  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);

  servo1.attach(5);
  servo1.write(90);
  servo2.attach(6);
  servo2.write(90);
  pinza.attach(3);
  
  pm.shutdown(false);
  pm.setIntensity(8);
}

void loop(){
  pm.kitten();
  int x = random(0, 9);

  if(x > 2){
    colors(x);
    delay(2000);
  }
  else{
    pinza.write(50);
    colors(x);
    delay(2000);
    pinza.write(100);
  }
}

////////////////////////////////////////////////////////////////////////
void colors(int x){
  switch(x){
    case 0:
      ledAzul2();
      break;
    case 1:
      ledRojo();
      break;
    case 2:
      ledAzul();
      break;
    case 3:
      ledVerde();
      break;
    case 4:
      ledMorado();
      break;
    case 5:
      ledAmarillo();
      break;
    case 6:
      ledBlanco();
      break;
    case 7:
      ledAzul2();
      break;
    case 8:
      ledVerde();
      break;
    case 9:
      ledRojo();
      break;
      
  }
}
void ledRojo(){
  digitalWrite(BLUELED, LOW);
  digitalWrite(REDLED, HIGH);
  digitalWrite(GREENLED, LOW);
}
void ledAzul(){
  digitalWrite(BLUELED, HIGH);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);
}
void ledVerde(){
  digitalWrite(BLUELED, LOW);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, HIGH);
}
void ledMorado(){
  digitalWrite(BLUELED, HIGH);
  digitalWrite(REDLED, HIGH);
  digitalWrite(GREENLED, LOW);
}
void ledAmarillo(){
  digitalWrite(BLUELED, LOW);
  digitalWrite(REDLED, HIGH);
  digitalWrite(GREENLED, HIGH);
}
void ledBlanco(){
  digitalWrite(BLUELED, HIGH);
  digitalWrite(REDLED, HIGH);
  digitalWrite(GREENLED, HIGH);
}
void ledAzul2(){
  digitalWrite(BLUELED,  HIGH);
  digitalWrite(REDLED ,  LOW);
  digitalWrite(GREENLED, HIGH);
}

