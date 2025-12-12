/*
 *******************************************************************************
 * Copyright (c) 2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#if defined(ARDUINO_SWAN_R5)
#include "pins_arduino.h"

// Digital PinName array
const PinName digitalPin[] = {
  PD_9,   //  0 - D0
  PD_8,   //  1 - D1
  PF_15,  //  2 - D2
  PE_13,  //  3 - D3
  PE_3,   //  4 - D4
  PE_11,  //  5 - D5
  PE_9,   //  6 - D6
  PF_13,  //  7 - D7
  PF_12,  //  8 - D8
  PD_15,  //  9 - D9
  PA_4,   // 10 - A10/D10
  PA_7,   // 11 - A11/D11
  PA_6,   // 12 - A12/D12
  PA_5,   // 13 - A13/D13
  PB_9,   // 14 - D14
  PE_1,   // 15 - D15
  PD_0,   // 16 - CS
  PA_3,   // 17 - A0
  PA_1,   // 18 - A1
  PC_3,   // 19 - A2
  PC_1,   // 20 - A3
  PC_4,   // 21 - A4
  PC_5,   // 22 - A5
  PB_1,   // 23 - A6
  PD_1,   // 24 - CK
  PC_2,   // 25 - A7
  PB_15,  // 26 - MO
  PB_14,  // 27 - MI
  PA_10,  // 28 - RX
  PG_12,  // 29 - RTS
  PA_9,   // 30 - TX
  PB_4,   // 31 - CTS
  PH_3,   // 32 - B
  PD_5,   // 33 - QEN
  PD_6,   // 34 - RX2
  PA_2,   // 35 - A9/TX2
  PD_4,   // 36 - RTS2
  PD_3,   // 37 - CTS2
  PB_11,  // 38 - RX3
  PB_10,  // 39 - TX3
  PD_2,   // 40 - RTS3
  PB_13,  // 41 - CTS3
  PB_6,   // 42 - SCL
  PF_1,   // 43 - SCL2
  PB_7,   // 44 - SDA
  PF_0,   // 45 - SDA2
  PC_9,   // 46 - SDA3
  PC_0,   // 47 - A14/SCL3
  PC_11,  // 48 - QCS
  PE_10,  // 49 - QCLK
  PE_15,  // 50 - QIO3
  PE_14,  // 51 - QIO2
  PB_0,   // 52 - A15/QIO1
  PE_12,  // 53 - QIO0
  PE_2,   // 54 - LED_BUILTIN
  PC_13,  // 55 - USER_BTN
  PA_13,  // 56 - SWDIO
  PA_14,  // 57 - SWCLK
  PB_3,   // 58 - JTDO
  PA_15,  // 59 - JTDI
  PG_7,   // 60 - LPUART1_VCP_TX (TX0)
  PG_8,   // 61 - LPUART1_VCP_RX (RX0)
  PE_4,   // 62 - ENABLE_3V3
  PE_6,   // 63 - DISCHARGE_3V3
  PA_0,   // 64 - A8/BATTERY_VOLTAGE (STAT)
  PC_6,   // 65 - USB_DETECT
  PA_11,  // 66 - USB_DM
  PA_12,  // 67 - USB_DP
  PC_14,  // 68 - OSC32 IN (LSE)
  PC_15,  // 69 - OSC32 OUT (LSE)
  PG_0,   // 70 - VERSION BIT 0
  PG_1,   // 71 - VERSION BIT 1
  PG_2,   // 72 - VERSION BIT 2
  PG_3,   // 73 - VERSION BIT 3
  PB_2    // 74 - BOOT1 (Do not connect to VDD)
};

// Analog (Ax) pin number array
const uint32_t analogInputPin[] = {
  17, // PA3, A0
  18, // PA1, A1
  19, // PC3, A2
  20, // PC1, A3
  21, // PC4, A4
  22, // PC5, A5
  23, // PB1, A6
  25, // PC2, A7
  64, // PA0, A8/BATTERY_VOLTAGE (STAT)
  35, // PA2, A9/TX2
  10, // PA4, A10/D10
  11, // PA7, A11/D11
  12, // PA6, A12/D12
  13, // PA5, A13/D13
  47, // PC0, A14/SCL3
  52  // PB0, A15/QIO1
};

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

WEAK void initVariant(void)
{
  /* All pins set to high-Z (floating) initially */
  /* DS12023 Rev 5, Section 3.10.5 - Reset mode: */
  /* In order to improve the consumption under reset, the I/Os state under and after reset is
   * “analog state” (the I/O schmitt trigger is disable). In addition, the internal reset pull-up is
   * deactivated when the reset source is internal.
   */

  /* Turn on the 3V3 regulator */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOE, GPIO_InitStruct.Pin, GPIO_PIN_SET);
}

/**
  * @brief  System Clock Configuration – High-performance STM32L4R5xx with PLL from MSI
  *
  * Key features:
  *   • SYSCLK = 120 MHz from PLL (MSI Range 11 /6 × 30 /2 via PLLR)
  *   • USB FS uses MSI Range 11 as CLK48 source ("The MSI clock can be selected as clock source for the USB FS device, SDMMC1 and RNG when the MSI is in Range 11 (48 MHz)", RM0432 Rev 9 §6.2.10)
  *   • OSPI clocked from SYSCLK (120 MHz)
  *   • ADC clocked from SYSCLK (120 MHz)
  *   • LSE enabled for RTC timing
  *   • Voltage Scale 1 Boost required for 120 MHz ("In Range 1 boost mode (R1MODE = 0), the maximum system clock frequency is 120 MHz", RM0432 §5.3.11)
  *   • MSI PLL-mode enabled ("The MSI PLL-mode can be used to obtain a more accurate MSI frequency when the device wakes up from Stop mode", RM0432 §6.3.16)
  *
  * References:
  *   • RM0432 Rev 9 – STM32L4+ Series advanced Arm-based 32-bit MCUs (www.st.com/resource/en/reference_manual/rm0432-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
  *   • RM0432 §6.2.5 – PLL characteristics: VCO frequency range 64-344 MHz
  *   • RM0432 §6.4.4 – PLL configuration: PLLM divides input to 4-16 MHz range
  *   • RM0432 §6.2.14 – "PLLR clock output is used to generate the system clock"
  *   • RM0432 Table 15 – Flash latency 5 wait states at 120 MHz in Range 1 Boost
  *   • RM0432 Table 31 – LSEDRV[1:0] = 01 for medium low drive capability
  *   • STM32CubeL4 v1.18.0 (07 Jun 2023) – USB_Device examples for NUCLEO-L4R5ZI (www.st.com/en/embedded-software/stm32cubel4.html)
  *   • AN2867 Rev 23 (Jan 2025) – Guidelines for oscillator design on STM32 MCUs/MPUs (www.st.com/resource/en/application_note/an2867-guidelines-for-oscillator-design-on-stm8afals-and-stm32-mcusmpus-stmicroelectronics.pdf)
  */
WEAK void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

  /* -------------------------------------------------------------------------
   * 1. Voltage scaling – Scale 1 Boost for SYSCLK up to 120 MHz
   *    "Range 1 boost mode is selected by setting the BOOSTEN bit … and allows to reach 120 MHz"
   *    (RM0432 Rev 9 §5.3.11 Voltage regulator)
   * ------------------------------------------------------------------------- */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 2. LSE drive capability – medium low (LSEDRV[1:0] = 01)
   *    "LSEDRV[1:0] = 01: medium low drive capability"
   *    (RM0432 Rev 9 Table 31 LSE drive capability)
   *    See AN2867 Rev 23 §5.2 for STM32-compatible low-speed resonators
   * ------------------------------------------------------------------------- */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMLOW);

  /* -------------------------------------------------------------------------
   * 3. Oscillator configuration
   *     • MSI = ON, Range 11 (48 MHz) – PLL source and USB CLK48
   *     • LSE = ON – for RTC
   *     • PLL = ON – MSI /6 ×30 /2 (PLLR) = 120 MHz SYSCLK
   *       PLL input = 48 /6 = 8 MHz (4-16 MHz range, RM0432 §6.4.4)
   *       VCO = 8 ×30 = 240 MHz (64-344 MHz range, RM0432 §6.2.5)
   *       SYSCLK from PLLR /2 = 120 MHz (RM0432 §6.2.14)
   *     • HSI not included (off by default)
   * ------------------------------------------------------------------------- */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV5;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 4. System clock mux and bus dividers
   *    SYSCLK = PLLR = 120 MHz, no AHB/APB dividers
   *    Flash latency = 5 WS ("5 wait states" at 120 MHz, Range 1 Boost, RM0432 Table 15)
   * ------------------------------------------------------------------------- */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK    | RCC_CLOCKTYPE_PCLK1
                                     | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 5. Peripheral clock selection
   *     • USB → MSI (48 MHz, valid in Range 11, RM0432 §6.2.10)
   *     • OSPI → SYSCLK (120 MHz)
   *     • ADC → SYSCLK (120 MHz)
   *     • SDMMC1 not selected
   *    (RM0432 §6.3.2: Peripheral clocks set after SYSCLK)
   * ------------------------------------------------------------------------- */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC
                                       | RCC_PERIPHCLK_OSPI
                                      //  | RCC_PERIPHCLK_SDMMC1
                                       | RCC_PERIPHCLK_USB
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  PeriphClkInit.OspiClockSelection = RCC_OSPICLKSOURCE_SYSCLK;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_MSI;
  // PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_NONE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 6. MSI PLL-mode enable
   *    "The MSI PLL-mode can be used to obtain a more accurate MSI frequency when the device wakes up from Stop mode"
   *    (RM0432 Rev 9 §6.3.16 MSI PLL-mode)
   * ------------------------------------------------------------------------- */
  HAL_RCCEx_EnableMSIPLLMode();
}

#ifdef __cplusplus
}
#endif
#endif /* ARDUINO_SWAN_R5* */
