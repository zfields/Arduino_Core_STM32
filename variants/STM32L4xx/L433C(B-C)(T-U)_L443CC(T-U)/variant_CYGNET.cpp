/*
 *******************************************************************************
 * Copyright (c) 2020, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

#if defined(ARDUINO_CYGNET)
#include "pins_arduino.h"

// Digital PinName array
const PinName digitalPin[] = {
  PA_0,   //  0 - A0/D0
  PA_1,   //  1 - A1/D1
  PA_2,   //  2 - A2/D2
  PA_3,   //  3 - A3/D3
  PB_1,   //  4 - A4/D4
  PB_8,   //  5 - D5
  PB_9,   //  6 - D6
  PA_8,   //  7 - LED_BUILTIN
  PC_13,  //  8 - USER_BTN
  PB_14,  //  9 - D9
  PB_13,  // 10 - D10
  PB_0,   // 11 - D11
  PB_15,  // 12 - D12
  PB_4,   // 13 - D13
  PA_7,   // 14 - A5
  PA_5,   // 15 - CK
  PB_5,   // 16 - MO
  PA_6,   // 17 - MI
  PA_10,  // 18 - RX
  PA_9,   // 19 - TX
  PH_3,   // 20 - B
  PB_6,   // 21 - SCL
  PB_7,   // 22 - SDA
  PA_13,  // 23 - SWDIO
  PA_14,  // 24 - SWCLK
  PB_10,  // 25 - LPUART1_VCP_RX
  PB_11,  // 26 - LPUART1_VCP_TX
  PH_0,   // 27 - ENABLE_3V3
  PH_1,   // 28 - DISCHARGE_3V3
  PA_15,  // 29 - CHARGE_DETECT
  PA_4,   // 30 - A6/BATTERY_VOLTAGE (STAT)
  PB_3,   // 31 - USB_DETECT
  PA_11,  // 32 - USB_DM
  PA_12,  // 33 - USB_DP
  PC_14,  // 34 - OSC32_IN (LSE)
  PC_15   // 35 - OSC32_OUT (LSE)
};

// Analog (Ax) to digital pin number array
const uint32_t analogInputPin[] = {
   0,  // PA0, A0
   1,  // PA1, A1
   2,  // PA2, A2
   3,  // PA3, A3
   4,  // PB1, A4
  14,  // PA7, A5
  30   // PA4, A6/BATTERY_VOLTAGE (STAT)
};

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

WEAK void initVariant(void)
{
  /* All pins set to high-Z (floating) initially */
  /* DS11449 Rev 8, Section 3.9.5 - Reset Mode: */
  /* In order to improve the consumption under reset, the I/Os state under and after reset is
   * “analog state” (the I/O schmitt trigger is disable). In addition, the internal reset pull-up is
   * deactivated when the reset source is internal.
   */

  /* Turn on the 3V3 regulator */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct); /* PH0 is ENABLE_3V3, PH1 is DISCHARGE_3V3 */
  HAL_GPIO_WritePin(GPIOH, GPIO_InitStruct.Pin, GPIO_PIN_SET); /* Enable 3V3 regulator and disable discharging */
}

/**
  * @brief  System Clock Configuration – Low-power optimized STM32L433 with native USB FS
  *
  * Key features:
  *   • SYSCLK = 48 MHz directly from MSI Range 11 → no PLL, minimal power consumption and EMI
  *   • USB FS (48 MHz) sourced directly from MSI ("The MSI clock can be selected as clock source for the USB FS device when MSI is in Range 11 (48 MHz)", RM0394 Rev 6 §7.2.10)
  *   • HSI (16MHz) enabled for ADC accuracy (optional but common in ST examples)
  *   • LSE enabled with low drive for RTC / ultra-low-power timing
  *   • Voltage Scale 1 required for MSI Range 11 operation
  *   • MSI PLL-mode enabled for better frequency stability in Stop modes
  *
  * References:
  *   • RM0394 Rev 6 (STM32L43x/L44x) – §7.2.10 "Clock source selection for 48 MHz clock (CLK48)"
  *   • RM0394 §7.3.16 "MSI PLL-mode" – "This mode is used to obtain a more accurate MSI frequency in Stop mode"
  *   • STM32CubeL4 v1.18.0 – USB_Device/CDC_Standalone example for NUCLEO-L433RC-P
  *   • AN2867 Rev 11 – "Oscillator design guide for STM8AF/AL/S, STM32 MCUs and MPUs"
  */
WEAK void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

  /* -------------------------------------------------------------------------
   * 1. Voltage scaling – Scale 1 is mandatory for MSI Range 11 (48 MHz)
   *    "Range 11 is only available in voltage range 1" (RM0394 §7.2.5)
   * ------------------------------------------------------------------------- */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 2. LSE drive capability – low drive recommended for lowest current
   *    "The LSE oscillator is in ultra-low-power mode when the LSEDRV[1:0] bits are set to '00'" (RM0394 §7.2.3)
   * ------------------------------------------------------------------------- */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /* -------------------------------------------------------------------------
   * 3. Oscillator configuration
   *     • MSI = ON, Range 11 (48 MHz) – selected directly as SYSCLK and CLK48 source
   *     • HSI = ON (used by ADC in many examples, kept for compatibility)
   *     • LSE = ON (RTC / low-power timing)
   *     • PLL = OFF → lowest possible active-mode consumption
   * ------------------------------------------------------------------------- */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_LSE
                                          | RCC_OSCILLATORTYPE_MSI
                                          | RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_11;          // 48 MHz
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;             // No PLL → minimal power
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 4. Select MSI as system clock source and configure bus dividers
   *    SYSCLK = HCLK = PCLK1 = PCLK2 = 48 MHz
   * ------------------------------------------------------------------------- */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK    | RCC_CLOCKTYPE_PCLK1
                                     | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 5. Peripheral clock selection
   *     • USB FS   → MSI (48 MHz) : "RCC_CCIPR.CLK48SEL = 11" when RCC_USBCLKSOURCE_MSI
   *     • SDMMC1   → MSI (48 MHz) : allowed when MSI is in Range 11
   *     • ADC      → SYSCLK (48 MHz)
   *    (RM0394 Table 44 "CLK48 clock sources" confirms MSI Range 11 is a valid CLK48 source)
   * ------------------------------------------------------------------------- */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC
                                      //  | RCC_PERIPHCLK_OSPI
                                       | RCC_PERIPHCLK_SDMMC1
                                       | RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection    = RCC_ADCCLKSOURCE_SYSCLK;
  // PeriphClkInit.OspiClockSelection   = RCC_OSPICLKSOURCE_SYSCLK;
  PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_MSI;
  PeriphClkInit.UsbClockSelection    = RCC_USBCLKSOURCE_MSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }

  /* -------------------------------------------------------------------------
   * 6. Enable MSI PLL-mode
   *     "This mode improves the accuracy of the MSI clock in Stop mode" (RM0394 §7.3.16)
   * ------------------------------------------------------------------------- */
  HAL_RCCEx_EnableMSIPLLMode();

  /* -------------------------------------------------------------------------
   * 7. Configure wake-up clock from Stop mode
   *     Ensures the system restarts on MSI after Stop (lowest recovery time & power)
   * ------------------------------------------------------------------------- */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
}

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_CYGNET */
