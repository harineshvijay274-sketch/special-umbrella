/*
 * flash.c
 *it contain the function definition for configuring the MCU's internal flash memory acording to the CPU
 *  Created on: Jul 18, 2026
 *      Author: harineshvijay
 */

#include "flash.h"

void flash_config_wait_state(uint8_t hclk){


  //Reset the flash access control register to the default value
  FLASH->ACR &= ~(FLASH_ACR_LATENCY | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN | FLASH_ACR_ICRST
       | FLASH_ACR_DCRST);

  FLASH->ACR |= (FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN );

  // CALCULATING THE LATENCY BASED ON THE hCLK

  uint8_t latency = (hclk - 1)/30;

  FLASH->ACR |= (latency << FLASH_ACR_LATENCY_Pos);
}
