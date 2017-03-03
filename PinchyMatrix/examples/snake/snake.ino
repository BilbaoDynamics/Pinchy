#include <PinchyMatrix.h>

#define BUZZER 2
#define snakeSpeed 200  //cambia este número para aumentar/disminuir la velocidad del juego

PinchyMatrix pm = PinchyMatrix(16,15,17);

int snake[35][2];//35 puntos tiene la matriz, 2 puntos para identificar la posicion.
int snakeLength = 1;
boolean playing = true;
int fruit[2], fruits;
byte mov = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  pm.shutdown(false);    //
  pm.setIntensity(8);    //Intensidad de los led en la matriz #1 (valor entre 0 y 15)
  
  playing=true;
  snake[0][0]=2;
  snake[0][1]=5;
  newFruit();
  fruits = 0;
  draw();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while(playing)
  {
    readActionTec();
    delay(snakeSpeed);
    makeMov();
    draw();
    testDie();
  }
  pm.number(fruits);
}

void readActionTec()
{
  char tmpChar;
  if(Serial.available())
  {
    tmpChar = Serial.read();
    if (tmpChar == 'a')
    {
      mov+=3;
      mov%=4;
    }
    if (tmpChar == 'd')
    {
      mov++;
      mov%=4;
    }
  }
}

void readAction()
{
  //Proximamente para jugar con un par de botones
  
}

void newFruit()
{
  fruit[0] = random(0,5);
  fruit[1] = random(0,7);
  fruits++;
}

void makeMov()
{
  for(int i=snakeLength; i>0; i--)
  {
    snake[i][0] = snake[i-1][0];
    snake[i][1] = snake[i-1][1];
  }
  switch(mov)
  {
    case 0:
      snake[0][0]=snake[1][0];
      if(snake[1][1] == 0)
        snake[0][1] = 6;
      else
        snake[0][1] = snake[1][1]-1;
    break;
    
    case 2:
      snake[0][0]=snake[1][0];
      if(snake[1][1] == 6)
        snake[0][1] = 0;
      else
        snake[0][1] = snake[1][1]+1;
      break;
    
    case 3:
      snake[0][1] = snake[1][1];
      if(snake[1][0] == 0)
        snake[0][0] = 4;
      else
        snake[0][0] = snake[1][0]-1;
      break;  
      
      case 1:
      snake[0][1] = snake[1][1];
      if(snake[1][0] == 4)
        snake[0][0] = 0;
      else
        snake[0][0] = snake[1][0]+1;
      break;
      
      default:
      break;
  }
  eatFruit();
}

void draw(){
  pm.clearDisplay();
  drawSnake();
  drawFruit();
}

void drawSnake(){
  for(int i=0; i<snakeLength; i++)
      pm.setLed(snake[i][0],snake[i][1],true);
}

void drawFruit(){
  pm.setLed(fruit[0],fruit[1],true);
}

void eatFruit()
{
  digitalWrite(BUZZER,LOW);
  if (snake[0][0] == fruit[0] && snake[0][1] == fruit[1])
  {
    digitalWrite(BUZZER,HIGH);
    snakeLength++;
    newFruit();
  }
}

void testDie()
{
  for(int i=1; i<snakeLength; i++)
  {
    if(snake[0][0] == snake[i][0] && snake [0][1] == snake[i][1])
      playing=false;
  }   
}

