/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
void WFI_SET(void);
void INTX_DISABLE(void);
void INTX_ENABLE(void);
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
u8 f_typetell(u8 *fname);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_BL_Pin GPIO_PIN_0
#define LCD_BL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define T_BIN		0X00	//binÎÄ¼þ
#define T_LRC		0X10	//lrcÎÄ¼þ

#define T_NES		0X20	//nesÎÄ¼þ
#define T_SMS		0X21	//smsÎÄ¼þ

#define T_TEXT		0X30	//.txtÎÄ¼þ
#define T_C			0X31	//.cÎÄ¼þ
#define T_H			0X32    //.hÎÄ¼þ

#define T_WAV		0X40	//WAVÎÄ¼þ
#define T_MP3		0X41	//MP3ÎÄ¼þ
#define T_APE		0X42	//APEÎÄ¼þ
#define T_FLAC		0X43	//FLACÎÄ¼þ

#define T_BMP		0X50	//bmpÎÄ¼þ
#define T_JPG		0X51	//jpgÎÄ¼þ
#define T_JPEG		0X52	//jpegÎÄ¼þ
#define T_GIF		0X53	//gifÎÄ¼þ

#define T_AVI		0X60	//aviÎÄ¼þ
#define	TYPE_BIN		(0X00)
#define	TYPE_LRC		(0X10)
#define	TYPE_GAME		(0X20)
#define	TYPE_TEXT		(0X30)
#define	TYPE_MUSIC		(0X40)
#define	TYPE_PICTURE	(0X50)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
