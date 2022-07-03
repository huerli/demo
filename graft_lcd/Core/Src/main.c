/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "retarget.h"
#include "stdio.h"
#include "lcd.h"
#include "string.h"
#include "piclib.h"
#include "esp8266.h"
#include "w25q.h"
#include "malloc.h"
#include "fontupd.h"
#include "text.h"
#include "exfuns.h"
#include "ff.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
__attribute__((aligned(4))) u8 SDIO_DATA_BUFFER[512];
#define SD_TIMEOUT 			((uint32_t)100000000)
#define SD_TRANSFER_OK     	((uint8_t)0x00)
#define SD_TRANSFER_BUSY   	((uint8_t)0x01)

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


u16 pic_get_tnum(u8 *path)
{
    u8 res;
    u16 rval=0;
    DIR tdir;
    FILINFO *tfileinfo;
    tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));
    res=f_opendir(&tdir,(const TCHAR*)path);

    if(res==FR_OK&&tfileinfo)
    {
        while(1)
        {
            res=f_readdir(&tdir,tfileinfo);
            if(res!=FR_OK||tfileinfo->fname[0]==0)break;
            res=f_typetell((u8*)tfileinfo->fname);
            if((res&0XF0)==0X50)
            {
                rval++;
            }
        }
    }
    myfree(SRAMIN,tfileinfo);
    return rval;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    u8 res;
    DIR picdir;
    FILINFO *picfileinfo;
    u8 *pname;
    u16 totpicnum;
    u16 curindex;
    u8 key;
    u8 pause=0;
    u8 t;
    u16 temp;
    u32 *picoffsettbl;
    uint8_t lcd_id[12];
    u32 fontcnt;
    u8 i,j;
    u8 fontx[2];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
    my_mem_init(SRAMIN);
    exfuns_init();
    f_mount(fs[0],"0:",1);
    f_mount(fs[1],"1:",1);
    RetargetInit(&huart1);
    HAL_Delay(50);
    LCD_Init();
    POINT_COLOR=RED;
    sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);
    Show_Str(30,30,200,16,"汉字显示实验:",16,0);
    LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
    LCD_ShowString(30,110,200,16,16,lcd_id);
    LCD_ShowString(30,130,200,12,12,"2022/7/2");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==GPIO_PIN_SET);

    POINT_COLOR=RED;
    while(font_init())
    {
UPD:
        LCD_Clear(WHITE);
        POINT_COLOR=RED;
        LCD_ShowString(30,70,200,16,16,"SD Card OK");
        LCD_ShowString(30,90,200,16,16,"Font Updating...");
        key=update_font(20,110,16,"0:");
        while(key)
        {
            LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
            HAL_Delay(200);
            LCD_Fill(20,110,200+20,110+16,WHITE);
            HAL_Delay(200);
        }
        LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
        HAL_Delay(1500);
        LCD_Clear(WHITE);
    }
    POINT_COLOR=RED;

    Show_Str(30,130,200,16,"按KEY0",16,0);
    POINT_COLOR=BLUE;
    Show_Str(30,150,200,16,"内码高字节:",16,0);
    Show_Str(30,170,200,16,"内码低字节:",16,0);
    Show_Str(30,190,200,16,"汉字计数器:",16,0);

    Show_Str(30,220,200,24,"对应汉字:",24,0);
    while(1)
    {
        fontcnt=0;
        for(i=0x81;i<0xff;i++)
        {
            fontx[0]=i;
            LCD_ShowNum(118,150,i,3,16);
            for(j=0x40;j<0xfe;j++)
            {
                if(j==0x7f)continue;
                fontcnt++;
                LCD_ShowNum(118,170,j,3,16);
                LCD_ShowNum(118,190,fontcnt,5,16);
                fontx[1]=j;
                Show_Font(30+132,220,fontx,24,0);
                Show_Font(30+144,244,fontx,16,0);
                Show_Font(30+108,260,fontx,12,0);
                t=200;
                while(t--)
                {
                    HAL_Delay(1);
                    key=KEY_Scan(0);
                    if(key==KEY0_PRES) goto UPD;
                }
                HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
            }
        }
    }
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
SD_HandleTypeDef        SDCARD_Handler;
HAL_SD_CardInfoTypeDef  SDCardInfo;
DMA_HandleTypeDef SDTxDMAHandler,SDRxDMAHandler;

u8 SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    u8 sta;
    sta=HAL_SD_GetCardInfo(&SDCARD_Handler,cardinfo);
    return sta;
}


u8 SD_GetCardState(void)
{
    return((HAL_SD_GetCardState(&SDCARD_Handler)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}


u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)
{
    u8 sta=HAL_OK;
    u32 timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//??��?��?????(POLLING????,?????????��??SDIO??????����!!!)
    sta=HAL_SD_ReadBlocks(&SDCARD_Handler, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);

    while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
        if(timeout-- == 0)
        {
            sta=SD_TRANSFER_BUSY;
        }
    }
    INTX_ENABLE();//????��?????
    return sta;
}

u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt)
{
    u8 sta=HAL_OK;
    u32 timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//??��?��?????(POLLING????,?????????��??SDIO??????����!!!)
    sta=HAL_SD_WriteBlocks(&SDCARD_Handler,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);

    //????SD?��???��
    while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
        if(timeout-- == 0)
        {
            sta=SD_TRANSFER_BUSY;
        }
    }
    INTX_ENABLE();//????��?????
    return sta;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
