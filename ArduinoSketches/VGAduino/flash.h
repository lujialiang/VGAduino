#include <ctype.h>

#ifndef _FLASH_H_
#define _FLASH_H_

#define FLASH_ADDR 0xFFF0
#define CE_FLASH_LO() PORTH &= ~(1 << 3)
#define CE_FLASH_HI() PORTH |=  (1 << 3)
#define CE_RAM_LO() PORTH &= ~(1 << 4)
#define CE_RAM_HI() PORTH |=  (1 << 4)
#define CE_ROM_LO() PORTH &= ~(1 << 5)
#define CE_ROM_HI() PORTH |=  (1 << 5)
#define CE_BUF_LO() PORTH &= ~(1 << 6)
#define CE_BUF_HI() PORTH |=  (1 << 6)

#define WP_FLASH_LO() PORTD &= ~(1 << 4)
#define WP_FLASH_HI() PORTD |=  (1 << 4)
#define ALE_FLASH_LO() PORTD &= ~(1 << 5)
#define ALE_FLASH_HI() PORTD |=  (1 << 5)
#define CLE_FLASH_LO() PORTD &= ~(1 << 6)
#define CLE_FLASH_HI() PORTD |=  (1 << 6)

#define RB_FLASH() (PIND & (1 << 7))

/*
Read                  00h 30h   flash_read_row
Read for Copy Back    00h 35h   flash_read_for_copy_back
Read ID               90h -     flash_read_id
Reset                 FFh -   O flash_reset
Page Program          80h 10h   flash_page_program
Copy-Back Program     85h 10h   flash_copy_back_program
Block Erase           60h D0h   flash_erase_block
Random Data Input     85h -     flash_random_data_input
Random Data Output    05h E0h   flash_random_data_output
Read Status           70h -   O flash_read_status
Read EDC Status       7Bh -   O flash_read_edc_status
*/
uint8_t flash_read_status();//
uint8_t flash_read_edc_status();//
int flash_reset (uint8_t wp);//
void flash_read_id (uint8_t buf[]);//
int flash_read_row (uint8_t buf[], uint16_t row_addr);//
int flash_erase_block (uint16_t row_addr);//

int flash_page_program(uint8_t buf[], uint16_t row_addr);
int flash_read_for_copy_back(uint16_t row_addr);
int flash_copy_back_program();
int flash_random_data_input();
int flash_random_data_output();
#endif
