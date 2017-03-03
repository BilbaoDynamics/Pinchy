#include <PinchyMatrix.h>
#include <Ultrasonic.h>

PinchyMatrix pm = PinchyMatrix(16,15,17);
Ultrasonic u = Ultrasonic(7,8);

void setup() {
  // put your setup code here, to run once:
  pm.shutdown(false);    //
  pm.setIntensity(8);    //Intensidad de los led en la matriz #1 (valor entre 0 y 15)
  
}

void loop() {
  // put your main code here, to run repeatedly:
  pm.number(u.Ranging(CM));
  delay(200);
}
