/**
  ******************************************************************************
  * @file    fmc.c
  * @author  MCD Application Team
  * @brief   This file includes the fmc-memories supported STM32F7508-Discovery
  *          boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------

   - MT48LC4M32B2B5-6A: SDRAM external memory mounted on STM32F7508-Discovery board.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "memory.h"
#include "memory_msp.h"

/** @addtogroup FMC
  * @{
  */ 
  
/** @defgroup FMC
  * @{
  */ 

/** @defgroup FMC_Private_Types_Definitions FMC Private Types Definitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup FMC_Private_Defines FMC Private Defines
  * @{
  */
/**
  * @}
  */ 

/** @defgroup FMC_Private_Macros FMC Private Macros
  * @{
  */  
/**
  * @}
  */ 

/** @defgroup FMC_Private_Variables FMC Private Variables
  * @{
  */
/**
  * @}
  */ 
    
/** @defgroup FMC_Private_Functions FMC Private Functions
  * @{
  */

#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
/** @defgroup SDRAM_Constants SDRAM Defines
  * @{
  */
/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_8  */
 #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_16 

#define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2
/* #define SDCLOCK_PERIOD                FMC_SDRAM_CLOCK_PERIOD_3 */   

#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter (100Mhz SD clock) */
   
#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)
/**
  * @brief  FMC SDRAM Mode definition register defines
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200) 
/**
  * @}
  */ 
/**
  * @brief  Initializes the SDRAM device.
  * @retval SDRAM status
  */
uint32_t SDRAM_Startup(void)
{
  SDRAM_HandleTypeDef      sdramHandle;
  FMC_SDRAM_TimingTypeDef  Timing;
  FMC_SDRAM_CommandTypeDef Command; 
  __IO uint32_t            tmpmrd = 0;
  
  /* SDRAM device configuration */
  sdramHandle.Instance = FMC_SDRAM_DEVICE;
    
  /* Timing configuration for 100Mhz as SDRAM clock frequency (System clock is up to 200Mhz) */
  Timing.LoadToActiveDelay    = 2;
  Timing.ExitSelfRefreshDelay = 7;
  Timing.SelfRefreshTime      = 4;
  Timing.RowCycleDelay        = 7;
  Timing.WriteRecoveryTime    = 2;
  Timing.RPDelay              = 2;
  Timing.RCDDelay             = 2;
    
  sdramHandle.Init.SDBank             = FMC_SDRAM_BANK1;
  sdramHandle.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
  sdramHandle.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
  sdramHandle.Init.MemoryDataWidth    = SDRAM_MEMORY_WIDTH;
  sdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  sdramHandle.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
  sdramHandle.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  sdramHandle.Init.SDClockPeriod      = SDCLOCK_PERIOD;
  sdramHandle.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  sdramHandle.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

  /* SDRAM controller initialization */
  SDRAM_MspInit();

  if (HAL_SDRAM_Init(&sdramHandle, &Timing) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

/* SDRAM initialization sequence */
  /* Step 1: Configure a clock configuration enable command */
  Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */ 
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);
    
  /* Step 3: Configure a PALL (precharge all) command */ 
  Command.CommandMode            = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);  
  
  /* Step 4: Configure an Auto Refresh command */ 
  Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 8;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
  
  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |\
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |\
                     SDRAM_MODEREG_CAS_LATENCY_2           |\
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
  
  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(&sdramHandle, REFRESH_COUNT); 

  return MEMORY_OK;
}
#endif /*(DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)*/

/**
  * @}
  */  
  
/**
  * @}
  */ 
  
/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
