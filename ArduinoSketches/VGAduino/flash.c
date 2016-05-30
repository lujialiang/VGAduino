#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

#include "flash.h"

void flash_read_id (uint8_t buf[]) {
  CE_RAM_HI();
  CE_ROM_HI();
  CE_FLASH_LO();
  CE_BUF_LO();

  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x90;//Read ID Command
  CLE_FLASH_LO();
  ALE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x00;//Address 1 cycle
  ALE_FLASH_LO();
  for(int i = 0; i < 5; i++) {
    buf[i] = *(volatile uint8_t*)(FLASH_ADDR);
  }
}

int flash_read (uint8_t buf[], uint16_t row_addr) {
  int j;
  DDRJ |= (1 << 0);
  
  CE_RAM_HI();
  CE_ROM_HI();
  CE_FLASH_LO();
  CE_BUF_LO();

  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x00;//Command
  CLE_FLASH_LO();
  ALE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x00;//Address 1 cycle
  *(volatile uint8_t*)(FLASH_ADDR) = 0x00;//Address 2 cycle
  *(volatile uint8_t*)(FLASH_ADDR) = 0xFF && row_addr;//Address 3 cycle
  *(volatile uint8_t*)(FLASH_ADDR) = 0xFF && (row_addr >> 8);//Address 4 cycle
  ALE_FLASH_LO();
  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x30;//Read Command
  CLE_FLASH_LO();
  //while(~(RB_FLASH)) {}
  PORTJ |= (1 << 0);
  for(j = 0; RB_FLASH() == 0; j++) {
  }
  PORTJ &= ~(1 << 0);
  for(int i = 0; i < 2112; i++) {
    buf[i] = *(volatile uint8_t*)(FLASH_ADDR);
  }
  return j;
}

