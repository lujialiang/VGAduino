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

void flash_read_id (uint8_t buf[]);
int flash_read (uint8_t buf[], uint16_t addr);

#endif
