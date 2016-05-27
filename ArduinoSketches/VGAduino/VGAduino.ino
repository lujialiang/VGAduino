extern "C" {
#include "config.h"
#include "microrl.h"
}

// create microrl object and pointer on it
microrl_t rl;
microrl_t * prl = &rl;


int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor
int thirdSensor = 0;    // digital sensor
int inByte = 0;         // incoming serial byte

void setup() {
  DDRC = 0xFF;
  PORTC = 0x00;

  XMCRA |= (1 << SRE); // EMI on

  DDRH |= (1 << 3);
  DDRH |= (1 << 4);
  DDRH |= (1 << 5);
  DDRH |= (1 << 6);

  PORTH |= (1 << 3); //~CE FLASH Hi
  PORTH &= ~(1 << 4); //~CE RAM Lo
  PORTH |= (1 << 5); //~CE ROM Hi
  PORTH |= (1 << 6); //~CE BUF Hi

  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(2, INPUT);   // digital sensor is on digital pin 2
  establishContact();  // send a byte to establish contact until receiver responds

  // call init with ptr to microrl instance and print callback
  microrl_init (prl, print);
  // set callback for execute
  microrl_set_execute_callback (prl, execute);
  // set callback for completion
  microrl_set_complete_callback (prl, complet);
  // set callback for Ctrl+C
  microrl_set_sigint_callback (prl, sigint);

  prl->print("Test for prl->print(const char* str)\r\n");
  print_help();
  
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    microrl_insert_char (prl, inByte);
/*
    // read first analog input, divide by 4 to make the range 0-255:
    firstSensor = analogRead(A0) / 4;
    // delay 10ms to let the ADC recover:
    delay(10);
    // read second analog input, divide by 4 to make the range 0-255:
    secondSensor = analogRead(1) / 4;
    // read  switch, map it to 0 or 255L
    thirdSensor = map(digitalRead(2), 0, 1, 0, 255);
    // send sensor values:
    //Serial.write(firstSensor);
    //Serial.write(secondSensor);
    //Serial.write(thirdSensor);
    Serial.println(firstSensor);
    Serial.println(secondSensor);
    Serial.println(thirdSensor);
*/
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void print(const char * str) {
  Serial.print(str);
}
