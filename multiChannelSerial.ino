/*
MIT License

Copyright (c) 2017 Miguel Ascanio Gómez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Tlc5940.h>

const long BAUD_RATE = 115200;

/*
 * Converts two bytes (meant to be read from Serial input) into a 16 bits int
 */
int bytesToColor(int b1, int b2) {
  int r = 0;
  
  r  = b1;
  r  = ((r << 8) & 0xFFFF);
  r |= (b2 & 0xFF);
  r &= 0xFFFF;

  return r;
}

/*
 * Loop untill a valid byte is read, and return it.
 */
int readNextByte() {
  int in = 0;
  while(!Serial.available() > 0) {}
  while ((in = Serial.read()) == -1) {}

  return in;
}

void setup() {
  Serial.begin(BAUD_RATE);
  Tlc.init(0); // Initiates the TLC5940 and set all channels off
}

void loop() {
  if (Serial.available() > 0) {
    int in = 0;
    int i = 0;
    int channel;
    int value;
    int b1, b2;
    
    while (Serial.available() > 0 && in != -1 && in != '\n' && in != '\r' && i < 3) {
      in = Serial.read();
      if (in != -1 && in != '\n' && in != '\r' && i < 3) {
        // Read channel and value
        channel = in;
        b1 = readNextByte();
        b2 = readNextByte();
        value = bytesToColor(b1, b2);

        Tlc.set(channel, value);
        Tlc.update();
      }
    }
  }
}
