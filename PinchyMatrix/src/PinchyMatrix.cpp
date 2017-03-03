/*
 *    PinchyMatrix.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    This library has been designed for Pinchy, our robot, but you can use it
 *    with a display.
 *    Copyright (c) 2007 Eberhard Fahle [based on LedControl.cpp]
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */


#include "PinchyMatrix.h"

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15


PinchyMatrix::PinchyMatrix(int dataPin, int clkPin, int csPin) {

    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    SPI_MOSI=dataPin;
    for(int i=0;i<64;i++)
        status[i]=0x00;
    spiTransfer(OP_DISPLAYTEST,0);
    //scanlimit is set to max on startup
    setScanLimit(7);
    //decode is done in source
    spiTransfer(OP_DECODEMODE,0);
    clearDisplay();
    //we go into shutdown-mode on startup
    shutdown(true);
}

void PinchyMatrix::shutdown(bool b) {
    if(b)
        spiTransfer(OP_SHUTDOWN,0);
    else
        spiTransfer(OP_SHUTDOWN,1);
}

void PinchyMatrix::setScanLimit(int limit) {
    if(limit>=0 && limit<8)
        spiTransfer(OP_SCANLIMIT,limit);
}

void PinchyMatrix::setIntensity(int intensity) {
    if(intensity>=0 && intensity<16)
        spiTransfer(OP_INTENSITY,intensity);
}

void PinchyMatrix::clearDisplay() {
    for(int i=0;i<8;i++) {
        status[i]=0;
        spiTransfer(i+1,status[i]);
    }
}

void PinchyMatrix::setLed(int row, int col, boolean state) {
    byte val=0x00;
    col++;
    row++;
    if(row<1 || row>5 || col<1 || col>7)
        return;
    val=B10000000 >> row;
    if(state)
        status[col]=status[col]|val;
    else {
        val=~val;
        status[col]=status[col]&val;
    }
    spiTransfer(col,status[col]);
}

void PinchyMatrix::setCol(int col, byte value) {
    if(col<0 || col>7)
        return;
    status[col]=value*4;
    spiTransfer(col+1,status[col]);
}

void PinchyMatrix::setRow(int row, byte value) {
    byte val;
    value*=2;
    if(row<0 || row>4)
        return;
    for(int col=0;col<8;col++) {
        val=value >> (7-col);
        val=val & 0x01;
        setLed(row,col,val);
    }
}


void PinchyMatrix::spiTransfer(volatile byte opcode, volatile byte data) {
    #define MAXBYTES 2

    for(int i=0;i<MAXBYTES;i++)
        spidata[i]=(byte)0;
    //put our device data into the array
    spidata[1]=opcode;
    spidata[0]=data;
    //enable the line
    digitalWrite(SPI_CS,LOW);
    //Now shift out the data
    for(int i=MAXBYTES;i>0;i--)
        shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}

void PinchyMatrix::setMatrixByCols(byte tmp[7])
{
    for(int i=0; i<7; i++)
	spiTransfer(i+1,tmp[i]*4);
}

void PinchyMatrix::setMatrixByRows(byte tmp[5])
{
    for(int i=0; i<5; i++)
    {
	f[i]=tmp[i];
        setRow(i,tmp[i]);
    }
}

void PinchyMatrix::setMatrix()
{
    for(int i=0; i<5; i++)
        setRow(i,f[i]);
}

void PinchyMatrix::left()
{
  for (int i = 0; i<5; i++)
  {
      f[i]*=2;

    if(f[i]>127)
      f[i]-=127;
  }
  setMatrix();
}

void PinchyMatrix::right()
{
  for(int i=0; i<5; i++)
  {
    if(f[i]%2 == 1)
      f[i]+=127;
    f[i]/=2;

  }
  setMatrix();
}

void PinchyMatrix::up()
{
  byte tmp = f[4];
  for (int i=4; i>0; i--)
    f[i]=f[i-1];
  f[0] = tmp;
  setMatrix();
}

void PinchyMatrix::down()
{
  byte tmp = f[0];
  for(int i=0; i<4; i++)
    f[i] = f[i+1];
  f[4] = tmp;
  setMatrix();
}

void PinchyMatrix::number(int num) //numero entre 0 y 99
{
  byte decena = num/10;
  byte unidad = num-(10*decena);
  byte tabla[5][10] = {{7,1,7,7,1,7,7,1,7,7}, //f(0)
                       {5,1,4,1,1,1,5,1,5,1}, //f(1)
                       {5,1,7,3,7,7,7,1,7,7}, //f(2)
                       {5,1,1,1,5,4,4,1,5,5}, //f(3)
                       {7,3,7,7,5,7,7,7,7,7}};//f(4)
                      //0,1,2,3,4,5,6,7,8,9

  for(int i=0; i<5; i++)
  {
    f[i] = (tabla[i][unidad])+(16*tabla[i][decena]);
  }
  setMatrix();
}

void PinchyMatrix::kitten(){
  f[0] = B00110110;
  f[1] = B01011101;
  f[2] = B01001001;
  f[3] = B01000001;
  f[4] = B00000000;
  setMatrix();
}

void PinchyMatrix::veryHappy(){
	f[4] = B01111111;
	f[3] = B01010101;
	f[2] = B01010101;
	f[1] = B00110110;
	f[0] = B00011100;
	setMatrix();
}

void PinchyMatrix::smiling(){
	f[4] = B01111111;
	f[3] = B01000001;
	f[2] = B01000001;
	f[1] = B00100010;
	f[0] = B00011100;
	setMatrix();
}

void PinchyMatrix::happy(){
	f[4] = B00000000;
	f[3] = B01000001;
	f[2] = B01000001;
	f[1] = B00100010;
	f[0] = B00011100;
	setMatrix();
}

void PinchyMatrix::kissing(){
	f[4] = B00111100;
	f[3] = B00000010;
	f[2] = B00001100;
	f[1] = B00000010;
	f[0] = B00111100;
	setMatrix();
}

void PinchyMatrix::neutral(){
	f[4] = B00000000;
	f[3] = B00000000;
	f[2] = B00111110;
	f[1] = B00000000;
	f[0] = B00000000;
	setMatrix();
}

void PinchyMatrix::smirking(){
	f[4] = B00000000;
	f[3] = B00000001;
	f[2] = B00000001;
	f[1] = B00000010;
	f[0] = B00001100;
	setMatrix();
}

void PinchyMatrix::sad(){
	f[4] = B00011100;
	f[3] = B00100010;
	f[2] = B01000001;
	f[1] = B01000001;
	f[0] = B01000001;
	setMatrix();
}

void PinchyMatrix::tongue(){
	f[4] = B00000000;
	f[3] = B01111111;
	f[2] = B00001001;
	f[1] = B00001001;
	f[0] = B00000110;
	setMatrix();
}

void PinchyMatrix::openMouth(){
	f[4] = B00011100;
	f[3] = B00111110;
	f[2] = B00111110;
	f[1] = B00111110;
	f[0] = B00011100;
	setMatrix();
}

void PinchyMatrix::heart(){
	f[4] = B00110110;
	f[3] = B01001001;
	f[2] = B00100010;
	f[1] = B00010100;
	f[0] = B00001000;
	setMatrix();
}

void PinchyMatrix::bd(){
	f[4] = B01100110;
	f[3] = B01010101;
	f[2] = B01100101;
	f[1] = B01010101;
	f[0] = B01100110;
	setMatrix();
}

void PinchyMatrix::rightArrow(){
	f[4] = B00001000;
	f[3] = B00000100;
	f[2] = B00111110;
	f[1] = B00000100;
	f[0] = B00001000;
	setMatrix();
}

void PinchyMatrix::leftArrow(){
	f[4] = B00001000;
	f[3] = B00010000;
	f[2] = B00111110;
	f[1] = B00010000;
	f[0] = B00001000;
	setMatrix();
}

void PinchyMatrix::upArrow(){
	f[4] = B00001000;
	f[3] = B00011100;
	f[2] = B00101010;
	f[1] = B00001000;
	f[0] = B00001000;
	setMatrix();
}

void PinchyMatrix::downArrow(){
	f[4] = B00001000;
	f[3] = B00001000;
	f[2] = B00101010;
	f[1] = B00011100;
	f[0] = B00001000;
	setMatrix();
}

void PinchyMatrix::equals(){
  f[4] = B00000000;
	f[3] = B00111110;
	f[2] = B00000000;
	f[1] = B00111110;
	f[0] = B00000000;
  setMatrix();
}

void PinchyMatrix::FBplus1(){
  f[4] = B00000010;
	f[3] = B00100110;
	f[2] = B01110010;
	f[1] = B00100010;
	f[0] = B00000111;
  setMatrix();
}

void PinchyMatrix::FBlike(){
  f[4] = B00001100;
	f[3] = B00001100;
	f[2] = B00011111;
	f[1] = B01111111;
	f[0] = B01111111;
  setMatrix();
}

void PinchyMatrix::binary(String bin){
  int k = 0, z = 0;
  f[0] = B00000000;
  f[1] = B00000000;
  f[2] = B00000000;
  f[3] = B00000000;
  f[4] = B00000000;

  for (int i = 4; i >= 0; i--){
      for (int j = 0; j < 7; j++){
          if( bin.charAt(z) == '0'){
            bitClear(f[i], 6-k);
          }
          else if( bin.charAt(z) == '1'){
            bitSet(f[i], 6-k);
          }
          z++;
          k++;
      }
      k = 0;
  }
	setMatrix();
}
