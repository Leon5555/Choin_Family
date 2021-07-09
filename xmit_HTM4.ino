/*
        DIY Arduino based RC Transmitter
  by Dejan Nedelkovski, www.HowToMechatronics.com
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
  LRC 4/2/21 Code modified to drive Papa1 JoyStick 2 installed wrong, X and Y are switched
  sovled by mapping Y=A1  X=A0 for inputs 
  removed t1 and t2 mapped to D0 and D1 poss conflict with TX and RX
  can't initialize joystick1 button on 9 or 10, but works if I use button1... 
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// #include <Wire.h> only use if adding I2C and MPU6050 modified code and took out MPU6050 lines LRC
// 
// Define the digital inputs
#define jB1 10  // Joystick button 1
#define jB2 7  // Joystick button 2
// #define t1 0   // Toggle switch 1
// #define t2 1   // Toggle switch 1
#define b1 9   // Button 1  Temp using JB1 as button 1 5/6/21
#define b2 A4 // Button 2
#define b3 A7 // Button 3
#define b4 3   // Button 4
#define pot1 A5  // sliding pot 1
#define pot2 A6  // sliding pot 2

RF24 radio(5, 6);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address can be any 5 digits
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
//  byte tSwitch1;
//  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};
Data_Package data; //Create a variable with the above structure
void setup() {
  Serial.begin(9600);
  
 
  
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  // Activate the Arduino internal pull-up resistors
  pinMode(jB1, INPUT_PULLUP);
  pinMode(jB2, INPUT_PULLUP);
//  pinMode(t1, INPUT_PULLUP);
//  pinMode(t2, INPUT_PULLUP);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);
  
  // Set initial default values
  data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
//  data.tSwitch1 = 1;
//  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}
void loop() {
  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(A0), 0, 1023, 0, 255);  //  Convert the analog read value from 0 to 1023 
  data.j1PotY = map(analogRead(A1), 0, 1023, 0, 255);  //   into a BYTE value from 0 to 255 
  data.j2PotX = map(analogRead(A2), 0, 1023, 255, 0);   //  LRC 4/2/21 Reversed this line for Papa1 Steering Axis
  data.j2PotY = map(analogRead(A3), 0, 1023, 0, 255); // 
  data.pot1 = map(analogRead(A5), 0, 1023, 0, 255);
  data.pot2 = map(analogRead(A6), 0, 1023, 0, 255);
  // Read all digital inputs
  data.j1Button = digitalRead(jB1);
  data.j2Button = digitalRead(jB2);
//  data.tSwitch2 = digitalRead(t2);
  data.button1 = digitalRead(b1);
  data.button2 = digitalRead(b2);
  data.button3 = digitalRead(b3);
  data.button4 = digitalRead(b4);
 

  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));


 
}
