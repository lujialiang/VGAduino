extern "C" {
#include "config.h"
#include "microrl.h"
}

// create microrl object and pointer on it
microrl_t rl;
microrl_t * prl = &rl;

int inByte = 0; // incoming serial byte

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

  // call init with ptr to microrl instance and print callback
  microrl_init (prl, print);
  // set callback for execute
  microrl_set_execute_callback (prl, execute);
  // set callback for completion
  microrl_set_complete_callback (prl, complet);
  // set callback for Ctrl+C
  microrl_set_sigint_callback (prl, sigint);

  print_help();
}

void loop() {
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    microrl_insert_char (prl, inByte);
  }
}

void print(const char * str) {
  Serial.print(str);
}
