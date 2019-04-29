/**
  ******************************************************************************
  * @file    memory.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          all external memory configuration helper.
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

/** @addtogroup MEMORY
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEMORY_H
#define __MEMORY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"

/** @addtogroup MEMORY
  * @{
  */


/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/** @defgroup MEMORY_Exported_Types Memory Exported Types
  * @{
  */
/* Error codes */
#define MEMORY_OK          ((uint32_t)0x00)
#define MEMORY_ERROR       ((uint32_t)0x01)


/* QSPI Init Mode */
#define QSPI_MODE          ((uint32_t)0x00)    /* Init in quad-spi mode for XiP mode */
#define SPI_NOR_MODE       ((uint32_t)0x01)    /* Emulate an spi-nor using QSPI with one line for BootROM scenario */
/**
  * @}
  */

#define USE_EXTERNAL_SDRAM   1
#define USE_INTERNAL_SRAM    4
#define USE_QSPI             5
#define USE_SPI_NOR          7
#define USE_SDCARD           8

/*
  @verbatim
  ==============================================================================
                     ##### How to use this Config #####
  ==============================================================================
    [..]
      The configuration bellow allows simple selection switch between configuration

      The configuration is composed mainly by two area:
      (#) CODE_AREA: Used to specify external memory used for code execution
         (##) XiP Mode:
             (+++) USE_QSPI : QSPI Flash is used for code execution
         (##) BootROM Mode:
             (+++) USE_EXTERNAL_SDRAM : External SDRAM is used for code execution
             (+++) USE_INTERNAL_SRAM  : Internal SRAM is used for code execution

      (#) DATA_AREA: Used to specify volatile memory used for data holding
         (##) USE_EXTERNAL_SDRAM : External SDRAM is used for code execution
         (##) USE_INTERNAL_SRAM  : Internal SRAM is used for code execution

      (#) BINARY_AREA: Used to specify storage memory used for binary storage
                       Binary file will be copied from those area into the one
                       specified by CODE_AREA after boot for execution
         (##) USE_SPI_NOR : SPI NOR Flash is used for binary storage
             (+++) BINARY_BASE_OFFSET : Offset of the binary within SPI NOR Flash
             (+++) BINARY_SIZE        : Size of the binary image
         (##) USE_SDCARD  : SDCard is used for binary storage
             (+++) BINARY_FILENAME : Name of the binary file to be executed

      Below an automatic update APPLICATION_ADDRESS to default value based on
      user configuration, which can be tailored as required

      Finally a set of check allows to avoid unsupported combined configuration.

  @endverbatim
*/
#define DATA_AREA           USE_EXTERNAL_SDRAM
#define CODE_AREA           USE_QSPI
/* #define BINARY_AREA         USE_SDCARD */

#if (BINARY_AREA == USE_SDCARD)
  #define BINARY_FILENAME      "Project.bin"

#elif (BINARY_AREA == USE_SPI_NOR)
  #define BINARY_BASE_OFFSET   0x0U
  #define BINARY_SIZE         23000U

#endif


/*************************************************/
/*                                               */
/*     Configure Application Startup Address     */
/*                                               */
/*************************************************/
#define SDRAM_DEVICE_ADDR  ((uint32_t)0xC0000000)

#if (CODE_AREA == USE_QSPI)
  #define APPLICATION_ADDRESS            QSPI_BASE

#elif (CODE_AREA == USE_SPI_NOR)
  /* QSPI is used to emulate SPI-NOR*/
  #define APPLICATION_ADDRESS            QSPI_BASE

#elif (CODE_AREA == USE_EXTERNAL_SDRAM)
  #define APPLICATION_ADDRESS            SDRAM_DEVICE_ADDR

#elif (CODE_AREA == USE_INTERNAL_SRAM)
  #define APPLICATION_ADDRESS            RAMDTCM_BASE

#else
  #error "APPLICATION_ADDRESS not defined"

#endif

/****************************************************/
/*                                                  */
/*     Check to avoid unsupported configuration     */
/*                                                  */
/****************************************************/
#if ((DATA_AREA != USE_EXTERNAL_SDRAM) && \
     (DATA_AREA != USE_INTERNAL_SRAM))
  #error "Wrong type used for DATA_AREA"
#endif

#if (defined(BINARY_AREA) && (BINARY_AREA != USE_SPI_NOR) && (BINARY_AREA != USE_SDCARD))
  #error "Wrong type used for BINARY_AREA"
#endif

#if (((CODE_AREA == USE_QSPI) && defined(BINARY_AREA))                            || \
     ((CODE_AREA != USE_QSPI) && !defined(BINARY_AREA)) || \
     (CODE_AREA == USE_SPI_NOR)                                                   || \
     (CODE_AREA == USE_SDCARD))
  #error "Wrong type used for CODE_AREA"
#endif

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Memory_Exported_Functions Non-Volatile memory
  * @{
  */
uint32_t QSPI_Startup(void);
uint32_t SDCARD_Startup(void);
/**
  * @}
  */


/** @addtogroup Memory_Exported_Functions Volatile memory
  * @{
  */
uint32_t SDRAM_Startup(void);
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __MEMORY_H */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
