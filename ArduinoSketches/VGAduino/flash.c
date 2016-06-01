#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

#include "flash.h"

void flash_act() {
  CE_RAM_HI();
  CE_ROM_HI();
  CE_FLASH_LO();
  CE_BUF_LO();
  DDRD &= ~(1 << 7); // RB_FLASH_IN
  PORTD |= (1 << 7); // RB_FLASH_IN PULL UP

  WP_FLASH_HI();
  CLE_FLASH_LO();
  ALE_FLASH_LO();
}

int flash_wait_rb(int max_wait_time){
  int j;
  PORTJ |= (1 << 0);
  for(j = 0; ((RB_FLASH() == 0) & (j < max_wait_time)); j++) {
  }
  PORTJ &= ~(1 << 0);
  return j;
}

uint8_t flash_read_status() {
  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x70;
  CLE_FLASH_LO();
  return *(volatile uint8_t*)(FLASH_ADDR);
}

uint8_t flash_read_edc_status() {
  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x78;
  CLE_FLASH_LO();
  return *(volatile uint8_t*)(FLASH_ADDR);
}

int flash_reset (uint8_t wp) {
  flash_act();
  if (wp == 0) {
    WP_FLASH_LO();
  } else {
    WP_FLASH_HI();
  }
  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0xFF;//RESET Command
  CLE_FLASH_LO();
  return flash_wait_rb(0x7FFF);
}

void flash_read_id (uint8_t buf[]) {
  flash_act();

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

int flash_read_row (uint8_t buf[], uint16_t row_addr) {
  int j;
  flash_act();

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
  j = flash_wait_rb(0x7FFF);
  for(int i = 0; i < 2112; i++) {
    buf[i] = *(volatile uint8_t*)(FLASH_ADDR);
  }
  return j;
}

int flash_erase_block (uint16_t row_addr) {
  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0x60;
  CLE_FLASH_LO();
  ALE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0xFF && row_addr;
  *(volatile uint8_t*)(FLASH_ADDR) = 0xFF && (row_addr >> 8);
  ALE_FLASH_LO();
  CLE_FLASH_HI();
  *(volatile uint8_t*)(FLASH_ADDR) = 0xD0;
  CLE_FLASH_LO();
  return flash_wait_rb(0x7FFF);
}

int flash_page_program(uint8_t buf[], uint16_t row_addr) {

}

int flash_read_for_copy_back(uint16_t row_addr){

}

int flash_copy_back_program() {

}

int flash_random_data_input() {

}

int flash_random_data_output() {

}
