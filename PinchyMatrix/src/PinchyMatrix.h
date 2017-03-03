/*
 *    PinchyMatrix.h - A library for controling Leds with a MAX7219/MAX7221
 *    This library has been designed for Pinchy, our robot, but you can use it
 *    with a display.
 *    Copyright (c) 2007 Eberhard Fahle [based on LedControl.cpp]
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

#ifndef PinchyMatrix_h
#define PinchyMatrix_h

#include <avr/pgmspace.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


class PinchyMatrix {
    private :
        byte f[5];
        /* The array for shifting the data to the devices */
        byte spidata[16];
        /* Send out a single command to the device */
        void spiTransfer(byte opcode, byte data);

        /* We keep track of the led-status for all 8 devices in this array */
        byte status[64];
        /* Data is shifted out of this pin*/
        int SPI_MOSI;
        /* The clock is signaled on this pin */
        int SPI_CLK;
        /* This one is driven LOW for chip selectzion */
        int SPI_CS;
        /* The maximum number of devices we use */

    public:
        /*
         * Create a new controler
         * Params :
         * dataPin		pin on the Arduino where data gets shifted out
         * clockPin		pin for the clock
         * csPin		pin for selecting the device
         */
        PinchyMatrix(int dataPin, int clkPin, int csPin);

        /*
         * Set the shutdown (power saving) mode for the device
         * Params :
         * status	If true the device goes into power-down mode. Set to false
         *		for normal operation.
         */
        void shutdown(bool status);

        /*
         * Set the number of digits (or rows) to be displayed.
         * See datasheet for sideeffects of the scanlimit on the brightness
         * of the display.
         * Params :
         * limit	number of digits to be displayed (1..8)
         */
        void setScanLimit(int limit);

        /*
         * Set the brightness of the display.
         * Params:
         * intensity	the brightness of the display. (0..15)
         */
        void setIntensity(int intensity);

        /*
         * Switch all Leds on the display off.
         */
        void clearDisplay();

        /*
         * Set the status of a single Led.
         * Params :
         * row	the row of the Led (0..4)
         * col	the column of the Led (0..6)
         * state	If true the led is switched on,
         *		if false it is switched off
         */
        void setLed(int row, int col, boolean state);

        /*
         * Set all 5 Led's in a column to a new state
         * Params:
         * col	col which is to be set (0..6)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setCol(int col, byte value);

        /*
         * Set all 7 Led's in a row to a new state
         * Params:
         * row	row which is to be set (0..4)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setRow(int row, byte value);

	/*
         * Set all 5 Led's columns to a new state
         * Params:
         * f[7]  from left to right, cols will be set.
         *       each bit set to 1 will light up the
         *	 corresponding Led in the corresponding
	 *       column
         */
        void setMatrixByCols(byte f[7]);

	/*
         * Set all 7 Led's rows to a new state
         * Params:
         * f[5]  from bottom to top, rows will be set.
         *       each bit set to 1 will light up the
         *	 corresponding Led in the corresponding
	 *       row
         */
	void setMatrixByRows(byte f[5]);


	void setMatrix();

	/*
         * Move the drawing one column to the left
         */
	void left();

	/*
         * Move the drawing one column to the right
         */
	void right();

	/*
         * Move the drawing one row to the top
         */
	void up();

	/*
         * Move the drawing one row to the bottom
         */
	void down();

	/*
         * Set a number
         * Params:
         * num   number between 0 and 99.
         *       The number will appear on the
	 *       matrix
         */
	void number(int num);

	/*
         * A lot of faces, test them
         */
	void kitten();
	void veryHappy();
	void smiling();
	void happy();
	void kissing();
	void neutral();
	void smirking();
	void sad();
	void tongue();
	void openMouth();
	void heart();
	void bd();

	/*
         * Different arrows, useful.
         */
	void rightArrow();
	void leftArrow();
	void upArrow();
	void downArrow();

  //Extra
  void equals();
  void FBplus1();
  void FBlike();

  //TEMP just for testing
  void binary(String bin);

};

#endif	//PinchyMatrix.h
